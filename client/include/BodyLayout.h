#ifndef BODY_LAYOUT_H
#define BODY_LAYOUT_H


class BodyLayout {


private:
    std::vector<std::vector<SpriteData>> layout;


public:

    BodyLayout(std::vector<std::vector<SpriteData>>& layout) : layout(layout);

    Layout getLayout(Direction direction, unsigned int iterator) {

        int row = 0;
        switch (direction) {
            case Direction::Down:
                row = 0;
                break;

            case Direction::Up:
                row = 1;
                break;

            case Direction::Left:
                row = 2;
                break;

            case Direction::Right:
                row = 3;
                break;

            default:
                break;
        }

        int count = layout[row].size();
        int frame = static_cast<int>(iterator * 0.25) % count;
        return layout[row][frame];
  }

};






#endif



