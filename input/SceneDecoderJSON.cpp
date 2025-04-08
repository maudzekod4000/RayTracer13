#include "SceneDecoderJSON.h"

#include <assert.h>
#include <format>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

constexpr char missingInvalidKeyFmt[] = "Missing or invalid type for key '{}'";
constexpr char sizeMismatchFmt[] = "Unexpected element count of {} when {} is required for key '{}'";
constexpr char kSettings[] = "settings";
constexpr char kBgColor[] = "background_color";
constexpr char kImage[] = "image";
constexpr char kWidth[] = "width";
constexpr char kHeight[] = "height";
constexpr char kCamera[] = "camera";
constexpr char kMatrix[] = "matrix";
constexpr char kPos[] = "position";

std::expected<Scene, std::string> SceneDecoderJSON::decode(const uint8_t* data, size_t len)
{
  assert(data);
  assert(len > 0);

  using namespace rapidjson;

  Document d;

  const ParseResult r = d.Parse(reinterpret_cast<const char*>(data), len);

  if (!r) {
    return std::unexpected(GetParseError_En(r.Code()));
  }

  if (!d.IsObject()) {
    return std::unexpected("Expected object at the root.");
  }

  const auto& settings = d[kSettings];

  if (!settings.IsObject()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kSettings));
  }

  const auto& bgColor = settings[kBgColor];

  if (!bgColor.IsArray()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kBgColor));
  }

  constexpr int bgColorSize = 3;
  if (bgColor.Size() != bgColorSize) {
    return std::unexpected(std::format(sizeMismatchFmt, bgColor.Size(), bgColorSize, kBgColor));
  }

  // TODO: set in Scene

  const auto& image = settings[kImage];

  if (!image.IsObject()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kImage));
  }

  const auto& imageWidth = image[kWidth];

  if (!imageWidth.IsNumber()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, std::format("{}->{}", kImage, kWidth)));
  }

  // TODO: the width is ready

  const auto& imageHeight = image[kHeight];

  if (!imageHeight.IsNumber()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, std::format("{}->{}", kImage, kHeight)));
  }

  // TODO: the height is ready too

  const auto& camera = d[kCamera];

  if (!camera.IsObject()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kCamera));
  }

  const auto& matrix = camera[kMatrix];

  if (!matrix.IsArray()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kMatrix));
  }

  constexpr int matrixSize = 9;
  if (matrix.Size() != matrixSize) {
    return std::unexpected(std::format(sizeMismatchFmt, matrix.Size(), matrixSize, kMatrix));
  }

  const auto& pos = camera[kPos];

  if (!pos.IsArray()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kPos));
  }

  constexpr int posSize = 3;
  if (pos.Size() != posSize) {
    return std::unexpected(std::format(sizeMismatchFmt, pos.Size(), posSize, kPos));
  }
}
