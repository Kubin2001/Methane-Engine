#pragma once

#include <sol/sol.hpp>

#include "Physics.h"
#include "Renderer.h"


class Map;



class Object {
public:	
	MT::CompositeRect rect;
	MT::Color color;
	MT::Texture* tex = nullptr;
	sol::safe_function * behaviour = nullptr;

};

