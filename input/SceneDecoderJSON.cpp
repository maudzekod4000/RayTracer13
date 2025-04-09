#include "SceneDecoderJSON.h"

#include <assert.h>
#include <format>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

constexpr char missingInvalidKeyFmt[] = "Missing or invalid type for key '{}'";
constexpr char sizeMismatchFmt[] = "Unexpected element count of {} when {} is required for key '{}'";
constexpr char invalidValueInArrayFmt[] = "Invalid value '{}' in '{}'";
constexpr char kSettings[] = "settings";
constexpr char kBgColor[] = "background_color";
constexpr char kImage[] = "image";
constexpr char kWidth[] = "width";
constexpr char kHeight[] = "height";
constexpr char kCamera[] = "camera";
constexpr char kMatrix[] = "matrix";
constexpr char kPos[] = "position";
constexpr char kObjects[] = "objects";
constexpr char kVertices[] = "vertices";
constexpr char kTriangles[] = "triangles";

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

  const auto& objects = d[kObjects];

  if (!objects.IsArray()) {
    return std::unexpected(std::format(missingInvalidKeyFmt, kObjects));
  }

  for (const auto& object : objects.GetArray()) {
    if (!object.IsObject()) {
      return std::unexpected(std::format(missingInvalidKeyFmt, kObjects));
    }

    const auto& vertices = object[kVertices];

    if (vertices.IsArray() || vertices.Size() == 0 || vertices.Size() % 3 != 0) {
      return std::unexpected(std::format(missingInvalidKeyFmt, kVertices));
    }

    const auto& triangles = object[kTriangles];

    if (triangles.IsArray() || triangles.Size() == 0 || triangles.Size() % 3 != 0) {
      return std::unexpected(std::format(missingInvalidKeyFmt, kTriangles));
    }

    for (size_t i = 0; i < triangles.Size(); i += 3) {
      const auto& v1Idx = triangles[i];

      if (!v1Idx.IsNumber() || v1Idx.GetInt() < 0 || v1Idx.GetInt() >= vertices.Size()) {
        return std::unexpected(std::format(invalidValueInArrayFmt, v1Idx.GetString(), kTriangles));
      }

      const auto& vertex1x = vertices[v1Idx.GetInt()];

      if (!vertex1x.IsFloat()) {
        return std::unexpected(std::format(invalidValueInArrayFmt, vertex1x.GetString(), kVertices));
      }



      Vec3 vertex1Pos{};
    }
  }
}
