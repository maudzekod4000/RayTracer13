#ifndef RAY_H
#define RAY_H

#include "calc/TypeDefs.h"

enum class RayType {
  CAMERA,
  GI
};

struct Ray {
  Ray() = default;
  Ray(const Vec& origin, const Vec& dir, int bounceCount = 0, RayType type = RayType::CAMERA):
    origin(origin), dir(dir), bounceCount(bounceCount), type(type) {}

  Vec origin;
  Vec dir;
  int bounceCount;
  int maxBounces;
  RayType type;
};

#endif // !RAY_H
