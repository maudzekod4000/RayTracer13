#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "TypeDefs.h"

#include "Vertex.h"

struct Ray;
struct IntersectionData;

struct Triangle {
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

	Vertex a;
	Vertex b;
	Vertex c;
	Vec3 n;
	int objIdx;

	float area() const;

	bool intersect(const Ray& ray, IntersectionData& intersectionData) const;
};

#endif // !TRIANGLE_H
