#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "TypeDefs.h"

#include "Vertex.h"

struct Ray;
struct IntersectionData;

struct Triangle {
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

	/// Calculates the triangle's area.
	float area() const;

	/// Checks for intersection between the triangle and the ray.
	/// @param ray Currently traced ray.
	/// @param intersectionData Holds data from previous intersection and could be updated with a potential new intersection data. Out param.
	/// @return True, if there is an intersection.
	bool intersect(const Ray& ray, IntersectionData& intersectionData) const;
private:
	Vertex a;
	Vertex b;
	Vertex c;
	Vec3 n;
	int objIdx;
};

#endif // !TRIANGLE_H
