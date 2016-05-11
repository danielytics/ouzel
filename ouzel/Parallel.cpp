// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Parallel.h"

namespace ouzel
{
    namespace scene
    {
        Parallel::Parallel(const std::initializer_list<Animator*>& pAnimators):
            Animator(0.0f), animators(pAnimators)
        {
            for (Animator* animator : animators)
            {
                animator->retain();

                if (animator->getLength() > length)
                {
                    length = animator->getLength();
                }
            }
        }

        Parallel::~Parallel()
        {
            for (Animator* animator : animators)
            {
                animator->release();
            }
        }

        void Parallel::start(Node* targetNode)
        {
            Animator::start(targetNode);

            for (Animator* animator : animators)
            {
                animator->start(targetNode);
            }
        }

        void Parallel::reset()
        {
            Animator::reset();

            for (Animator* animator : animators)
            {
                animator->reset();
            }
        }

        void Parallel::updateProgress()
        {
            Animator::updateProgress();

            for (Animator* animator : animators)
            {
                float animationLength = animator->getLength();

                if (animationLength <= 0.0f || currentTime > animationLength)
                {
                    animator->setProgress(1.0f);
                }
                else
                {
                    animator->setProgress(currentTime / animationLength);
                }
            }
        }
    } // namespace scene
} // namespace ouzel
