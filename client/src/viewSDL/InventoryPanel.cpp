#include "InventoryPanel.h"

#include <string>
#include <unordered_map>

InventoryPanel::InventoryPanel(SDL2pp::Renderer &renderer,
                               const ItemTextureCache &itemTextureCache)
    : renderer(renderer), itemTextureCache(itemTextureCache) {}

void InventoryPanel::loadTextures() {
  SDL2pp::Surface baseSurf("assets/HUD/UserInventory/base.png");
  baseTex = std::make_unique<SDL2pp::Texture>(renderer, baseSurf);
  slotFont = std::make_unique<SDL2pp::Font>("fonts/Vera.ttf", 9);
}

SlotRect InventoryPanel::getEquipSlotRect(int idx) const {
  return {INV_PANEL_X + EQUIP_X[idx], INV_PANEL_Y + EQUIP_Y, SLOT_W, SLOT_H};
}

SlotRect InventoryPanel::getInventorySlotRect(int row, int col) const {
  int y = INV_PANEL_Y + INV_Y0 + row * INV_ROW_H;
  return {INV_PANEL_X + INV_X[col], y, SLOT_W, SLOT_H};
}

void InventoryPanel::renderItemIcon(int slotX, int slotY, uint8_t itemId) {
  if (itemId == 0)
    return;

  SDL2pp::Texture *tex = itemTextureCache.get(itemId);
  if (!tex)
    return;

  renderer.Copy(*tex, SDL2pp::NullOpt,
                SDL2pp::Rect(slotX, slotY, SLOT_W, SLOT_H));
}

void InventoryPanel::render(const ClientPlayer &player) {
  if (!baseTex)
    return;

  renderer.Copy(*baseTex, SDL2pp::NullOpt,
                SDL2pp::Rect(INV_PANEL_X, INV_PANEL_Y, BASE_W, BASE_H));

  auto getEquipItem = [&](uint8_t equipSlotIdx) -> uint8_t {
    switch (equipSlotIdx) {
    case 0:
      return player.getEquippedWeapon();
    case 1:
      return player.getEquippedArmor();
    case 2:
      return player.getEquippedHelmet();
    case 3:
      return player.getEquippedShield();
    default:
      return 0;
    }
  };

  for (int i = 0; i < EQUIP_COLS; i++) {
    SlotRect r = getEquipSlotRect(i);
    uint8_t itemId = getEquipItem(EQUIP_SLOT_IDS[i]);
    if (itemId != 0) {
      renderItemIcon(r.x, r.y, itemId);
    }
    try {
      SDL_Color gray{140, 140, 140, 180};
      std::string numStr = std::to_string(i);
      SDL2pp::Surface surf = slotFont->RenderUTF8_Solid(numStr, gray);
      SDL2pp::Texture tex(renderer, surf);
      int tx = r.x + r.w - surf.GetWidth() - 2;
      int ty = r.y + r.h - surf.GetHeight() - 1;
      renderer.Copy(tex, SDL2pp::NullOpt,
                    SDL2pp::Rect(tx, ty, surf.GetWidth(), surf.GetHeight()));
    } catch (...) {
    }
  }

  const auto &invItems = player.getInventory().getItems();
  for (int row = 0; row < INV_ROWS; row++) {
    for (int col = 0; col < INV_COLS; col++) {
      int idx = row * INV_COLS + col;
      if (idx >= 20)
        break;
      uint8_t itemId = invItems[idx];
      SlotRect r = getInventorySlotRect(row, col);
      if (itemId != 0) {
        renderItemIcon(r.x, r.y, itemId);
      }
      try {
        SDL_Color gray{140, 140, 140, 180};
        std::string numStr = std::to_string(idx);
        SDL2pp::Surface surf = slotFont->RenderUTF8_Solid(numStr, gray);
        SDL2pp::Texture tex(renderer, surf);
        int tx = r.x + r.w - surf.GetWidth() - 2;
        int ty = r.y + r.h - surf.GetHeight() - 1;
        renderer.Copy(tex, SDL2pp::NullOpt,
                      SDL2pp::Rect(tx, ty, surf.GetWidth(), surf.GetHeight()));
      } catch (...) {
      }
    }
  }

}

ClickTarget InventoryPanel::handleClick(int screenX, int screenY) const {
  for (int i = 0; i < EQUIP_COLS; i++) {
    SlotRect r = getEquipSlotRect(i);
    if (r.hitTest(screenX, screenY)) {
      return {ClickTargetType::Equipment, EQUIP_SLOT_IDS[i]};
    }
  }
  for (int row = 0; row < INV_ROWS; row++) {
    for (int col = 0; col < INV_COLS; col++) {
      int idx = row * INV_COLS + col;
      if (idx >= 20)
        break;
      SlotRect r = getInventorySlotRect(row, col);
      if (r.hitTest(screenX, screenY)) {
        return {ClickTargetType::Inventory, idx};
      }
    }
  }
  return {ClickTargetType::None, -1};
}
