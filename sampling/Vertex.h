#ifndef VERTEX_H
#define VERTEX_H

#include "calc/TypeDefs.h"

struct Vertex final {
public:
	Vertex(const Vec3& pos) : position(pos) {}

	const Vec3& pos() const { return position; }
private:
  Vec3 position;
};

#endif // !VERTEX_H
