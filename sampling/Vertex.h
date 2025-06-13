#pragma once

#include "calc/TypeDefs.h"

// TODO: We need a way to calculate the smooth normal of a vertex
// Algorithm for calculating smooth normal
// 1. Gather a vector of vertices with the indexing matching the one from the scene.
// 2. Create a vector of triangles 
struct Vertex {
  Vec3 pos{};
  Vec3 normal{}; ///< Smooth normal.
};
