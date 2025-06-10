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
	inline Scene(std::vector<Triangle>&& t, std::vector<Light>&& l):
		triangles(std::move(t)),
		lights(std::move(l))
	{}

	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	inline Vec3 trace(const Ray& ray) const {
		IntersectionData intersectionData{};

		for (auto& triangle : triangles) {
			triangle.intersect(ray, intersectionData);
		}

		return calculatePixelColor(intersectionData);
	}

	inline Vec3 calculatePixelColor(const IntersectionData& intr) const {
		return calculateDirectLight(intr);
	}

  inline Vec3 calculateDirectLight(const IntersectionData& intr) const {
    Vec3 finalColor(0);

    for (const Light& light : lights) {
      const Vec3 correctedHitPoint = intr.p + intr.pN * shadowBias;
      Vec3 lightDir = light.pos - correctedHitPoint;
      const float sphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);

      const float cosineLaw = glm::max(0.0f, glm::dot(lightDir, intr.pN));
      const float sphereArea = 4.0 * M_PI * sphereRadius * sphereRadius;
      const Ray shadowRay(correctedHitPoint, lightDir);

      IntersectionData shadowRayIntrs{};

      for (auto& triangle : triangles) {
        if (triangle.intersect(shadowRay, shadowRayIntrs)) {
          break;
        }
      }

      if (shadowRayIntrs.intersection) {
        const Vec3 colorContribution = intr.material.albedo * sphereArea * cosineLaw / (1.0f / light.intensity);
        finalColor += colorContribution;
      }
    }

    return finalColor;
  }

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  float shadowBias = 0.0001f;
};

#endif // !SCENE_H
