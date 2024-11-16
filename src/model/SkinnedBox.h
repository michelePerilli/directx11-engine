#pragma once

#pragma once
#include "../component/pipeline/DrawableBase.h"
#include <random>

#include "TestObject.h"

class ENGINE_DLL SkinnedBox : public TestObject<SkinnedBox> {
public:
    SkinnedBox(Graphics &gfx, std::mt19937 &rng,
               std::uniform_real_distribution<float> &adist,
               std::uniform_real_distribution<float> &ddist,
               std::uniform_real_distribution<float> &odist,
               std::uniform_real_distribution<float> &rdist);

    explicit SkinnedBox(Graphics &gfx);
    void init(Graphics &gfx);
};
