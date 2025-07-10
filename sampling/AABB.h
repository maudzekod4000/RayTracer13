#pragma once

#include <limits>

#include "calc/TypeDefs.h"
#include "sampling/Triangle.h"
#include "sampling/Vertex.h"

using namespace DirectX;

struct AABB {
  explicit AABB(const Vec& min, const Vec& max) {
    BoundingBox::CreateFromPoints(
      boundingBox,
      min, max
    );
  }
  explicit AABB(): AABB(XMVECTOR(std::numeric_limits<float>().min()), XMVECTOR(std::numeric_limits<float>().max())) {
  }
  explicit AABB(const Triangle& t): AABB() {
    // SIMD power mothefucka
    BoundingBox triangleBB1;
    BoundingBox::CreateFromPoints(triangleBB1, t.a.pos, t.b.pos);
    BoundingBox triangleBB2;
    BoundingBox::CreateFromPoints(triangleBB2, t.c.pos, t.c.pos);
    BoundingBox::CreateMerged(boundingBox, triangleBB1, triangleBB2);
  }
  DirectX::BoundingBox boundingBox; // backing structure

  inline void expand(const Triangle& t) {
    BoundingBox triangleBB1;
    BoundingBox::CreateFromPoints(triangleBB1, t.a.pos, t.b.pos);
    BoundingBox triangleBB2;
    BoundingBox::CreateFromPoints(triangleBB2, t.c.pos, t.c.pos);
    BoundingBox triangleBB;
    BoundingBox::CreateMerged(triangleBB, triangleBB1, triangleBB2);
    BoundingBox::CreateMerged(boundingBox, boundingBox, triangleBB);
  }

  inline bool intersect(const Triangle& triangle) const {
    return this->intersect(AABB(triangle));
  }

  inline bool intersect(const AABB& aabb) const {
    return boundingBox.Intersects(aabb.boundingBox);
  }

  inline bool intersect(const Ray& r) const {
    float dist;
    return boundingBox.Intersects(r.origin, r.dir, dist);
  }
};
