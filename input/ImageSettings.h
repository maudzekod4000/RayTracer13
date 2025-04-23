#ifndef IMAGE_SETTINGS_H
#define IMAGE_SETTINGS_H

#include <cstdint>

class ImageSettings final {
public:
	explicit ImageSettings(uint16_t w, uint16_t h) noexcept;

	uint16_t getWidth() const noexcept;
	uint16_t getHeight() const noexcept;

	ImageSettings(const ImageSettings&) = delete;
	ImageSettings& operator=(const ImageSettings&) = delete;

	ImageSettings(ImageSettings&&) = default;
private:
	uint16_t width;
	uint16_t height;
};

#endif // !IMAGE_SETTINGS_H
