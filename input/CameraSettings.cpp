#include "CameraSettings.h"

CameraSettings::CameraSettings(Mat3&& tm, Vec&& pos) noexcept :
	tm(std::move(tm)), pos(std::move(pos))
{
}

const Mat3& CameraSettings::getTransform() const noexcept
{
	return tm;
}

const Vec& CameraSettings::getPos() const noexcept
{
	return pos;
}
