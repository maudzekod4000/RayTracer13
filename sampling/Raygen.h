#ifndef RAYGEN_H
#define RAYGEN_H

#include <cstdint>

#include "Camera.h"
#include "Ray.h"

struct Raygen {
  inline Raygen(int32_t w, int32_t h, const Camera& camera, float focalDist):
    width(w), height(h), aspectRatio(static_cast<float>(w) / h), camera(camera), focalDist(focalDist) {}

  /// Creates a ray from camera position to an image plane.
  /// x, y are in raster coordinates, meaning, for a 640x480 image they will be from 0 up to 640 for x and
  /// 0 to 480 for y.
  inline Ray gen(float x, float y) {
    /* Sample the pixel at its center */
    x += 0.5f;
    y += 0.5f;

    /* Transform to NDC: 0.0 - 1.0 */
    x /= width;
    y /= height;

    /* To Screen space: bottom left is [-1;-1] and top right is [1;1] and 0,0 is at the center of the screen */
    x = (x * 2) - 1;
    y = 1 - (y * 2);

    /* Consider the aspect ratio, because pixels might not be square and we added 0.5 to both components. */
    x *= aspectRatio;

    const Vec3 rayDir(x, y, focalDist);

    return Ray(camera.pos, glm::normalize(camera.applyTransformation(rayDir)));
  }

private:
  int32_t width;
  int32_t height;
  float aspectRatio;
  const Camera& camera;
  float focalDist;
};

#endif // !RAYGEN_H
