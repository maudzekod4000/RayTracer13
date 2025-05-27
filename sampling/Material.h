#ifndef MATERIAL_H
#define MATERIAL_H

#include "calc/TypeDefs.h"

enum class MaterialType {
  DIFFUSE,
  REFLECTIVE,
  REFRACTIVE,
  NONE // TODO: The default should be diffuse, why do we need None?
};

// TODO: In our ray tracer, materials will be read-only.
struct Material final {
  Vec3 albedo; ///< Natural color of the object/material.
  MaterialType type = MaterialType::NONE;
  float ior = 1.0f;
  bool smoothShading = false;
};

#endif // !MATERIAL_H
