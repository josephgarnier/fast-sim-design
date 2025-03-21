////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "emitter_node.h"
#include "../core/command_queue.h"

#include <SFML/System/Time.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////

  EmitterNode::EmitterNode(Particle::Type type) noexcept
    : Parent{}
    , m_accumulated_time{sf::Time::Zero}
    , m_type{type}
    , m_particle_system{nullptr}
  {
  }

  void EmitterNode::updateCurrent(sf::Time const& dt, CommandQueue& commands)
  {
    if (m_particle_system)
    {
      emitParticles(dt);
    } else
    {
      // Find particle node with the same type as emitter node.
      auto finder = [this](ParticleNode& container, sf::Time) {
        if (container.getParticuleType() == m_type)
          m_particle_system = &container;
      };

      // Send a command through the scene graph to find the right particle system.
      Command finder_command;
      finder_command.name = "ParticleFinder";
      finder_command.category = BitFlags{Category::Type::PARTICLE_SYSTEM};
      finder_command.action = derivedAction<ParticleNode>(finder);

      commands.push(finder_command);
    }
  }

  void EmitterNode::emitParticles(sf::Time const& dt) noexcept
  {
    float const emission_rate = 30.f;
    sf::Time const interval = sf::seconds(1.f) / emission_rate;

    m_accumulated_time += dt;

    while (m_accumulated_time > interval)
    {
      m_accumulated_time -= interval;
      m_particle_system->addParticule(getWorldPosition());
    }
  }

}