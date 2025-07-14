#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include "calc/TypeDefs.h"
#include "Ray.h"

using namespace DirectX;

struct Camera {
    Camera(const Vec& pos, const Mat& matrix, uint16_t sensorWidth,
        uint16_t sensorHeight, float focalDistance):
  pos(pos),
  transformationMatrix(matrix),
  sensorWidth(sensorWidth),
  sensorHeight(sensorHeight),
  aspectRatio(float(sensorWidth) / sensorHeight),
  focalDist(focalDistance),
  halfSensorW(sensorWidth * 0.5f),
  halfSensorH(sensorHeight * 0.5f)
{}

  /// positive degrees are for left pan. Rotating around the Y vector because we are Y-up.
  inline void pan(float degrees) {
    float radians = XMConvertToRadians(degrees);
    XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX rotation = XMMatrixRotationAxis(axis, radians);
    transformationMatrix = XMMatrixMultiply(rotation, transformationMatrix);
  }

  /// Positive values are for upward lift.
  /// If there is an object directly in front of the camera it should go down, when we tilt the camera up.
  inline void tilt(float degrees) {
    float radians = XMConvertToRadians(degrees);
    XMVECTOR axis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMMATRIX rotation = XMMatrixRotationAxis(axis, radians);
    transformationMatrix = XMMatrixMultiply(rotation, transformationMatrix);
  }

  /// Positive values are for left roll.
  /// If there is an object directly in front of the camera it should seem that it has been rotated clockwise.
  inline void roll(float degrees) {
    float radians = XMConvertToRadians(degrees);
    XMVECTOR axis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMMATRIX rotation = XMMatrixRotationAxis(axis, radians);
    transformationMatrix = XMMatrixMultiply(rotation, transformationMatrix);
  }

  /// Negative values are for forward movement.
  inline void dolly(float delta) {
    XMMATRIX t = XMMatrixTranslation(0.f, 0.f, delta);
    transformationMatrix = XMMatrixMultiply(t, transformationMatrix);
  }

  /// Positive values are for right truck
  inline void truck(float delta) {
    XMMATRIX t = XMMatrixTranslation(delta, 0.f, 0.f);
    transformationMatrix = XMMatrixMultiply(t, transformationMatrix);
  }

  /// Positive values are for up movement.
  inline void pedestal(float delta) {
    XMMATRIX t = XMMatrixTranslation(0.f, delta, 0.f);
    transformationMatrix = XMMatrixMultiply(t, transformationMatrix);
  }

  inline Vec applyTransformation(const Vec& vec) const {
    return XMVector3Transform(vec, transformationMatrix);
  }

  /// Generates a ray towards a pixel on the render plane.
  /// @param x Row index of the pixel.
  /// @param y Column index of the pixel.
  /// @return A Ray from the camera pinhole to the pixel's center.
  inline Ray generateRay(float x, float y) const {
    /* Transform to NDC: 0.0 - 1.0 */
    x /= halfSensorW;
    y /= halfSensorH;

    /* To Screen space: bottom left is [-1;-1] and top right is [1;1] and 0,0 is at the center of the screen */
    x = x - 1.0f;
    y = 1.0f - y;

    /* Consider the aspect ratio, because pixels might not be square and we added 0.5 to both components. */
    x *= aspectRatio;

    const Vec rayDir(x, y, focalDist);

    return Ray(pos, XMVector3Normalize(applyTransformation(rayDir)));
}

private:
  Vec pos;
  Mat transformationMatrix;
  uint16_t sensorWidth;
  uint16_t sensorHeight;
  float aspectRatio;
  float focalDist;
  float halfSensorW;
  float halfSensorH;
  const int maxRayBounces = 16;
};

#endif // !CAMERA_H
