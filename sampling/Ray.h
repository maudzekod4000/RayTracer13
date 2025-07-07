#ifndef RAY_H
#define RAY_H

#include "calc/TypeDefs.h"

enum class RayType {
  CAMERA,
  GI
};

struct Ray {
  Ray() = default;
  Ray(const Vec3& origin, const Vec3& dir, int bounceCount = 0, RayType type = RayType::CAMERA):
    origin(origin), dir(dir), bounceCount(bounceCount), type(type) {}

  Vec3 origin;
  Vec3 dir;
  int bounceCount;
  int maxBounces;
  RayType type;
};

#endif // !RAY_H
