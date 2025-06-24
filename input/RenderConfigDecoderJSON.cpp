#include "RenderConfigDecoderJSON.h"

#include <unordered_map>
#include <assert.h>
#include <format>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "sampling/Vertex.h"
#include "sampling/Light.h"

constexpr char missingInvalidKeyFmt[] = "Missing or invalid type for key '{}'";
constexpr char sizeMismatchFmt[] = "Unexpected element count of {} when {} is required for key '{}'";
constexpr char invalidValueInArrayFmt[] = "Invalid value '{}' in '{}'";
constexpr char kSettings[] = "settings";
constexpr char kBgColor[] = "background_color";
constexpr char kImage[] = "image_settings";
constexpr char kWidth[] = "width";
constexpr char kHeight[] = "height";
constexpr char kCamera[] = "camera";
constexpr char kMatrix[] = "matrix";
constexpr char kPos[] = "position";
constexpr char kObjects[] = "objects";
constexpr char kVertices[] = "vertices";
constexpr char kTriangles[] = "triangles";
constexpr char kLights[] = "lights";
constexpr char kIntensity[] = "intensity";
constexpr char kAlbedo[] = "albedo";
constexpr char kMaterials[] = "materials";
constexpr char kType[] = "type";

struct ParseTriangle {
  int v1;
  int v2;
  int v3;
  int matIdx = -1;
};

std::expected<RenderConfig, std::string> RenderConfigDecoderJSON::decode(const uint8_t* data, size_t len)
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

	Vec3 backgroundColor(bgColor[0].GetFloat(), bgColor[1].GetFloat(), bgColor[2].GetFloat());

	const auto& image = settings[kImage];

	if (!image.IsObject()) {
		return std::unexpected(std::format(missingInvalidKeyFmt, kImage));
	}

	const auto& imageWidth = image[kWidth];
	constexpr int absurdlyBigImageSize = 10000;
	if (!imageWidth.IsNumber() || imageWidth.GetInt() < 1 || imageWidth.GetInt() > absurdlyBigImageSize) {
		return std::unexpected(std::format(missingInvalidKeyFmt, std::format("{}->{}", kImage, kWidth)));
	}

	const auto& imageHeight = image[kHeight];

	if (!imageHeight.IsNumber() || imageHeight.GetInt() < 1 || imageHeight.GetInt() > absurdlyBigImageSize) {
		return std::unexpected(std::format(missingInvalidKeyFmt, std::format("{}->{}", kImage, kHeight)));
	}

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

	for (size_t i = 0; i < matrixSize; i++) {
		if (!matrix[i].IsNumber()) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kMatrix));
		}
	}

	Mat3 cameraTm(
		matrix[0].GetFloat(),
		matrix[1].GetFloat(),
		matrix[2].GetFloat(),
		matrix[3].GetFloat(),
		matrix[4].GetFloat(),
		matrix[5].GetFloat(),
		matrix[6].GetFloat(),
		matrix[7].GetFloat(),
		matrix[8].GetFloat()
	);

	const auto& pos = camera[kPos];

	if (!pos.IsArray()) {
		return std::unexpected(std::format(missingInvalidKeyFmt, kPos));
	}

	constexpr int posSize = 3;
	if (pos.Size() != posSize) {
		return std::unexpected(std::format(sizeMismatchFmt, pos.Size(), posSize, kPos));
	}

	for (size_t i = 0; i < posSize; i++) {
		if (!pos[i].IsNumber()) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kPos));
		}
	}

	Vec3 cameraPos(pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat());

  std::vector<Material> sceneMaterials;
  if (d.HasMember("materials")) {
    const auto& materials = d[kMaterials];

    if (materials.IsArray()) {
      for (size_t i = 0; i < materials.Size(); i++) {
        const auto& material = materials[i].GetObject();

        Material sceneMaterial;

        auto type = material[kType].GetString();

        if (strcmp(type, "diffuse") == 0) {
          sceneMaterial.type = MaterialType::DIFFUSE;
        }
        else if (strcmp(type, "reflective") == 0) {
          sceneMaterial.type = MaterialType::REFLECTIVE;
        }
        else if (strcmp(type, "refractive") == 0) {
          sceneMaterial.type = MaterialType::REFRACTIVE;
        }
        else {
          sceneMaterial.type = MaterialType::DIFFUSE;
        }

        auto isSmoothShading = material["smooth_shading"].GetBool();

        sceneMaterial.smoothShading = isSmoothShading;

        auto albedoVec = material[kAlbedo].GetArray();

        sceneMaterial.albedo.x = albedoVec[0].GetFloat();
        sceneMaterial.albedo.y = albedoVec[1].GetFloat();
        sceneMaterial.albedo.z = albedoVec[2].GetFloat();

        sceneMaterials.push_back(sceneMaterial);
      }
    }

  }

	const auto& objects = d[kObjects];

	if (!objects.IsArray()) {
		return std::unexpected(std::format(missingInvalidKeyFmt, kObjects));
	}

	std::vector<Triangle> sceneTriangles;

	for (int objIdx = 0; objIdx < objects.Size(); objIdx++) {
    std::vector<ParseTriangle> parsedTriangles;
    std::unordered_map<int, Vertex> idxToVertex;
		const auto& object = objects.GetArray()[objIdx];
		if (!object.IsObject()) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kObjects));
		}

    // Material for the whole object
    int materialIdx = -1;

    if (object.HasMember("material_index")) {
      materialIdx = object["material_index"].GetInt();
    }

		const auto& vertices = object[kVertices];

		if (!vertices.IsArray() || vertices.Size() == 0 || vertices.Size() % 3 != 0) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kVertices));
		}

		const auto& triangles = object[kTriangles];

		if (!triangles.IsArray() || triangles.Size() == 0 || triangles.Size() % 3 != 0) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kTriangles));
		}

		for (size_t i = 0; i < triangles.Size(); i += 3) {
			const auto& vertex1Idx = triangles[i];

			if (!vertex1Idx.IsNumber() || vertex1Idx.GetInt() < 0 || vertex1Idx.GetInt() >= vertices.Size()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex1Idx.GetString(), kTriangles));
			}

			const auto& vertex1x = vertices[vertex1Idx.GetInt() * 3];

			if (!vertex1x.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex1x.GetString(), kVertices));
			}

			const auto& vertex1y = vertices[vertex1Idx.GetInt() * 3 + 1];

			if (!vertex1y.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex1y.GetString(), kVertices));
			}

			const auto& vertex1z = vertices[vertex1Idx.GetInt() * 3 + 2];

			if (!vertex1z.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex1z.GetString(), kVertices));
			}

			Vec3 vertex1Pos{ vertex1x.GetFloat(), vertex1y.GetFloat(), vertex1z.GetFloat() };

      idxToVertex[vertex1Idx.GetInt()] = Vertex(vertex1Pos);

			const auto& vertex2Idx = triangles[i + 1];

			if (!vertex2Idx.IsNumber() || vertex2Idx.GetInt() < 0 || vertex2Idx.GetInt() >= vertices.Size()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex2Idx.GetString(), kTriangles));
			}

			const auto& vertex2x = vertices[vertex2Idx.GetInt() * 3];

			if (!vertex2x.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex2x.GetString(), kVertices));
			}

			const auto& vertex2y = vertices[vertex2Idx.GetInt() * 3 + 1];

			if (!vertex2y.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex2y.GetString(), kVertices));
			}

			const auto& vertex2z = vertices[vertex2Idx.GetInt() * 3 + 2];

			if (!vertex2z.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex2z.GetString(), kVertices));
			}

			Vec3 vertex2Pos{ vertex2x.GetFloat(), vertex2y.GetFloat(), vertex2z.GetFloat() };

      idxToVertex[vertex2Idx.GetInt()] = Vertex(vertex2Pos);

			const auto& vertex3Idx = triangles[i + 2];

			if (!vertex3Idx.IsNumber() || vertex3Idx.GetInt() < 0 || vertex3Idx.GetInt() >= vertices.Size()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex3Idx.GetString(), kTriangles));
			}

			const auto& vertex3x = vertices[vertex3Idx.GetInt() * 3];

			if (!vertex3x.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex3x.GetString(), kVertices));
			}

			const auto& vertex3y = vertices[vertex3Idx.GetInt() * 3 + 1];

			if (!vertex3y.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex3y.GetString(), kVertices));
			}

			const auto& vertex3z = vertices[vertex3Idx.GetInt() * 3 + 2];

			if (!vertex3z.IsNumber()) {
				return std::unexpected(std::format(invalidValueInArrayFmt, vertex3z.GetString(), kVertices));
			}

			Vec3 vertex3Pos{ vertex3x.GetFloat(), vertex3y.GetFloat(), vertex3z.GetFloat() };

      idxToVertex[vertex3Idx.GetInt()] = Vertex(vertex3Pos);

      ParseTriangle pt{ vertex1Idx.GetInt(), vertex2Idx.GetInt(), vertex3Idx.GetInt(), materialIdx };

      parsedTriangles.push_back(pt);
		}

    // Calculate smooth normals.
    for (const auto& triangle : parsedTriangles) {
      Vertex& v1 = idxToVertex[triangle.v1];
      Vertex& v2 = idxToVertex[triangle.v2];
      Vertex& v3 = idxToVertex[triangle.v3];

      Vec3 n = MathUtils::normal(v1.pos, v2.pos, v3.pos);

      v1.normal += n;
      v1.normal = glm::normalize(v1.normal);

      v2.normal += n;
      v2.normal = glm::normalize(v2.normal);

      v3.normal += n;
      v3.normal = glm::normalize(v3.normal);
    }

    for (const auto& triangle : parsedTriangles) {
      Vertex& v1 = idxToVertex[triangle.v1];
      Vertex& v2 = idxToVertex[triangle.v2];
      Vertex& v3 = idxToVertex[triangle.v3];

      if (triangle.matIdx > -1) {
		    sceneTriangles.emplace_back(v1, v2, v3, sceneMaterials[triangle.matIdx]);

      }
      else {
        sceneTriangles.emplace_back(v1, v2, v3, Material{});
      }
    }

	}

	std::vector<Light> sceneLights;

	const auto& lights = d[kLights];

	if (!lights.IsNull() && !lights.IsArray()) {
		return std::unexpected(std::format(missingInvalidKeyFmt, kLights));
	}

	for (int i = 0; i < lights.Size(); i++) {
		const auto& light = lights.GetArray()[i];
		if (!light.IsObject()) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kLights));
		}

		const auto& pos = light[kPos];

		if (!pos.IsArray() && pos.Size() != 3) {
			return std::unexpected(std::format(missingInvalidKeyFmt, kLights));
		}

		Light sceneLight;
		sceneLight.pos = Vec3(pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat());
		
		const auto& intensity = light[kIntensity];

		sceneLight.intensity = intensity.GetInt();

    if (light.HasMember(kAlbedo)) {
      const auto& albedo = light[kAlbedo];

      if (albedo.IsArray() && albedo.Size() == 3) {
        sceneLight.albedo = Vec3(albedo[0].GetFloat(), albedo[1].GetFloat(), albedo[2].GetFloat());
      }
    }

		sceneLights.push_back(sceneLight);
	}

	CameraSettings cs(std::move(cameraTm), std::move(cameraPos));
	ImageSettings is(uint16_t(imageWidth.GetInt()), uint16_t(imageHeight.GetInt()));
	Settings s(backgroundColor);
	Scene sc(std::move(sceneTriangles), std::move(sceneLights), backgroundColor);

	return RenderConfig(std::move(cs), std::move(s), std::move(is), std::move(sc));
}
