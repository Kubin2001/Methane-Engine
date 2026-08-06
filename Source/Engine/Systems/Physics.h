#pragma once

#include "Rectangle.h"
#include "glm.hpp"


class Map;

inline glm::vec2 CreateDirVec(const Point &start, const Point &dest, const float moveSpeed) {
	glm::vec2 dir = glm::vec2{ dest.x, dest.y } - glm::vec2{ start.x, start.y };
	float len = glm::length(dir);

	if (len > 0.0001f) { 
		dir /= len;   
		return dir * moveSpeed;
	}
	return glm::vec2(0.0f);

}



class MoveComponent {
public:
	MT::CompositeRect collider{};
	MT::Rect rect{};
	glm::vec2 dir{};
	glm::vec2 force{};
	float moveSpeed = 1.0f;

	void AdjustCollider() {
		collider.x = static_cast<float>(rect.x + ((rect.w - collider.w) / 2.0f));
		collider.y = static_cast<float>(rect.y + ((rect.h - collider.h) / 2.0f));
	}

	void AdjustRect() {
		rect.x = static_cast<int>(collider.x - ((rect.w - collider.w) / 2.0f));
		rect.y = static_cast<int>(collider.y - ((rect.h - collider.h) / 2.0f));
	}


	MoveComponent() = default;

	MoveComponent(const MT::Rect &rect, float moveSpeed = 1.0f, float colliderRatio = 1.0f) {
		this->rect = rect;
		this->moveSpeed = moveSpeed;
		this->collider.w = rect.w * colliderRatio;
		this->collider.h = rect.h * colliderRatio;
		AdjustCollider();
	}

	void ApplyForce(const Point& origin, float power, bool atract = false);

	void Move(Map* map, float x, float y);
};