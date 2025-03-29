#ifndef RAY_H
#define RAY_H

#include "TypeDefs.h"

struct Ray {
  Ray(const Vec3& origin, const Vec3& dir): origin(origin), dir(dir) {}
  Ray() : Ray(Vec3(0.0f), Vec3(0.0f)) {}
  Vec3 origin;
  Vec3 dir;
};

#endif // !RAY_H
