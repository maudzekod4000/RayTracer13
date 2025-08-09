#pragma once

#include "calc/TypeDefs.h"

struct Light {
	Vec pos;
  Vec albedo = DirectX::XMVectorSplatOne(); /// White light
	int intensity;
};
