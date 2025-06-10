#pragma once 

#include "sampling/Vertex.h"
#include "sampling/Material.h"
#include "calc/TypeDefs.h"
#include "sampling/Ray.h"
#include "sampling/IntersectionData.h"
#include "calc/MathUtils.h"

struct Triangle {
	inline Triangle(Vertex&& a, Vertex&& b, Vertex&& c, Material&& m):
		a(std::move(a)),
		b(std::move(b)),
		c(std::move(c)),
		n(MathUtils::normal(a.pos, b.pos, c.pos)),
		material(std::move(m))
	{}

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
		const float t = glm::dot(this->a.pos - ray.origin, n) / glm::dot(ray.dir, n);

		if (t > intersectionData.t || t <= 0.0f) {
			return false;
		}

		const Vec3 p = ray.origin + t * ray.dir;

		// If the intersection point is just a bit to the side of the triangle we don't care.
		// We avoid artifacts when two triangle sides are next to each other.
		// We want them to blend a bit to avoid jagged edges.
		constexpr float e = -0.000001f;

		if (dot(n, cross(b.pos - a.pos, p - a.pos)) <= e ||
			dot(n, cross(c.pos - b.pos, p - b.pos)) <= e ||
			dot(n, cross(a.pos - c.pos, p - c.pos)) <= e) {
			return false;
		}

		intersectionData.t = t;
		intersectionData.p = p;
		intersectionData.intersection = true;

		return true;
	}
private:
	Vertex a;
	Vertex b;
	Vertex c;
	Vec3 n;
public:
	Material material;
};
