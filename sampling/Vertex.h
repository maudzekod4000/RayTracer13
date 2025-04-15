#ifndef VERTEX_H
#define VERTEX_H

#include "TypeDefs.h"

struct Vertex {
public:
	Vertex(const Vec3& pos) : position(pos) {}

	const Vec3& pos() const { return position; }
private:
  Vec3 position;
  Vec3 smoothNormal{};
};

#endif // !VERTEX_H
