#include "EmptyItem.h"
#include "Inventory.h"

EmptyItem::EmptyItem() : Item(0) {}

uint8_t EmptyItem::getID() const { return id; }

void EmptyItem::switchInInventory(Inventory &, uint8_t) {}
