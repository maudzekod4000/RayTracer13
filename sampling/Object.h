#ifndef OBJECT_H
#define OBJECT_H

#include "sampling/Material.h"

class Object {
public:
	Object(const Material&) noexcept;
private:
	Material material;
};

#endif // !OBJECT_H
