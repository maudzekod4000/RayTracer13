#include "Triangle.h"

#include "Ray.h"
#include "IntersectionData.h"

// Calculates the area of the triangle defined by the three point arguments.
static float calculateArea(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return glm::length(glm::cross(b - a, c - a)) / 2.0f;
}

Triangle::Triangle(const Vertex& a, const Vertex& b, const Vertex& c):
    a(a), b(b), c(c),
    n(glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos))),
    objIdx(-1)
{}

float Triangle::area() const
{
    return calculateArea(a.pos, b.pos, c.pos);
}

// TODO: Maybe this should return intersectiondata?
bool Triangle::intersect(const Ray& ray, IntersectionData& intersectionData) const
{
	const float rayProj = glm::dot(ray.dir, n);
	// The projection of the ray direction onto the normal of the triangle
	// is the length from the ray origin to the traingle plane.
	const float t = glm::dot(this->a.pos - ray.origin, n) / rayProj;

	if (t > intersectionData.t || t <= 0) {
		return false;
	}

	Vec3 p = ray.origin + t * ray.dir;

	// If the intersection point is just a bit to the right of the triangle side we don't care.
	// We avoid artifacts when two triangle sides are next to each other.
	// We want them to blend a bit to avoid jagged edges.
	const float e = -0.000001;

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
