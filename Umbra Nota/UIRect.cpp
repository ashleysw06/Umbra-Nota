#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include "UIRect.h"

using namespace sf;

UIRect::UIRect() {
    UIRect(0, 0, 100, 100);
}

UIRect::UIRect(float aX, float aY, float aWidth, float aHeight) {
    position = { aX, aY };
    size = { aWidth, aHeight };
}

float UIRect::GetLeft() {
    return position.x - size.x / 2;
}

float UIRect::GetTop() {
    return position.y - size.y / 2;
}

float UIRect::GetRight() {
    return position.x + size.x / 2;
}

float UIRect::GetBottom() {
    return position.y + size.y / 2;
}

Vector2f UIRect::GetTopLeft() {
    return { GetLeft(), GetTop() };
}

Vector2f UIRect::GetTopRight() {
    return { GetRight(), GetTop() };
}

Vector2f UIRect::GetBottomLeft() {
    return { GetLeft(), GetBottom() };
}

Vector2f UIRect::GetBottomRight() {
    return { GetRight(), GetTop() };
}

bool UIRect::Contains(Vector2f aPoint) {
    bool inBoundsX = aPoint.x >= GetLeft() && aPoint.x <= GetRight();
    bool inBoundsY = aPoint.y >= GetTop() && aPoint.y <= GetBottom();
    return inBoundsX && inBoundsY;
}

bool UIRect::Contains(UIRect aRect) {
    bool inBoundsLeft = aRect.GetLeft() >= GetLeft() && aRect.GetLeft() <= GetRight();
    bool inBoundsRight = aRect.GetRight() <= GetRight() && aRect.GetRight() >= GetLeft();
    bool inBoundsX = inBoundsLeft || inBoundsRight;

    bool inBoundsTop = aRect.GetTop() >= GetTop() && aRect.GetTop() <= GetBottom();
    bool inBoundsBottom = aRect.GetBottom() <= GetBottom() && aRect.GetBottom() >= GetTop();
    bool inBoundsY = inBoundsTop || inBoundsBottom;
    return inBoundsX && inBoundsY;
}
