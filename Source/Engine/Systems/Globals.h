#pragma once

namespace EditorGlobals {
	// 20 upper layers are reserved for editor
	constexpr int UILayerLowest = 80;
	constexpr int UILayerMenuBar = 95;
	constexpr int UILayerHighest = 99;
}


inline static size_t anonymousID = 0;

inline static std::string AnonUIName() {
	return std::to_string(anonymousID++);
}

inline static MT::Rect RectFillBetweenY(int x, int w, const MT::Rect& up, const MT::Rect& down) {
	return MT::Rect{
		x,
		up.y + up.h,
		w,
		down.y - (up.y + up.h)
	};
}