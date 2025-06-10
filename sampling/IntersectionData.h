#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "calc/TypeDefs.h"
#include "sampling/Triangle.h"

struct IntersectionData {
  Triangle triangle;
  Vec3 p{}; // TODO: Do we need to default initialize it?
  //Vec3 pN{};
  //Vec3 hitBaryCentricCoordinates{};
  //Vec3 pNNonSmooth{};
  float t = 99999.9f;
  bool intersection = false;
};

#endif // !INTERSECTION_DATA_H
