#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include "TypeDefs.h"

struct Camera {
  inline Camera(const Vec3& pos, const Mat3& matrix): pos(pos), transformationMatrix(Mat4(matrix)) {}

  /*
  * positive degrees are for left pan. Rotating around the Y vector because we are Y-up.
  */
  inline void pan(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(0.0f, 1.0f, 0.0f));
  }

  /*
  * Positive values are for upward lift.
  * If there is an object directly in front of the camera it should go down, when we tilt the camera up.
  */
  inline void tilt(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(1.0f, 0.0f, 0.0f));
  }

  /*
  * Positive values are for left roll.
  * If there is an object directly in front of the camera it should seem that it has been rotated clockwise.
  */
  inline void roll(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(0.0f, 0.0f, 1.0f));
  }

  // Negative values are for forward movement.
  inline void dolly(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(0.0f, 0.0f, delta));
  }

  // Positive values are for right truck
  inline void truck(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(delta, 0.0f, 0.0f));
  }

  // Positive values are for up movement.
  inline void pedestal(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(0.0f, delta, 0.0f));
  }

  inline Vec3 applyTransformation(const Vec3& vec) const {
    return transformationMatrix * glm::vec4(vec, 1.0f);
  }

  Vec3 pos;
  glm::mat4 transformationMatrix;
};

#endif // !CAMERA_H
