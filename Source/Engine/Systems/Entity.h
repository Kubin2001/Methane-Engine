#pragma once

#include <sol/sol.hpp>

#include "Physics.h"
#include "Renderer.h"


class Map;

struct JsonEntity {
	MT::Texture* tex = nullptr;
	sol::safe_function behaviour{};
	Point size{};
};


class Entity {
protected:
public:
	Map* map = nullptr;
	MoveComponent mc{};
	MT::Texture* tex = nullptr;
	sol::safe_function behaviour{};

	Point region{}; // Map region in which entity resides x rows y columns


	Entity(Map* map, const MT::Rect& rect, MT::Texture* tex, float moveSpeed, float colliderRatio, sol::safe_function& behaviour) : mc(rect, moveSpeed, colliderRatio) {
		this->tex = tex;
		this->map = map;
		this->behaviour = behaviour;
	}

	void Update() {
		if (!behaviour.valid()) { return; }
		behaviour(this, map);

		mc.Move(map, mc.dir.x, mc.dir.y);
	}

	void ApplyForce(const Point& origin, float power, bool atract = false) {
		mc.ApplyForce(origin, power, atract);
	}

	Entity& operator = (const JsonEntity& other) {
		this->behaviour = other.behaviour;
		this->mc.rect.w = other.size.x;
		this->mc.rect.h = other.size.y;
		this->tex = other.tex;

		return *this;
	}
};

