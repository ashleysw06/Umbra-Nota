#include "ScrollView.h"
#include "UIRect.h"
#include <iostream>

ScrollView::ScrollView(Layout aLayout /*= NONE*/) : Container() {
	contentSize.x = padding * 2 - margin;
	contentSize.y = padding * 2 - margin;
	layout = aLayout;
}

ScrollView::ScrollView(UIRect* aRect, Layout aLayout /*= NONE*/) : Container(aRect) {
	contentSize.x = padding * 2 - margin;
	contentSize.y = padding * 2 - margin;
	layout = aLayout;
}

ScrollView::ScrollView(float x, float y, float width, float height, Layout aLayout /*= NONE*/) : Container(x, y, width, height) {
	contentSize.x = padding * 2 - margin;
	contentSize.y = padding * 2 - margin;
	layout = aLayout;
}

void ScrollView::AddScrollVelocityX(float x) {
	scrollVelocity.x = scrollVelocity.x + scrollSpeed * x;
}

void ScrollView::AddScrollVelocityY(float y) {
	scrollVelocity.y = scrollVelocity.y + scrollSpeed * y;
}

void ScrollView::AddScrollVelocity(Vector2f aVelocity)
{
}

void ScrollView::ApplyVelocity() {
	if (children.size() == 0) return;
	Container* child = children[0];
	Vector2f overflowSize = { contentSize.x - rect->size.x, contentSize.y - rect->size.y };

	if (abs(scrollVelocity.x) > scrollVelocityLimit) {
		scrollVelocity.x = (signbit(scrollVelocity.x) ? -1 : 1) * scrollVelocityLimit;
	}
	if (abs(scrollVelocity.y) > scrollVelocityLimit) {
		scrollVelocity.y = (signbit(scrollVelocity.y) ? -1 : 1) * scrollVelocityLimit;
	}

	scrollDistance += scrollVelocity;
	Vector2f scrollOverflow = { -scrollDistance.x - overflowSize.x, -scrollDistance.y - overflowSize.y };

	if (limitScrollToContainerBounds) {
		if (scrollDistance.x > 0) scrollDistance.x *= scrollElasticity;
		if (scrollOverflow.x > 0 && overflowSize.x > 0) {
			scrollDistance.x = -overflowSize.x - scrollOverflow.x * scrollElasticity;
		}
		else if (overflowSize.x < 0) scrollDistance.x *= scrollElasticity;

		if (scrollDistance.y > 0) scrollDistance.y *= scrollElasticity;
		if (scrollOverflow.y > 0 && overflowSize.y > 0) {
			scrollDistance.y = -overflowSize.y - scrollOverflow.y * scrollElasticity;
		}
		else if (overflowSize.y < 0) scrollDistance.y *= scrollElasticity;
	}
}

void ScrollView::ApplyDrag() {
	scrollVelocity *= scrollVelocityDrag;
}

void ScrollView::AddChild(Container* child) {
	Container::AddChild(child);
	cout << "Adding Child: " << contentSize.y << endl;

	switch (layout) {
	case ScrollView::NONE:
		break;
	case ScrollView::HORIZONTAL:
		contentSize.x += child->rect->size.x + margin;
		cout << "Size: " << contentSize.x << endl;
		cout << "New Content Size: " << contentSize.x << endl;
		break;
	case ScrollView::VERTICAL:
		contentSize.y += child->rect->size.y + margin;
		cout << "Size: " << contentSize.x << endl;
		cout << "New Content Size: " << contentSize.y << endl;
		break;
	case ScrollView::GRID:
		break;
	default:
		break;
	}

}
