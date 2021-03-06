// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <memory>
#include <vector>
#include <set>
#include <cstdint>
#include "Types.h"
#include "Noncopyable.h"
#include "Rectangle.h"

namespace ouzel
{
    namespace scene
    {
        class Scene: public Noncopyable, public std::enable_shared_from_this<Scene>
        {
        public:
            Scene();
            virtual ~Scene();

            virtual void draw();

            void addLayer(const LayerPtr& layer);
            void removeLayer(const LayerPtr& layer);
            void removeAllLayers();
            bool hasLayer(const LayerPtr& layer) const;
            const std::vector<LayerPtr>& getLayers() const { return layers; }

            virtual void recalculateProjection();

            NodePtr pickNode(const Vector2& position) const;
            std::set<NodePtr> pickNodes(const std::vector<Vector2>& edges) const;

        protected:
            std::vector<LayerPtr> layers;
        };
    } // namespace scene
} // namespace ouzel
