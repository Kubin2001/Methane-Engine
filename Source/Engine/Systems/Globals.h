#pragma once

#include "Rectangle.h"

namespace EditorGlobals {
	// 20 upper layers are reserved for editor
	constexpr int UILayerLowest = 80;
	constexpr int UILayerLow = 82;
	constexpr int UILayerSceneLayout = 82;
	constexpr int UILayerMenuBar = 95;
	constexpr int UILayerHighest = 99;
}


inline static MT::Rect RectFillBetweenY(int x, int w, const MT::Rect& up, const MT::Rect& down) {
	return MT::Rect{
		x,
		up.y + up.h,
		w,
		down.y - (up.y + up.h)
	};
}