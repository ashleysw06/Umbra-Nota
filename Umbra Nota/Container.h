#include "UIRect.h"

using namespace sf;
using namespace std;

#pragma once
class Container {
public:
    static Vector2f* screenSize;

    vector<Container*> children;
    UIRect* rect;

    Container();
    Container(UIRect* rect);
    Container(float x, float y, float width, float height);

    void SetRect(UIRect* rect);

    RectangleShape GetShape();

    enum ParentChildInteraction {
        LAYOUT_IGNORE,
        LAYOUT_HORIZONTAL,
        LAYOUT_VERTICAL,
        LAYOUT_GRID
    };
};