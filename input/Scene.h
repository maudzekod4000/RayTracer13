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

	inline Vec3 trace(const Ray& ray) const {
		IntersectionData intersectionData{};

		for (auto& triangle : triangles) {
			triangle.intersect(ray, intersectionData);
		}

    return intersectionData.intersection ?
      calculatePixelColor(intersectionData) :
      backgroundColor;
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
      const Ray shadowRay(correctedHitPoint, lightDir);

      IntersectionData shadowRayIntrs{};

      for (auto& triangle : triangles) {
        if (triangle.intersect(shadowRay, shadowRayIntrs)) {
          break;
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

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  Vec3 backgroundColor;
  float shadowBias = 0.0055f;
};

#endif // !SCENE_H
