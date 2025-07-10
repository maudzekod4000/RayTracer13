#pragma once

#include "calc/TypeDefs.h"

enum class MaterialType {
  DIFFUSE,
  REFLECTIVE,
  REFRACTIVE,
  CONSTANT
};

struct Material {
  Vec albedo = Vec(1.0, 0.0f, 0.0f); ///< Natural color of the object/material.
  MaterialType type = MaterialType::DIFFUSE;
  float ior = 1.0f;
  bool smoothShading = false;
  bool backFaceCulling = false;
};
