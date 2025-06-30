#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "calc/TypeDefs.h"
#include "sampling/Material.h"

struct IntersectionData {
  Material material;
  Vec3 p{};
  Vec3 pN{}; /// Smooth normal
  Vec3 pNN{}; /// Not smooth normal
  float t = 99999.9f;
  bool intersection = false;
  Vec3 rayDir{};
};

#endif // !INTERSECTION_DATA_H
