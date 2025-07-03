#ifndef SCENE_H
#define SCENE_H

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

#include "sampling/Triangle.h"
#include "sampling/IntersectionData.h"
#include "calc/TypeDefs.h"
#include "sampling/Material.h"
#include "sampling/Light.h"

class Scene {
public:
	inline Scene(std::vector<Triangle>&& t, std::vector<Light>&& l, const Vec3& background):
		triangles(std::move(t)),
		lights(std::move(l)),
    backgroundColor(background)
	{}

	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	inline Vec3 trace(const Ray& ray, int depth = 0) const {
    if (depth > maxDepth) {
      return backgroundColor;
    }
		IntersectionData intr{};

		for (auto& triangle : triangles) {
			triangle.intersect(ray, intr);
		}

    return intr.intersection ?
      calculatePixelColor(intr, depth) :
      backgroundColor;
	}

	inline Vec3 calculatePixelColor(const IntersectionData& intr, int depth) const {
    if (intr.material.type == MaterialType::DIFFUSE) {
      const Vec3 lightColor = calculateDirectLight(intr);
      return intr.material.albedo * lightColor;
    }
    else if (intr.material.type == MaterialType::REFLECTIVE) {
      const Vec3 lightColor = calculateDirectLight(intr);
      return calculateReflection(intr.rayDir, intr.pN, intr.p, depth) * lightColor;
    }
    else if (intr.material.type == MaterialType::REFRACTIVE) {
      return calculateRefraction(intr, depth);
    }
    else if (intr.material.type == MaterialType::CONSTANT) {
      return intr.material.albedo;
    }
    else {
      return intr.material.albedo;
    }
	}

  inline Vec3 calculateDirectLight(const IntersectionData& intr) const {
    Vec3 finalColor(0);

    for (const Light& light : lights) {
      const Vec3 correctedHitPoint = intr.p + intr.pN * shadowBias;
      Vec3 lightDir = light.pos - correctedHitPoint;
      const float sphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);
      const Ray shadowRay(correctedHitPoint, lightDir);

      IntersectionData shadowRayIntrs{};

      for (auto& triangle : triangles) {
        if (triangle.material.type != MaterialType::REFRACTIVE) {
          if (triangle.intersect(shadowRay, shadowRayIntrs)) {
            break;
          }
        }
      }

      if (shadowRayIntrs.t > sphereRadius) {
        const float cosineLaw = glm::max(0.0f, glm::dot(lightDir, intr.pN));
        const float sphereArea = 4.0 * M_PI * sphereRadius * sphereRadius;
        const Vec3 colorContribution = Vec3(float(light.intensity) / sphereArea * cosineLaw);
        finalColor += colorContribution;
      }
    }

    return glm::clamp(finalColor, Vec3(0.0f), Vec3(1.0f));
  }

  // Perfect mirror reflection. As if the ray hits not the mirror but the surface it reflects.
  inline Vec3 calculateReflection(const Vec3& rayDir, const Vec3& n, const Vec3& p, int depth) const {
    return trace(Ray(p + n * reflectionBias, glm::reflect(rayDir, n)), depth + 1);
  }

  // TODO: There is some shadow acne on the edges of the refraction.
  inline Vec3 calculateRefraction(const IntersectionData& intr, int depth) const {
      float n1 = 1.0f;
      float n2 = intr.material.ior;
      Vec3 surfaceNormal = intr.material.smoothShading ? intr.pN : intr.pNN;
      const Vec3 rayDir = glm::normalize(intr.rayDir);
      const bool inside = glm::dot(rayDir, surfaceNormal) > 0;

      if (inside) {
        surfaceNormal = -surfaceNormal;
        std::swap(n1, n2);
      }

      const float iorRatio = n1 / n2;
      const float cosTheta = glm::dot(-rayDir, surfaceNormal);
      const float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

      if (iorRatio * sinTheta > 1.0f) {
        return calculateReflection(rayDir, surfaceNormal, intr.p, depth);
      }

      const Vec3 R = glm::refract(rayDir, surfaceNormal, iorRatio);
      const float shlikApprox = calculateReflectance(cosTheta, iorRatio);

      const Vec3 reflectionColor = calculateReflection(rayDir, surfaceNormal, intr.p, depth);

      const Vec3 refractionOrigin(intr.p + (-surfaceNormal * refractionBias));
      const Ray refractionRay(refractionOrigin, R);

      const Vec3 refractionColor = trace(refractionRay, depth + 1);

      return glm::mix(refractionColor, reflectionColor, shlikApprox);
  }

  /// Use Schlick's approximation for reflectance.
  /// It tells us how much of the light is reflected.
  inline float calculateReflectance(float cosine, float etaiOverEtat) const {
    float r0 = (1.0f - etaiOverEtat) / (1.0f + etaiOverEtat);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow((1.0f - cosine), 5.0f);
  }

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  Vec3 backgroundColor;
  float shadowBias = 0.0055f;
  float reflectionBias = 0.0001f;
  float refractionBias = 0.0001f;
  const int maxDepth = 16;
};

#endif // !SCENE_H
