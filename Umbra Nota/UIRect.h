#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>

using namespace sf;

#pragma once
class UIRect {
public:
    Vector2f position;
    Vector2f size;

    UIRect();
    UIRect(float aX, float aY, float aW, float aH);

    float GetTop();
    float GetLeft();
    float GetBottom();
    float GetRight();

    Vector2f GetTopLeft();
    Vector2f GetTopRight();
    Vector2f GetBottomLeft();
    Vector2f GetBottomRight();

    bool Contains(Vector2f aPoint);
    bool Contains(UIRect aRect);
};

