// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Animator.h"

namespace ouzel
{
    namespace scene
    {
        class Fade: public Animator
        {
        public:
            Fade(float pLength, float pOpacity, bool pRelative = false);

            virtual void start(const NodePtr& targetNode) override;

        protected:
            virtual void updateProgress() override;

            float opacity;
            float startOpacity = 0.0f;
            float targetOpacity = 0.0f;
            float diff = 0.0f;
            bool relative;
        };
    } // namespace scene
} // namespace ouzel
