#include "CameraSettings.h"

CameraSettings::CameraSettings(Mat3&& tm, Vec3&& pos) noexcept :
	tm(std::move(tm)), pos(std::move(pos))
{
}

const Mat3& CameraSettings::getTransform() const noexcept
{
	return tm;
}

const Vec3& CameraSettings::getPos() const noexcept
{
	return pos;
}
