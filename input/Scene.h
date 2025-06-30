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
    if (depth > 10) {
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
    Vec3 lightColor = calculateDirectLight(intr);
    if (intr.material.type == MaterialType::DIFFUSE) {
      return intr.material.albedo * lightColor;
    }
    else if (intr.material.type == MaterialType::REFLECTIVE) {
      return calculateReflection(intr, depth) * lightColor;
    }
    else if (intr.material.type == MaterialType::REFRACTIVE) {
      return calculateRefraction(intr, depth);
    }
    return lightColor;
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

  // Perfect mirror reflection. As if the ray hits not the mirror but the surface it reflects.
  inline Vec3 calculateReflection(const IntersectionData& intr, int depth) const {
    Vec3 reflectDir = glm::reflect(intr.rayDir, intr.pN);

    Vec3 reflectionTraceColor = trace(Ray(intr.p + intr.pN * shadowBias, reflectDir), depth + 1);

    return reflectionTraceColor;
  }

  inline Vec3 calculateRefraction(const IntersectionData& intr, int depth) const {
      float n1 = 1.0f;
      float n2 = intr.material.ior;
      Vec3 surfaceNormal = intr.pNN;
      // Check if the incident ray leaves the transparent object.
      // The default is that it enters it.
      // If the direction of the ray is the same as the surface normal, then the ray must exit the object.
      if (glm::dot(intr.rayDir, surfaceNormal) > 0) {
        // For the calculations we need the normal to be in the opposite direction to the ray.
        surfaceNormal = -surfaceNormal;
        std::swap(n1, n2);
      }

      // Below, A is the incident angle and B is refraction angle

      // Cosine between the ray and surface normal
      float cosA = -glm::dot(intr.rayDir, surfaceNormal);
      float sinA = sqrtf(1.0f - cosA * cosA);

      if ((n1 / n2) * sinA > 1.0f) {
        // Total reflection.
        // Send only a reflection ray
        Vec3 reflectionDir = glm::reflect(intr.rayDir, surfaceNormal);

        Vec3 reflectionColor = trace(Ray(intr.p + surfaceNormal * shadowBias, reflectionDir), depth + 1);

        return reflectionColor;
      }
      else {
        // Snells law: n1 * sin(incident) = n2 * sin(refraction)
        float sinB = sinA * (n1 / n2);
        float cosB = sqrtf(1.0f - sinB * sinB);

        // This is the shadow that the refraction ray throws on the normal
        Vec3 A = cosB * -surfaceNormal;

        // This is the third side of the triangle between the incident ray
        // and the normal. This is going to be perpentdicular to the normal.
        Vec3 D = intr.rayDir + cosA * surfaceNormal;

        // We need a vector of length 1 so we can then compute the part of it which is
        // 'seen' from sin of angle B
        Vec3 C = glm::normalize(D);

        // The vector B is just part of the C vector
        Vec3 B = C * sinB;

        // The resulting refraction direction is the third side of the triangle formed by A and B
        Vec3 R = A + B;

        // We need to move the start point of the ray beyond the border
        Vec3 refractionRayOrigin = intr.p + (-surfaceNormal * shadowBias);
        Ray refractionRay(refractionRayOrigin, R);

        Vec3 refractionColor = trace(refractionRay, depth + 1);

        Vec3 reflectionDir = glm::reflect(intr.rayDir, surfaceNormal);

        Vec3 reflectionColor = trace(Ray(intr.p + surfaceNormal * shadowBias, reflectionDir), depth + 1);

        float fresnel = 0.5f * powf(1.0f - cosA, 5.0f);

        return glm::mix(refractionColor, reflectionColor, fresnel);
      }
  }

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  Vec3 backgroundColor;
  float shadowBias = 0.0555f;
};

#endif // !SCENE_H
