#ifndef CAMERA_SETTINGS_H
#define CAMERA_SETTINGS_H

#include "calc/TypeDefs.h"

class CameraSettings final {
public:
	explicit CameraSettings(Mat3&& tm, Vec&& pos) noexcept;

	const Mat3& getTransform() const noexcept;

	const Vec& getPos() const noexcept;

	// This class is not big, but it may grow and also, having the CameraSettings at more
	// than one place is kinda smell and could lead to bugs.
	CameraSettings(const CameraSettings&) = delete;
	CameraSettings& operator=(const CameraSettings&) = delete;
	
	CameraSettings(CameraSettings&&) = default;
private:
	/// Transformation matrix
	Mat3 tm;
	/// Camera position in space.
	Vec pos;
};

#endif // !CAMERA_SETTINGS_H
