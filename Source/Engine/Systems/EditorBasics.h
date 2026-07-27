#pragma once

#include "Rectangle.h"
#include "Renderer.h"

class EditorRect : public MT::CompositeRect{
public:
	MT::ColorA color;
	MT::Texture* tex = nullptr;
};
