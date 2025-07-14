#pragma once 

#include "sampling/Vertex.h"
#include "sampling/Material.h"
#include "calc/TypeDefs.h"
#include "sampling/Ray.h"
#include "sampling/IntersectionData.h"
#include "calc/MathUtils.h"

struct Triangle {
	inline Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Material& m):
		a(a),
		b(b),
		c(c),
		n(XMVector3Normalize(XMVector3Cross(b.pos - a.pos, c.pos - a.pos))),
		material(m)
	{}

  inline Triangle() = default;

	/// Calculates the triangle's area.
	inline float area() const {
		return MathUtils::area(a.pos, b.pos, c.pos);
	}

	/// Checks for intersection between the triangle and the ray.
	/// @param ray Currently traced ray.
	/// @param intersectionData Holds data from previous intersection and could be updated with a potential new intersection data. Out param.
	/// @return True, if there is an intersection.
	inline bool intersect(const Ray& ray, IntersectionData& intersectionData) const {
		// The projection of the ray direction onto the normal of the triangle
		// is the length from the ray origin to the traingle plane.
    XMVECTOR originToTriangleVertexA = a.pos - ray.origin;
    float angleBetweenTriangleAndRay = XMVectorGetX(XMVector3Dot(originToTriangleVertexA, n));
    float angleBetweenRayDirAndN = XMVectorGetX(XMVector3Dot(ray.dir, n));
    const float t = angleBetweenTriangleAndRay / angleBetweenRayDirAndN;

		if (t > intersectionData.t || t <= 0.0f) {
			return false;
		}

		const Vec p = ray.origin + t * ray.dir;

		// If the intersection point is just a bit to the side of the triangle we don't care.
		// We avoid artifacts when two triangle sides are next to each other.
		// We want them to blend a bit to avoid jagged edges.
		constexpr float e = -0.000001f;

		if (XMVectorGetX(XMVector3Dot(n, XMVector3Cross(b.pos - a.pos, p - a.pos))) <= e ||
			XMVectorGetX(XMVector3Dot(n, XMVector3Cross(c.pos - b.pos, p - b.pos))) <= e ||
			XMVectorGetX(XMVector3Dot(n, XMVector3Cross(a.pos - c.pos, p - c.pos))) <= e) {
			return false;
		}

		intersectionData.t = t;
		intersectionData.p = p;
		intersectionData.intersection = true;
    intersectionData.material = material;
    intersectionData.ray = ray;
    intersectionData.pNN = n;

    float areaM = XMVectorGetX(XMVector3Length(XMVector3Cross(p - a.pos, c.pos - a.pos))) * 0.5f;
    float areaN = XMVectorGetX(XMVector3Length(XMVector3Cross(b.pos - a.pos, p - a.pos))) * 0.5f;
    float areaTri = XMVectorGetX(XMVector3Length(XMVector3Cross(b.pos - a.pos, c.pos - a.pos))) * 0.5f;

    float u = areaM / areaTri;
    float v = areaN / areaTri;
    float w = 1 - u - v;

    intersectionData.pN = b.normal * u + c.normal * v + a.normal * w;

		return true;
	}

	Vertex a;
	Vertex b;
	Vertex c;
	Vec n;
	Material material;
};
