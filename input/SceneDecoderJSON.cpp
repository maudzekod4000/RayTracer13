#include "SceneDecoderJSON.h"

#include <assert.h>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

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
    return std::unexpected("JSON: Expected object at the root.");
  }

  // TODO: Mahni si .vs ot git
}
