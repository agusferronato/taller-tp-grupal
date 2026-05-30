#ifndef RENDERABLE_ENTITY_H
#define RENDERABLE_ENTITY_H


#include <SDL2pp/SDL2pp.hh>
#include "Camera.h"

class RenderableEntity {

private:
    int priority{1};

protected:
    bool wasRendered {false};

public:

    virtual void render(SDL2pp::Renderer& renderer, Camera& camera, unsigned int it) = 0;

    virtual int get_x() = 0;
    virtual int get_y() = 0;
    virtual int get_h() = 0;

    inline bool rendered() {
        return wasRendered;
    };

    inline bool hasPriority(int priority) {
        return this->priority == priority;
    };

    inline void clear() {
        wasRendered = false;        
    };


    virtual ~RenderableEntity() = default;
};


#endif
