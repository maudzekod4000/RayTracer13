#ifndef SCENE_H
#define SCENE_H

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <random>
#include <algorithm>

#include "sampling/Triangle.h"
#include "sampling/IntersectionData.h"
#include "calc/TypeDefs.h"
#include "sampling/Material.h"
#include "sampling/Light.h"
#include "sampling/AABBTree.h"

class Scene {
public:
	explicit inline Scene(AABBTree&& t, std::vector<Light>&& l, const Vec& background):
		triangles(std::move(t)),
		lights(std::move(l)),
    backgroundColor(background)
	{}

	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	inline Vec trace(const Ray& ray) const {
    if (ray.type == RayType::CAMERA) {
      if (ray.bounceCount > cameraRaysMaxDepth) {
        return backgroundColor;
      }
    }
    else if (ray.type == RayType::GI) {
      if (ray.bounceCount > giRaysMaxDepth) {
        return backgroundColor;
      }
    }
		IntersectionData intr = triangles.intersectAABBTree(ray);

    return intr.intersection ?
      calculatePixelColor(intr) :
      backgroundColor;
	}

	inline Vec calculatePixelColor(const IntersectionData& intr) const {
    if (intr.material.type == MaterialType::DIFFUSE) {
      const Vec directLight = calculateDirectLight(intr);
      const Vec indirectLight = calculateDiffuse(intr);
      const Vec totalLight = XMVectorClamp(directLight + indirectLight, XMVectorZero(), XMVectorSplatOne());
      return intr.material.albedo * totalLight;
    }
    else if (intr.material.type == MaterialType::REFLECTIVE) {
      const Vec lightColor = calculateDirectLight(intr);
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

  inline Vec calculateDirectLight(const IntersectionData& intr) const {
    Vec finalColor = XMVectorZero();

    for (const Light& light : lights) {
      const Vec correctedHitPoint = intr.p + intr.pN * shadowBias;
      Vec lightDir = light.pos - correctedHitPoint;
      const float sphereRadius = XMVectorGetX(XMVector3Length(lightDir));
      lightDir = XMVector3Normalize(lightDir);
      const Ray shadowRay(correctedHitPoint, lightDir);

      IntersectionData shadowRayIntrs = triangles.intersectAABBTree(shadowRay);

      if (shadowRayIntrs.material.type != MaterialType::REFRACTIVE && shadowRayIntrs.t > sphereRadius) {
        float lightDirPnAngle = XMVectorGetX(XMVector3Dot(lightDir, intr.pN));
        const float cosineLaw = std::max(0.0f, lightDirPnAngle);
        const float sphereArea = 4.0 * M_PI * sphereRadius * sphereRadius;
        const float colorComponent = float(light.intensity) / sphereArea * cosineLaw;
        const Vec colorContribution = XMVectorSet(colorComponent, colorComponent, colorComponent, 0.0f);
        finalColor += colorContribution;
      }
    }

    return XMVectorClamp(finalColor, XMVectorZero(), XMVectorSplatOne());
  }

  // Perfect mirror reflection. As if the ray hits not the mirror but the surface it reflects.
  inline Vec calculateReflection(const Ray& ray, const Vec& n, const Vec& p) const {
    return trace(Ray(p + n * reflectionBias, XMVector3Reflect(ray.dir, n), ray.bounceCount + 1, ray.type));
  }

  // TODO: There is some shadow acne on the edges of the refraction.
  inline Vec calculateRefraction(const IntersectionData& intr) const {
      float n1 = 1.0f;
      float n2 = intr.material.ior;
      Vec surfaceNormal = intr.material.smoothShading ? intr.pN : intr.pNN;
      const Vec rayDir = intr.ray.dir;
      const bool inside = XMVectorGetX(XMVector3Dot(rayDir, surfaceNormal)) > 0;

      if (inside) {
        surfaceNormal = -surfaceNormal;
        std::swap(n1, n2);
      }

      const float iorRatio = n1 / n2;
      const float cosTheta = XMVectorGetX(XMVector3Dot(-rayDir, surfaceNormal));
      const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

      if (iorRatio * sinTheta > 1.0f) {
        return calculateReflection(intr.ray, surfaceNormal, intr.p);
      }

      const Vec R = XMVector3Refract(rayDir, surfaceNormal, iorRatio);
      const float shlikApprox = calculateReflectance(cosTheta, iorRatio);

      const Vec reflectionColor = calculateReflection(intr.ray, surfaceNormal, intr.p);

      const Vec refractionOrigin(intr.p + (-surfaceNormal * refractionBias));
      const Ray refractionRay(refractionOrigin, R, intr.ray.bounceCount + 1, intr.ray.type);

      const Vec refractionColor = trace(refractionRay);

      return XMVectorLerp(refractionColor, reflectionColor, shlikApprox);
  }

  inline Vec calculateDiffuse(const IntersectionData& intr) const {
    Vec diffReflColor = XMVectorZero();
    Vec n = intr.material.smoothShading ? intr.pN : intr.pNN;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i = 0; i < giRaysCount; i++) {
      Vec rightAxis = XMVector3Normalize(XMVector3Cross(intr.ray.dir, n));
      Vec upAxis = n;
      Vec forwardAxis = XMVector3Normalize(XMVector3Cross(rightAxis, upAxis));

      XMMATRIX localHitMatrix = XMMATRIX(
        rightAxis,     // Row 0
        forwardAxis,   // Row 1
        upAxis,        // Row 2
        XMVectorSet(0, 0, 0, 1) // Optional 4th row for padding if needed
      );

      // Generate random angle in the XY plane
      float randAngleInXY = M_PI * dist(rng);
      // Construct random vector in the XY plane
      Vec randVecXY = XMVectorSet(cos(randAngleInXY), sin(randAngleInXY), 0.0f, 0.0f);

      // Generate random angle in the XZ plane
      float randAngleINXZ = M_PI * 2 * dist(rng);
      XMMATRIX rotMatY = XMMatrixRotationY(randAngleINXZ);

      Vec randVecInXYRotated = XMVector3Transform(randVecXY, rotMatY);

      Vec diffReflRayDir = XMVector3Transform(randVecInXYRotated, localHitMatrix);
      Vec diffRayOrigin = intr.p + (n * reflectionBias);

      if (intr.ray.type == RayType::CAMERA) {
        // This is the first camera ray that hit the surface, now we initiate GI rays.
        Ray diffRay(diffRayOrigin, diffReflRayDir, 0, RayType::GI);
        diffReflColor = diffReflColor + trace(diffRay);
      }
      else if (intr.ray.type == RayType::GI) {
        // Continue shooting GI rays
        Ray diffRay(diffRayOrigin, diffReflRayDir, intr.ray.bounceCount + 1, RayType::GI);
        diffReflColor = diffReflColor + trace(diffRay);
      }

    }

    return diffReflColor / (float)giRaysCount;
  }

	AABBTree triangles;
	std::vector<Light> lights;
  Vec backgroundColor;
  const float shadowBias = 0.0055f;
  const float reflectionBias = 0.0001f;
  const float refractionBias = 0.0001f;
  const int giRaysCount = 1;
  const int giRaysMaxDepth = 2;
  const int cameraRaysMaxDepth = 3;

private:
  /// Use Schlick's approximation for reflectance.
  /// It tells us how much of the light is reflected.
  inline float calculateReflectance(float cosine, float etaiOverEtat) const {
    float r0 = (1.0f - etaiOverEtat) / (1.0f + etaiOverEtat);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::powf((1.0f - cosine), 5.0f);
  }
};

#endif // !SCENE_H
