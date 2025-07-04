#ifndef RAY_H
#define RAY_H

#include "calc/TypeDefs.h"

struct Ray {
  Ray() = default;
  Ray(const Vec3& origin, const Vec3& dir, int bounceCount = 0, int maxBounces):
    origin(origin), dir(dir), bounceCount(bounceCount), maxBounces(maxBounces) {}

  Vec3 origin;
  Vec3 dir;
  int bounceCount;
  int maxBounces;
};

#endif // !RAY_H
