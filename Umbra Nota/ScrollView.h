#include <SFML/Graphics.hpp>
#include "Container.h"
#include "UIRect.h"

#pragma once
class ScrollView : public Container {
public:
    enum Layout {
        NONE,
        HORIZONTAL,
        VERTICAL,
        GRID
    };

    Vector2f scrollDistance = { 0, 0 };
    Vector2f scrollVelocity = { 0, 0 };
    Vector2f scrollOverflow = { 0, 0 };
    float scrollVelocityLimit = 50;
    float scrollVelocityDrag = 0.9;

    bool limitScrollToContainerBounds = true;
    float scrollSpeed = 7;
    float scrollElasticity = 0.9;
    
    float padding = 10;
    float margin = 5;

    Vector2f contentSize = { 0, 0 };
    Layout layout = NONE;

    ScrollView(Layout layout = NONE);
    ScrollView(UIRect* aRect, Layout layout = NONE);
    ScrollView(float x, float y, float width, float height, Layout layout = NONE);

    void AddScrollVelocityX(float x);
    void AddScrollVelocityY(float y);
    void AddScrollVelocity(Vector2f aVelocity);
    void ApplyVelocity();
    void ApplyDrag();

    void AddChild(Container* child) override;

};