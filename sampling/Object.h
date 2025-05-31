#ifndef OBJECT_H
#define OBJECT_H

#include "sampling/Material.h"

/// An object in the scene. Usually associated with a set of triangles and holds common information about the latter.
/// It is, essentially, a mesh.
/// TODO: Rename this to Mesh, if it makes sense later on in development.
class Object {
public:
	Object(const Material&) noexcept;

	const Material& getMaterial() const { return material; }
private:
	Material material;
};

// TODO: Ahh...it's good to have small objects, but it is better not to go to main memory so often...

#endif // !OBJECT_H
