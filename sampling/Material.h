#pragma once

#include "calc/TypeDefs.h"

enum class MaterialType {
  DIFFUSE,
  REFLECTIVE,
  REFRACTIVE
};

// TODO: In our ray tracer, materials will be read-only so you can design it as immutable object.
struct Material {
  Vec3 albedo = Vec3(1.0, 0.0f, 0.0f); ///< Natural color of the object/material.
  MaterialType type = MaterialType::DIFFUSE;
  float ior = 1.0f;
  bool smoothShading = false;
};
