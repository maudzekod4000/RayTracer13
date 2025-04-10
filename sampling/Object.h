#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "Triangle.h"
//#include "Material.h"

class Object{
public:
  Object(std::vector<Triangle>&&) noexcept;
private:

  std::vector<Triangle> triangles;
  //Material mat;
};

#endif // !OBJECT_H
