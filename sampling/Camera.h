#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include "calc/TypeDefs.h"
#include "Ray.h"

struct Camera {
    Camera(const Vec3& pos, const Mat3& matrix, uint16_t sensorWidth,
        uint16_t sensorHeight, float focalDistance):
  pos(pos),
  transformationMatrix(Mat4(matrix)),
  sensorWidth(sensorWidth),
  sensorHeight(sensorHeight),
  aspectRatio(float(sensorWidth) / sensorHeight),
  focalDist(focalDistance),
  halfSensorW(sensorWidth / 2.0f),
  halfSensorH(sensorHeight / 2.0f)
{}

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
  inline Ray generateRay(float x, float y) const {
    /* Sample the pixel at its center */
    x += 0.5f;
    y += 0.5f;

    /* Transform to NDC: 0.0 - 1.0 */
    x /= halfSensorW;
    y /= halfSensorH;

    /* To Screen space: bottom left is [-1;-1] and top right is [1;1] and 0,0 is at the center of the screen */
    x = x - 1.0f;
    y = 1.0f - y;

    /* Consider the aspect ratio, because pixels might not be square and we added 0.5 to both components. */
    x *= aspectRatio;

    const Vec3 rayDir(x, y, focalDist);

    return Ray(pos, glm::normalize(applyTransformation(rayDir)), 0, maxRayBounces);
}

private:
  Vec3 pos;
  glm::mat4 transformationMatrix;
  uint16_t sensorWidth;
  uint16_t sensorHeight;
  float aspectRatio;
  float focalDist;
  float halfSensorW;
  float halfSensorH;
  const int maxRayBounces = 16;
};

#endif // !CAMERA_H
