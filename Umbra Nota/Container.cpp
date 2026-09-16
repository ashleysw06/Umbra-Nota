#include<vector>
#include <SFML/Graphics.hpp>

#include "UIRect.h"
#include "Container.h"

Container::Container() {
    UIRect lRect;
    rect = &lRect;
}

Container::Container(UIRect* aRect) {
    rect = aRect;
}

Container::Container(float x, float y, float width, float height) {
    UIRect lRect(x, y, width, height);
    rect = &lRect;
}

void Container::SetRect(UIRect* aRect) {
    rect = aRect;
}

RectangleShape Container::GetShape() {
    RectangleShape container({ rect->width, rect->height });
    container.setPosition({ rect->x - rect->width / 2, rect->y - rect->height / 2 });
    return container;
}