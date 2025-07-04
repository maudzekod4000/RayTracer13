#ifndef SCENE_H
#define SCENE_H

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <random>

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
    if (ray.bounceCount > ray.maxBounces) {
      return backgroundColor;
    }
		IntersectionData intr{};

		for (auto& triangle : triangles) {
			triangle.intersect(ray, intr);
		}

    return intr.intersection ?
      calculatePixelColor(intr) :
      backgroundColor;
	}

	inline Vec3 calculatePixelColor(const IntersectionData& intr) const {
    if (intr.material.type == MaterialType::DIFFUSE) {
      const Vec3 directLight = calculateDirectLight(intr);
      const Vec3 indirectLight = calculateDiffuse(intr);
      return intr.material.albedo * (directLight + indirectLight);
    }
    else if (intr.material.type == MaterialType::REFLECTIVE) {
      const Vec3 lightColor = calculateDirectLight(intr);
      return calculateReflection(intr.ray, intr.pN, intr.p) * lightColor;
    }
    else if (intr.material.type == MaterialType::REFRACTIVE) {
      return calculateRefraction(intr);
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
  inline Vec3 calculateReflection(const Ray& ray, const Vec3& n, const Vec3& p) const {
    return trace(Ray(p + n * reflectionBias, glm::reflect(ray.dir, n)));
  }

  // TODO: There is some shadow acne on the edges of the refraction.
  inline Vec3 calculateRefraction(const IntersectionData& intr) const {
      float n1 = 1.0f;
      float n2 = intr.material.ior;
      Vec3 surfaceNormal = intr.material.smoothShading ? intr.pN : intr.pNN;
      const Vec3 rayDir = intr.ray.dir;
      const bool inside = glm::dot(rayDir, surfaceNormal) > 0;

      if (inside) {
        surfaceNormal = -surfaceNormal;
        std::swap(n1, n2);
      }

      const float iorRatio = n1 / n2;
      const float cosTheta = glm::dot(-rayDir, surfaceNormal);
      const float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

      if (iorRatio * sinTheta > 1.0f) {
        return calculateReflection(intr.ray, surfaceNormal, intr.p);
      }

      const Vec3 R = glm::refract(rayDir, surfaceNormal, iorRatio);
      const float shlikApprox = calculateReflectance(cosTheta, iorRatio);

      const Vec3 reflectionColor = calculateReflection(intr.ray, surfaceNormal, intr.p);

      const Vec3 refractionOrigin(intr.p + (-surfaceNormal * refractionBias));
      const Ray refractionRay(refractionOrigin, R);

      const Vec3 refractionColor = trace(refractionRay);

      return glm::mix(refractionColor, reflectionColor, shlikApprox);
  }

  inline Vec3 calculateDiffuse(const IntersectionData& intr) const {
    Vec3 diffReflColor(0.0f);
    Vec3 n = intr.material.smoothShading ? intr.pN : intr.pNN;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i = 0; i < giRaysCount; i++) {
      Vec3 rightAxis = glm::normalize(glm::cross(intr.ray.dir, n));
      Vec3 upAxis = n;
      Vec3 forwardAxis = glm::normalize(glm::cross(rightAxis, upAxis));

      glm::mat3x3 localHitMatrix(1.0f);
      localHitMatrix[0] = rightAxis;
      localHitMatrix[1] = forwardAxis;
      localHitMatrix[2] = upAxis;

      // Generate random angle in the XY plane
      float randAngleInXY = M_PI * dist(rng);
      // Construct random vector in the XY plane
      Vec3 randVecXY = Vec3(cos(randAngleInXY), sin(randAngleInXY), 0);

      // Generate random angle in the XZ plane
      float randAngleINXZ = M_PI * 2 * dist(rng);
      glm::mat3x3 rotMatY(1.0f);
      rotMatY[0][0] = cos(randAngleINXZ);
      rotMatY[2][0] = -sin(randAngleINXZ);
      rotMatY[0][2] = sin(randAngleINXZ);
      rotMatY[2][2] = cos(randAngleINXZ);

      Vec3 randVecInXYRotated = randVecXY * rotMatY;

      Vec3 diffReflRayDir = randVecInXYRotated * localHitMatrix;
      Vec3 diffRayOrigin = intr.p + (n * reflectionBias);
      Ray diffRay(diffRayOrigin, diffReflRayDir);
      // Hm very interesting problem with the ray bounces.
      // Maybe if we have two types of rays: original and GI.
      // then we can say:
      // ok, is that a GI ray? lets continue it by adding to its depth.
      // is it a original ray? start a new GI ray from it with depth 0.
      diffReflColor = diffReflColor + trace(diffRay);
    }

    return diffReflColor / (float)giRaysCount;
  }

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
  Vec3 backgroundColor;
  const float shadowBias = 0.0055f;
  const float reflectionBias = 0.0001f;
  const float refractionBias = 0.0001f;
  const int giRaysCount = 8;
  const int giRaysMaxDepth = 2;

private:
  /// Use Schlick's approximation for reflectance.
  /// It tells us how much of the light is reflected.
  inline float calculateReflectance(float cosine, float etaiOverEtat) const {
    float r0 = (1.0f - etaiOverEtat) / (1.0f + etaiOverEtat);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow((1.0f - cosine), 5.0f);
  }
};

#endif // !SCENE_H
