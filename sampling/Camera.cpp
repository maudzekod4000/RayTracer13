#include "Camera.h"

Camera::Camera(const Vec3& pos, const Mat3& matrix, uint32_t sensorWidth, uint32_t sensorHeight, float focalDistance)
    : pos(pos),
    transformationMatrix(Mat4(matrix)),
    sensorWidth(sensorWidth),
    sensorHeight(sensorHeight),
    aspectRatio(float(sensorWidth) / sensorHeight),
    focalDist(focalDistance)
{
}

Ray Camera::generateRay(float x, float y)
{
    /* Sample the pixel at its center */
    x += 0.5f;
    y += 0.5f;

    /* Transform to NDC: 0.0 - 1.0 */
    x /= sensorWidth;
    y /= sensorHeight;

    /* To Screen space: bottom left is [-1;-1] and top right is [1;1] and 0,0 is at the center of the screen */
    x = (x * 2.0f) - 1.0f;
    y = 1.0f - (y * 2.0f);

    /* Consider the aspect ratio, because pixels might not be square and we added 0.5 to both components. */
    x *= aspectRatio;

    const Vec3 rayDir(x, y, focalDist);

    return Ray(pos, glm::normalize(applyTransformation(rayDir)));
}
