#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include "TypeDefs.h"
#include "Ray.h"

struct Camera {
    Camera(const Vec3& pos, const Mat3& matrix, uint16_t sensorWidth,
        uint16_t sensorHeight, float focalDistance);

  /// positive degrees are for left pan. Rotating around the Y vector because we are Y-up.
  inline void pan(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(0.0f, 1.0f, 0.0f));
  }

  /// Positive values are for upward lift.
  /// If there is an object directly in front of the camera it should go down, when we tilt the camera up.
  inline void tilt(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(1.0f, 0.0f, 0.0f));
  }

  /// Positive values are for left roll.
  /// If there is an object directly in front of the camera it should seem that it has been rotated clockwise.
  inline void roll(float degrees) {
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(degrees), Vec3(0.0f, 0.0f, 1.0f));
  }

  /// Negative values are for forward movement.
  inline void dolly(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(0.0f, 0.0f, delta));
  }

  /// Positive values are for right truck
  inline void truck(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(delta, 0.0f, 0.0f));
  }

  /// Positive values are for up movement.
  inline void pedestal(float delta) {
    transformationMatrix = glm::translate(transformationMatrix, Vec3(0.0f, delta, 0.0f));
  }

  inline Vec3 applyTransformation(const Vec3& vec) const {
    return transformationMatrix * glm::vec4(vec, 1.0f);
  }

  /// Generates a ray towards a pixel on the render plane.
  /// @param x Row index of the pixel.
  /// @param y Column index of the pixel.
  /// @return A Ray from the camera pinhole to the pixel's center.
  Ray generateRay(float x, float y) const;

private:
  Vec3 pos;
  glm::mat4 transformationMatrix;
  uint16_t sensorWidth;
  uint16_t sensorHeight;
  float aspectRatio;
  float focalDist;
  float halfSensorW;
  float halfSensorH;
};

#endif // !CAMERA_H
