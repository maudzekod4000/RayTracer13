#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "calc/TypeDefs.h"
#include "sampling/Material.h"
#include "Ray.h"

struct IntersectionData {
  Material material;
  Vec p{};
  Vec pN{}; /// Smooth normal
  Vec pNN{}; /// Non-smooth normal
  float t = 99999.9f;
  bool intersection = false;
  Ray ray;
};

#endif // !INTERSECTION_DATA_H
