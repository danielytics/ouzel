// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#include "Animator.h"
#include "Types.h"

namespace ouzel
{
    namespace scene
    {
        class Sequence: public Animator
        {
        public:
            Sequence(const std::vector<AnimatorPtr>& pAnimators);

            virtual void start(const NodePtr& node) override;
            virtual void reset() override;

        protected:
            virtual void updateProgress() override;

            std::vector<AnimatorPtr> animators;
        };
    } // namespace scene
} // namespace ouzel
