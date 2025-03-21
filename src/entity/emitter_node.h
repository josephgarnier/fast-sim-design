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

#ifndef FAST_SIM_DESIGN_EMITTER_NODE_H
#define FAST_SIM_DESIGN_EMITTER_NODE_H

#include "particle.h"
#include "particle_node.h"
#include "../gui/scene_node.h"

#include <SFML/System/Time.hpp>

namespace FastSimDesign {
  class EmitterNode : public SceneNode
  {
  private:
    using Parent = SceneNode;

  public:
    explicit EmitterNode(Particle::Type type) noexcept; // Default constructor
    virtual ~EmitterNode() = default; // Destructor
  protected:
  private:
    virtual void updateCurrent(sf::Time const& dt, CommandQueue& commands) override;
    void emitParticles(sf::Time const& dt) noexcept;

  private:
    sf::Time m_accumulated_time;
    Particle::Type m_type;
    ParticleNode* m_particle_system;
  };
}
#endif