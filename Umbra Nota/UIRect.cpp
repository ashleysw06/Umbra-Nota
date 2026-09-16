#include <SFML/System.hpp>
#include "UIRect.h"

using namespace sf;

UIRect::UIRect() {
    x = 0;
    y = 0;
    width = 100;
    height = 100;
}

UIRect::UIRect(float aX, float aY, float aWidth, float aHeight) {
    x = aX;
    y = aY;
    width = aWidth;
    height = aHeight;
}

float UIRect::GetLeft() {
    return x - width / 2;
}

float UIRect::GetTop() {
    return y - height / 2;
}

float UIRect::GetRight() {
    return x + width / 2;
}

float UIRect::GetBottom() {
    return y + height / 2;
}

//Vector2f UIRect::GetTopLeft() {
//    return { GetLeft(), GetTop() };
//}
//
//Vector2f UIRect::GetTopRight() {
//    return { GetRight(), GetTop() };
//}
//
//Vector2f UIRect::GetBottomLeft() {
//    return { GetLeft(), GetBottom() };
//}
//
//Vector2f UIRect::GetBottomRight() {
//    return { GetRight(), GetTop() };
//}
