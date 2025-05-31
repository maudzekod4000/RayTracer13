#include "Triangle.h"

#include "calc/TypeDefs.h"
#include "sampling/Ray.h"
#include "sampling/IntersectionData.h"
#include "calc/MathUtils.h"

Triangle::Triangle(Vertex&& a, Vertex&& b, Vertex&& c):
	a(std::move(a)),
	b(std::move(b)),
	c(std::move(c))
{}

float Triangle::area() const
{
    return MathUtils::area(a.pos, b.pos, c.pos);
}

bool Triangle::intersect(const Ray& ray, IntersectionData& intersectionData) const
{
	const Vec3 n = MathUtils::normal(a.pos, b.pos, c.pos);
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
