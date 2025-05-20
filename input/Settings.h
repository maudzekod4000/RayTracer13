#ifndef SETTINGS_H
#define SETTINGS_H

#include "TypeDefs.h"

// General settings that do not fall into any specific category (or at least not yet).
class Settings {
public:
	Settings(Vec3&& backgroundColor) noexcept;
private:
	// The color of the pixel when the ray does not hit anything.
	Vec3 backgroundColor; // TODO: Use PPMColor for this
};

#endif // !SETTINGS_H
