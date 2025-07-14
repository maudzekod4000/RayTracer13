#include "CameraSettings.h"

CameraSettings::CameraSettings(Mat&& tm, Vec&& pos) noexcept :
	tm(std::move(tm)), pos(std::move(pos))
{
}

const Mat& CameraSettings::getTransform() const noexcept
{
	return tm;
}

const Vec& CameraSettings::getPos() const noexcept
{
	return pos;
}
