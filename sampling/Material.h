#pragma once

#include "calc/TypeDefs.h"

using namespace DirectX;

enum class MaterialType {
  DIFFUSE,
  REFLECTIVE,
  REFRACTIVE,
  CONSTANT
};

struct Material {
  Vec albedo = XMVectorZero(); ///< Natural color of the object/material.
  MaterialType type = MaterialType::DIFFUSE;
  float ior = 1.0f;
  bool smoothShading = false;
  bool backFaceCulling = false;
};
