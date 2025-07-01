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
    Vec3 lightColor = calculateDirectLight(intr);
    if (intr.material.type == MaterialType::DIFFUSE) {
      return intr.material.albedo * lightColor;
    }
    else if (intr.material.type == MaterialType::REFLECTIVE) {
      return calculateReflection(intr.rayDir, intr.pN, intr.p, depth) * lightColor;
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
    float projOfRayOnNegNormal = glm::dot(rayDir, -n);
    float doubleProjNeg = -(2 * projOfRayOnNegNormal);
    Vec3 scaledNormal = n * doubleProjNeg;
    Vec3 reflectDir = glm::reflect(rayDir, n);
    //Vec3 reflectDir = rayDir - scaledNormal;

    Vec3 reflectionTraceColor = trace(Ray(p + n * reflectionBias, reflectDir), depth + 1);

    return reflectionTraceColor;
  }

  // TODO: There is some shadow acne on the edges of the refraction.
  inline Vec3 calculateRefraction(const IntersectionData& intr, int depth) const {
      float n1 = 1.0f;
      float n2 = intr.material.ior;
      Vec3 surfaceNormal = intr.material.smoothShading ? intr.pN : intr.pNN;
      Vec3 rayDir = glm::normalize(intr.rayDir);
      bool inside = glm::dot(rayDir, surfaceNormal) > 0;

      if (inside) {
        surfaceNormal = -surfaceNormal;
        std::swap(n1, n2);
      }

      float etaiOverEtat = n1 / n2;

      float cosTheta = std::fminf(glm::dot(-rayDir, surfaceNormal), 1.0f);
      Vec3 rOutPerp = etaiOverEtat * (rayDir + cosTheta * surfaceNormal);
      Vec3 rOutParallel = -glm::sqrt(glm::abs(1.0f - glm::length(rOutPerp) * glm::length(rOutPerp))) * surfaceNormal;
      Vec3 R = glm::normalize(rOutPerp + rOutParallel);

      float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

      if (etaiOverEtat * sinTheta > 1.0f) {
        return calculateReflection(rayDir, surfaceNormal, intr.p, depth);
      }

      float shlikApprox = reflectance(cosTheta, etaiOverEtat);

      Vec3 reflectionColor = calculateReflection(rayDir, surfaceNormal, intr.p, depth);

      Vec3 refractionOrigin(intr.p + (-surfaceNormal * refractionBias));
      Ray refractionRay(refractionOrigin, R);

      Vec3 refractionColor = trace(refractionRay, depth + 1);

      return glm::mix(refractionColor, reflectionColor, shlikApprox);
  }

  static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  Vec3 backgroundColor;
  float shadowBias = 0.0155f;
  float reflectionBias = 0.0001f;
  float refractionBias = 0.0001f;
  const int maxDepth = 20;
};

#endif // !SCENE_H
