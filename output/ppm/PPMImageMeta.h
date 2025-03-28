#ifndef PPM_IMAGE_META_H
#define PPM_IMAGE_META_H

#include <ostream>
#include <stdint.h>

/// <summary>
/// Holds metadata of a PPM output image.
/// Has methods for writing metadata to an output stream.
/// </summary>
class PPMImageMeta {
public:
	friend class PPMImage;
	PPMImageMeta(
		uint16_t imgWidth,
		uint16_t imgHeight,
		uint16_t maxColor
	);

	/// <summary>
	/// Given an output stream it writes instance fields to it.
	/// </summary>
	/// <param name="os">Output stream to write to.</param>
	void writeHeaders(std::ostream& os) const;
private:
	const uint16_t imgWidth;
	const uint16_t imgHeight;
	const uint16_t maxColor;
};

#endif // !PPM_IMAGE_META_H
