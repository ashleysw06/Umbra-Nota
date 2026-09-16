#include <SFML/System.hpp>

#pragma once
class UIRect {
public:
    float x, y, width, height;
    UIRect();
    UIRect(float aX, float aY, float aW, float aH);
    float GetTop();
    float GetLeft();
    float GetBottom();
    float GetRight();

    /*Vector2f GetTopLeft();
    Vector2f GetTopRight();
    Vector2f GetBottomLeft();
    Vector2f GetBottomRight();*/
};

