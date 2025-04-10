#include "Object.h"

Object::Object(std::vector<Triangle>&& t) noexcept:
	triangles(std::move(t))
{
}
