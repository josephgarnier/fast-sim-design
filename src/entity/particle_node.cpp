////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "particle_node.h"
#include "entity_data.h"

#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <unordered_map>

namespace FastSimDesign {
  namespace {
    std::unordered_map<Particle::Type, ParticleData> Data_table = initializeParticleData();
  }
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////

  ParticleNode::ParticleNode(Particle::Type type, TextureHolder const& textures) noexcept
    : Parent{}
    , m_particules{}
    , m_texture{textures.get(Textures::ID::PARTICLE)}
    , m_type{type}
    , m_vertices{sf::Quads}
    , m_needs_vertex_update{true}
  {
  }

  void ParticleNode::addParticule(sf::Vector2f position) noexcept
  {
    Particle particule;
    particule.m_position = position;
    particule.m_color = Data_table[m_type].m_color;
    particule.m_lifetime = Data_table[m_type].m_lifetime;

    m_particules.push_back(std::move(particule));
  }

  Particle::Type ParticleNode::getParticuleType() const noexcept
  {
    return m_type;
  }

  BitFlags<Category::Type> ParticleNode::getCategory() const noexcept
  {
    return BitFlags<Category::Type>{Category::Type::PARTICLE_SYSTEM};
  }

  void ParticleNode::updateCurrent(sf::Time const& dt, CommandQueue&)
  {
    // Remove expired particles at beginning.
    while (!m_particules.empty() && m_particules.front().m_lifetime <= sf::Time::Zero)
      m_particules.pop_front();

    // Decrease lifetime of existing particles.
    for (Particle& particle : m_particules)
      particle.m_lifetime -= dt;

    m_needs_vertex_update = true;
  }

  void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
  {
    if (m_needs_vertex_update)
    {
      computeVertices();
      m_needs_vertex_update = false;
    }

    // Apply particle texture.
    states.texture = &m_texture;

    // Draw vertices.
    target.draw(m_vertices, states);
  }

  void ParticleNode::computeVertices() const noexcept
  {
    sf::Vector2f const size{m_texture.getSize()};
    sf::Vector2f const half = size / 2.f;

    // Refill vertex array.
    m_vertices.clear();
    for (auto const& particle : m_particules)
    {
      sf::Vector2f const& pos = particle.m_position;
      sf::Color color = particle.m_color;

      float ratio = particle.m_lifetime.asSeconds() / Data_table[m_type].m_lifetime.asSeconds();
      color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

      addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
      addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
      addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
      addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
    }
  }

  void ParticleNode::addVertex(float world_x, float world_y, float tex_coord_x, float tex_coord_y, sf::Color const& color) const noexcept
  {
    sf::Vertex vertex;
    vertex.position = sf::Vector2f{world_x, world_y};
    vertex.texCoords = sf::Vector2f{tex_coord_x, tex_coord_y};
    vertex.color = color;

    m_vertices.append(vertex);
  }

}