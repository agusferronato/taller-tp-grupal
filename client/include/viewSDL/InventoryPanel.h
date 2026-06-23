#ifndef INVENTORYPANEL_H
#define INVENTORYPANEL_H

#include <SDL2pp/SDL2pp.hh>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "ClientPlayer.h"
#include "TextureManager.h"

struct SlotRect {
  int x, y, w, h;
  bool hitTest(int px, int py) const;
};

enum class ClickTargetType { Equipment, Inventory, None };

struct ClickTarget {
  ClickTargetType type{ClickTargetType::None};
  int index{0};
};

constexpr int INV_PANEL_X = 686;
constexpr int INV_PANEL_Y = 128;

class InventoryPanel {
public:
  InventoryPanel(SDL2pp::Renderer &renderer,
                 const TextureManager &textureManager);

  void loadTextures();
  void render(const ClientPlayer &player);
  ClickTarget handleClick(int screenX, int screenY) const;

private:
  SDL2pp::Renderer &renderer;
  const TextureManager &textureManager;

  std::unique_ptr<SDL2pp::Texture> baseTex;
  std::unique_ptr<SDL2pp::Font> slotFont;

  SlotRect getEquipSlotRect(int idx) const;
  SlotRect getInventorySlotRect(int row, int col) const;

  void renderItemIcon(int slotX, int slotY, uint8_t itemId);
  void renderGoldText(const ClientPlayer &player);
  void renderCenteredText(const std::string &text, const SDL2pp::Rect &rect,
                          SDL_Color color);

  static constexpr int SLOT_W = 32;
  static constexpr int SLOT_H = 32;

  static constexpr int EQUIP_COLS = 4;
  static constexpr int EQUIP_Y = 39;

  static constexpr int INV_COLS = 6;
  static constexpr int INV_ROWS = 3;
  static constexpr int INV_Y0 = 122;
  static constexpr int INV_ROW_H = 39;

  static constexpr int BASE_W = 267;
  static constexpr int BASE_H = 294;

  static constexpr int SAFE_GOLD_X = 59;
  static constexpr int EXCESS_GOLD_X = 173;
  static constexpr int GOLD_VALUE_Y = 260;
  static constexpr int GOLD_VALUE_W = 74;
  static constexpr int GOLD_VALUE_H = 25;

  static constexpr int EQUIP_X[EQUIP_COLS] = {32, 89, 146, 203};
  static constexpr int INV_X[INV_COLS] = {20, 59, 98, 137, 176, 215};

  static constexpr uint8_t EQUIP_SLOT_IDS[EQUIP_COLS] = {0, 2, 1, 3};
};

#endif
