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

#include "../gui/scene_node.h"
#include "particle.h"
#include "particle_node.h"

#include <SFML/System/Time.hpp>

namespace FastSimDesign {
class EmitterNode final : public SceneNode
{
private:
  using Parent = SceneNode;

public:
  explicit EmitterNode(Particle::Type type) noexcept;
  virtual ~EmitterNode() = default;

private:
  virtual void updateCurrent(
      sf::Time const& dt, CommandQueue& commands) override;
  void emitParticles(sf::Time const& dt) noexcept;

private:
  sf::Time m_accumulated_time{sf::Time::Zero};
  Particle::Type m_type;
  ParticleNode* m_particle_system{nullptr};
};
} // namespace FastSimDesign
#endif