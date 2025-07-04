#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "calc/TypeDefs.h"
#include "sampling/Material.h"
#include "Ray.h"

struct IntersectionData {
  Material material;
  Vec3 p{};
  Vec3 pN{}; /// Smooth normal
  Vec3 pNN{}; /// Non-smooth normal
  float t = 99999.9f;
  bool intersection = false;
  Ray ray;
};

#endif // !INTERSECTION_DATA_H
