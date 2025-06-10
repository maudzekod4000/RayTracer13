#ifndef RAY_H
#define RAY_H

#include "calc/TypeDefs.h"

struct Ray {
  Ray(const Vec3& origin, const Vec3& dir): origin(origin), dir(dir) {}

  Vec3 origin;
  Vec3 dir;
};

#endif // !RAY_H
