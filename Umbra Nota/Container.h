#include "UIRect.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

#pragma once
class Container {
public:
    static Vector2f* screenSize;

    vector<Container*> children;
    UIRect* rect;

    Container();
    Container(UIRect* aRect);
    Container(float x, float y, float width, float height);

    void SetRect(UIRect* rect);

    virtual void AddChild(Container* child);

    RectangleShape GetShape();
};