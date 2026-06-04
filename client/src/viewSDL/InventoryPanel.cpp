#include "InventoryPanel.h"

#include <string>
#include <unordered_map>

InventoryPanel::InventoryPanel(SDL2pp::Renderer &renderer,
                               const ItemTextureCache &itemTextureCache)
    : renderer(renderer), itemTextureCache(itemTextureCache) {}

void InventoryPanel::loadTextures() {
  SDL2pp::Surface baseSurf("assets/HUD/UserInventory/base.png");
  baseTex = std::make_unique<SDL2pp::Texture>(renderer, baseSurf);
}

SlotRect InventoryPanel::getEquipSlotRect(int idx) const {
  return {INV_PANEL_X + EQUIP_X[idx], INV_PANEL_Y + EQUIP_Y, SLOT_W, SLOT_H};
}

SlotRect InventoryPanel::getInventorySlotRect(int row, int col) const {
  int y = INV_PANEL_Y + INV_Y0 + row * INV_ROW_H;
  return {INV_PANEL_X + INV_X[col], y, SLOT_W, SLOT_H};
}

void InventoryPanel::drawBorderRect(int x, int y, int w, int h,
                                     SDL_Color color) {
  renderer.SetDrawColor(color.r, color.g, color.b, color.a);
  SDL2pp::Rect top(x, y, w, 1);
  renderer.FillRect(top);
  SDL2pp::Rect bottom(x, y + h - 1, w, 1);
  renderer.FillRect(bottom);
  SDL2pp::Rect left(x, y, 1, h);
  renderer.FillRect(left);
  SDL2pp::Rect right(x + w - 1, y, 1, h);
  renderer.FillRect(right);
}

void InventoryPanel::renderItemIcon(int slotX, int slotY, uint8_t itemId) {
  if (itemId == 0)
    return;

  SDL2pp::Texture *tex = itemTextureCache.get(itemId);
  if (tex) {
    renderer.Copy(*tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(slotX, slotY, SLOT_W, SLOT_H));
    return;
  }

  const ItemDef &def = ITEM_TABLE[itemId];

  SDL_Color color{180, 150, 100, 255};
  switch (def.type) {
  case ItemType::Weapon:
    color = {200, 80, 80, 255};
    break;
  case ItemType::Armor:
    color = {80, 120, 200, 255};
    break;
  case ItemType::Helmet:
    color = {80, 200, 80, 255};
    break;
  case ItemType::Shield:
    color = {180, 180, 80, 255};
    break;
  case ItemType::Staff:
    color = {200, 130, 80, 255};
    break;
  case ItemType::PotionHp:
    color = {200, 60, 60, 255};
    break;
  case ItemType::PotionMana:
    color = {60, 100, 220, 255};
    break;
  }

  SDL2pp::Rect bg(slotX + 1, slotY + 1, SLOT_W - 2, SLOT_H - 2);
  renderer.SetDrawColor(30, 30, 30, 200);
  renderer.FillRect(bg);

  drawBorderRect(slotX + 1, slotY + 1, SLOT_W - 2, SLOT_H - 2, color);

  std::string label;
  label += def.name[0];
  try {
    SDL2pp::Font font("fonts/Vera.ttf", 11);
    SDL2pp::Surface surf = font.RenderUTF8_Solid(label, color);
    SDL2pp::Texture tex(renderer, surf);
    int tx = slotX + (SLOT_W - surf.GetWidth()) / 2;
    int ty = slotY + (SLOT_H - surf.GetHeight()) / 2;
    renderer.Copy(tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(tx, ty, surf.GetWidth(), surf.GetHeight()));
  } catch (...) {
  }
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
  }

  const auto &invItems = player.getInventory().getItems();
  for (int row = 0; row < INV_ROWS; row++) {
    for (int col = 0; col < INV_COLS; col++) {
      int idx = row * INV_COLS + col;
      if (idx >= 20)
        break;
      uint8_t itemId = invItems[idx];
      if (itemId == 0)
        continue;
      SlotRect r = getInventorySlotRect(row, col);
      renderItemIcon(r.x, r.y, itemId);
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
