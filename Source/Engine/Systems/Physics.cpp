#pragma once

#include "Physics.h"

#include "Map.h"

constexpr int marginSize = 5;

constexpr float forceDecay = 0.02f;
constexpr float foceBaseDist = 100.0f;

static bool CollidingLeft(Map* map, MT::CompositeRect& collider, float x) {
	int newX = static_cast<int>(std::ceil(x)) + static_cast<int>(collider.x);

	Tile* tileUp = map->GetTile({ newX, static_cast<int>(collider.y) + marginSize });

	Tile* tileCenter = map->GetTile({ newX, static_cast<int>(collider.y + (collider.h * 0.5f)) });

	Tile* tileDown = map->GetTile({ newX, static_cast<int>(collider.y) + collider.h - marginSize });

	if (!tileUp || !tileDown || !tileCenter) {
		collider.x = MapPos::minX;
		return true;
	}

	if (!tileUp->passable || !tileDown->passable || !tileCenter->passable) {
		collider.x = tileUp->rect.x + tileUp->rect.w;
		return true;
	}
	return false;
}

static bool ColidingRight(Map* map, MT::CompositeRect& collider, float x) {
	int newX = static_cast<int>(std::ceil(x)) + static_cast<int>(collider.x + collider.w);

	Tile* tileUp = map->GetTile({ newX, static_cast<int>(collider.y) + marginSize });

	Tile* tileCenter = map->GetTile({ newX, static_cast<int>(collider.y + (collider.h * 0.5)) });

	Tile* tileDown = map->GetTile({ newX, static_cast<int>(collider.y) + collider.h - marginSize });

	if (!tileUp || !tileDown || !tileCenter) {
		collider.x = MapPos::maxX - collider.w;
		return true;
	}

	if (!tileUp->passable || !tileDown->passable || !tileCenter->passable) {
		collider.x = tileUp->rect.x - collider.w;
		return true;
	}
	return false;
}

static bool ColidingUp(Map* map, MT::CompositeRect& collider, float y) {
	int newY = static_cast<int>(std::ceil(y)) + static_cast<int>(collider.y);

	Tile* tileLeft = map->GetTile({ static_cast<int>(collider.x) + marginSize, newY });

	Tile* tileCenter = map->GetTile({ static_cast<int>(collider.x + (collider.w * 0.5)), newY });

	Tile* tileRight = map->GetTile({ static_cast<int>(collider.x) + collider.w - marginSize, newY });

	if (!tileLeft || !tileRight || !tileCenter) {
		collider.y = MapPos::minY;
		return true;
	}

	if (!tileLeft->passable || !tileRight->passable || !tileCenter->passable) {
		collider.y = tileLeft->rect.y + tileLeft->rect.h;
		return true;
	}
	return false;
}

static bool ColidingDown(Map* map, MT::CompositeRect& collider, float y) {
	int newY = static_cast<int>(std::ceil(y)) + static_cast<int>(collider.y + collider.h);

	Tile* tileLeft = map->GetTile({ static_cast<int>(collider.x) + marginSize, newY });

	Tile* tileCenter = map->GetTile({ static_cast<int>(collider.x + (collider.w * 0.5)), newY });

	Tile* tileRight = map->GetTile({ static_cast<int>(collider.x) + collider.w - marginSize, newY });

	if (!tileLeft || !tileRight || !tileCenter) {
		collider.y = MapPos::maxY - collider.h;
		return true;
	}

	if (!tileLeft->passable || !tileRight->passable || !tileCenter->passable) {
		collider.y = tileLeft->rect.y - collider.h;
		return true;
	}
	return false;
}

void MoveComponent::ApplyForce(const Point& origin, float power, bool atract) {
	float maxDist = foceBaseDist * power;
	float dist = CalculateEuclidean(origin, collider.GetCenter());

	if (dist > maxDist) {
		return;
	}
	// Base power multiplied by distance
	float distPower = power - (power * (dist / maxDist));

	glm::vec2 appliedForce = CreateDirVec(collider.GetCenter(), origin, distPower);

	if (!atract) {
		appliedForce = -appliedForce;
	}

	force += appliedForce;
}

void MoveComponent::Move(Map *map, float x, float y) {
	x += force.x;
	y += force.y;
	bool colidingRight = ColidingRight(map, collider, x);
	bool colidingLeft = CollidingLeft(map, collider, x);
	bool colidingDown = ColidingDown(map, collider, y);
	bool colidingUp = ColidingUp(map, collider, y);
	if (x > 0 && !colidingRight) {
		collider.x += x;
	}
	else if (x < 0 && !colidingLeft) {
		collider.x += x;
	}

	if (y > 0 && !colidingDown) {
		collider.y += y;
	}
	else if (y < 0 && !colidingUp) {
		collider.y += y;
	}

	float len = glm::length(force);
	if (len <= forceDecay) {
		force = glm::vec2(0.0f, 0.0f);
	}
	else {
		force *= (len - forceDecay) / len;
	}

	AdjustRect();

}