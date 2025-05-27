#include "Triangle.h"

#include "calc/TypeDefs.h"
#include "sampling/Ray.h"
#include "sampling/IntersectionData.h"

// Calculates the area of the triangle defined by the three points.
static float calculateArea(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return glm::length(glm::cross(b - a, c - a)) / 2.0f;
}

static Vec3 calculateNormal(const Vertex& a, const Vertex& b, const Vertex& c)
{
	return glm::normalize(glm::cross(b.pos() - a.pos(), c.pos() - a.pos()));
}

Triangle::Triangle(Vertex&& a, Vertex&& b, Vertex&& c):
	a(std::move(a)),
	b(std::move(b)),
	c(std::move(c))
{}

float Triangle::area() const
{
    return calculateArea(a.pos(), b.pos(), c.pos());
}

bool Triangle::intersect(const Ray& ray, IntersectionData& intersectionData) const
{
	const Vec3 n = calculateNormal(a, b, c);
	const float rayProj = glm::dot(ray.dir, n);
	// The projection of the ray direction onto the normal of the triangle
	// is the length from the ray origin to the traingle plane.
	const float t = glm::dot(this->a.pos() - ray.origin, n) / rayProj;

	if (t > intersectionData.t || t <= 0.0f) {
		return false;
	}

	const Vec3 p = ray.origin + t * ray.dir;

	// If the intersection point is just a bit to the right of the triangle side we don't care.
	// We avoid artifacts when two triangle sides are next to each other.
	// We want them to blend a bit to avoid jagged edges.
	constexpr float e = -0.000001;

	if (dot(n, cross(b.pos() - a.pos(), p - a.pos())) <= e ||
		dot(n, cross(c.pos() - b.pos(), p - b.pos())) <= e ||
		dot(n, cross(a.pos() - c.pos(), p - c.pos())) <= e) {
		return false;
	}

	intersectionData.t = t;
	intersectionData.p = p;
	intersectionData.intersection = true;

	return true;
}
