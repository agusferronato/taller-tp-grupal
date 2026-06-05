#ifndef TILE_H
#define TILE_H

#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <utility>
#include <algorithm> 
#include "Info.h" 
#include <SDL2pp/SDL2pp.hh>

#define GRASS_TEXTURE_ID 0


struct GridItem {
    int texture_id;
    int x_start, y_start;
    int x_end, y_end;
    int i, j;
};

class Tile {
private:
    int id; 
    int texture_id;
    int priority;
    int origin_i, origin_j;
    int width_px, height_px;
    
    int min_i{0}, min_j{0};
    int max_i{0}, max_j{0};

    std::vector<GridItem> items;
    std::set<std::tuple<int, int, int>> localCollidables;

public:
    Tile(int id, int texture_id, int priority, int start_i, int start_j, int w_px, int h_px)
        : id(id), texture_id(texture_id), priority(priority), 
          origin_i(start_i), origin_j(start_j), width_px(w_px), height_px(h_px) {}

    int getId() const { return id; }
    int getTextureId() const { return texture_id; }
    int getPriority() const { return priority; }
    int getOriginI() const { return origin_i; }
    int getOriginY() const { return origin_j; }
    int getWidthPx() const { return width_px; }
    int getHeightPx() const { return height_px; }
    
    int getMinI() const { return min_i; }
    int getMinJ() const { return min_j; }
    int getMaxI() const { return max_i; }
    int getMaxJ() const { return max_j; }

    const std::vector<GridItem>& getItems() const { return items; }
    const std::set<std::tuple<int, int, int>>& getCollidableCells() const { return localCollidables; }

    void addItem(GridItem item) { 
        if (items.empty()) {
            min_i = max_i = item.i;
            min_j = max_j = item.j;
        } else {
            if (item.i < min_i) min_i = item.i;
            if (item.i > max_i) max_i = item.i;
            if (item.j < min_j) min_j = item.j;
            if (item.j > max_j) max_j = item.j;
        }
        items.push_back(item); 
    }

    void addCollidable(int i, int j) { localCollidables.insert({i, j, priority}); }

    void updatePosition(int new_i, int new_j) {
        int offset_i = new_i - origin_i;
        int offset_j = new_j - origin_j;
        origin_i = new_i;
        origin_j = new_j;

        min_i += offset_i;
        max_i += offset_i;
        min_j += offset_j;
        max_j += offset_j;

        for (auto& item : items) {
            item.i += offset_i;
            item.j += offset_j;
        }

        std::set<std::tuple<int, int, int>> updatedCollidables;
        for (auto& cell : localCollidables) {
            updatedCollidables.insert({std::get<0>(cell) + offset_i, std::get<1>(cell) + offset_j, priority});
        }
        localCollidables = std::move(updatedCollidables);
    }
};

#endif