////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#pragma once

#ifndef FAST_SIM_DESIGN_PARTICLE_NODE_H
#define FAST_SIM_DESIGN_PARTICLE_NODE_H

#include "particle.h"
#include "../gui/scene_node.h"
#include "../core/resource_identifiers.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>

namespace FastSimDesign {
  class ParticleNode final : public SceneNode
  {
  private:
    using Parent = SceneNode;

  public:
    explicit ParticleNode(Particle::Type type, TextureHolder const& textures) noexcept; // Default constructor
    virtual ~ParticleNode() = default; // Destructor

    void addParticule(sf::Vector2f position) noexcept;
    Particle::Type getParticuleType() const noexcept;
    virtual BitFlags<Category::Type> getCategory() const noexcept override;

  private:
    virtual void updateCurrent(sf::Time const& dt, CommandQueue& commands) override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    void computeVertices() const noexcept;
    void addVertex(float world_x, float world_y, float tex_coord_x, float tex_coord_y, sf::Color const& color) const noexcept;

  private:
    std::deque<Particle> m_particules;
    sf::Texture const& m_texture;
    Particle::Type m_type;

    mutable sf::VertexArray m_vertices;
    mutable bool m_needs_vertex_update;
  };
}
#endif