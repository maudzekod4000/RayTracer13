#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "calc/TypeDefs.h"

struct IntersectionData {
  Vec3 p{}; // TODO: Do we need to default initialize it?
  Vec3 color{};
  //Vec3 pN{};
  //Vec3 hitBaryCentricCoordinates{};
  //Vec3 pNNonSmooth{};
  float t = 9999.9f;
  bool intersection = false;
};

#endif // !INTERSECTION_DATA_H
