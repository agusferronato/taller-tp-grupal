#include "InventoryPanel.h"

#include "Formulas.h"

#include <string>
#include <unordered_map>

InventoryPanel::InventoryPanel(SDL2pp::Renderer &renderer,
                               const TextureManager &textureManager)
    : renderer(renderer), textureManager(textureManager) {}

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

  SDL2pp::Texture *tex = textureManager.getItemIcon(itemId);
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

  renderGoldText(player);
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

void InventoryPanel::renderGoldText(const ClientPlayer &player) {
  uint32_t totalGold = player.getGold();
  uint32_t excessGold =
      Formulas::calcularOroExceso(totalGold, player.getLevel());
  uint32_t safeGold = totalGold - excessGold;

  SDL_Color goldColor{255, 223, 128, 255};
  SDL2pp::Rect safeRect(INV_PANEL_X + SAFE_GOLD_X, INV_PANEL_Y + GOLD_VALUE_Y,
                        GOLD_VALUE_W, GOLD_VALUE_H);
  SDL2pp::Rect excessRect(INV_PANEL_X + EXCESS_GOLD_X,
                          INV_PANEL_Y + GOLD_VALUE_Y, GOLD_VALUE_W,
                          GOLD_VALUE_H);

  renderCenteredText(std::to_string(safeGold), safeRect, goldColor);
  renderCenteredText(std::to_string(excessGold), excessRect, goldColor);
}

void InventoryPanel::renderCenteredText(const std::string &text,
                                        const SDL2pp::Rect &rect,
                                        SDL_Color color) {
  if (!slotFont)
    return;

  SDL2pp::Surface surf = slotFont->RenderUTF8_Solid(text, color);
  SDL2pp::Texture tex(renderer, surf);
  int x = rect.GetX() + (rect.GetW() - surf.GetWidth()) / 2;
  int y = rect.GetY() + (rect.GetH() - surf.GetHeight()) / 2;

  renderer.Copy(tex, SDL2pp::NullOpt,
                SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}
