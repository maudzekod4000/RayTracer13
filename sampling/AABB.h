#pragma once

#include <limits>
#include <assert.h>

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
  explicit AABB(): AABB(-XMVectorSplatInfinity(), XMVectorSplatInfinity()) {
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

  inline void split(int component, AABB& first, AABB& second) const {
    XMVECTOR bbCenterVec = XMLoadFloat3(&boundingBox.Center);
    XMVECTOR bbExtentsVec = XMLoadFloat3(&boundingBox.Extents);

    // The min and max of the original bounding box.
    XMVECTOR minVec = bbCenterVec - bbExtentsVec;
    XMVECTOR maxVec = bbCenterVec + bbExtentsVec;

    XMFLOAT3 min;
    XMStoreFloat3(&min, minVec);
    XMFLOAT3 max;
    XMStoreFloat3(&max, maxVec);

    XMVECTOR newFirstBoxMin = minVec;
    XMVECTOR newFirstBoxMax;

    XMVECTOR newSecondBoxMin;
    XMVECTOR newSecondBoxMax = maxVec;

    if (component == 0) {
      newFirstBoxMax = XMVectorSet(boundingBox.Center.x, max.y, max.z, 0.0f);
      newSecondBoxMin = XMVectorSet(boundingBox.Center.x, min.y, min.z, 0.0f);
    }
    else if (component == 1) {
      newFirstBoxMax = XMVectorSet(max.x, boundingBox.Center.y, max.z, 0.0f);
      newSecondBoxMin = XMVectorSet(min.x, boundingBox.Center.y, min.z, 0.0f);
    }
    else if (component == 2) {
      newFirstBoxMax = XMVectorSet(max.x, max.y, boundingBox.Center.z, 0.0f);
      newSecondBoxMin = XMVectorSet(min.x, min.y, boundingBox.Center.z, 0.0f);
    }
    else {
      assert(false);
    }

    BoundingBox::CreateFromPoints(first.boundingBox, newFirstBoxMin, newFirstBoxMax);
    BoundingBox::CreateFromPoints(second.boundingBox, newSecondBoxMin, newSecondBoxMax);
  }
};
