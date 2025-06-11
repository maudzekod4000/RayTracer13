#pragma once

#include "calc/TypeDefs.h"

// General settings that do not fall into any specific category (or at least not yet).
struct Settings {
	inline Settings(const Vec3& backgroundColor): backgroundColor(backgroundColor) {}

	// The color of the pixel when the ray does not hit anything.
	Vec3 backgroundColor; // TODO: Use PPMColor for this
};
