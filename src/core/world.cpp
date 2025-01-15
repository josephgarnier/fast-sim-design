////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "world.h"
#include "../entity/aircraft.h"
#include "../gui/sprite_node.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace FastSimDesign {
  World::World(sf::RenderWindow& windows)
    : m_window{windows}
    , m_world_view{m_window.getDefaultView()}
    , m_textures{}
    , m_scene_graph{}
    , m_scene_layers{}
    , m_command_queue{}
    , m_world_bounds{0.f, 0.f, m_world_view.getSize().x, 2000.f}
    , m_spawn_position{m_world_view.getSize().x / 2.f, m_world_bounds.height - m_world_view.getSize().y / 2.f}
    , m_scroll_speed{-50.f}
    , m_player_aircraft{nullptr}
  {
    loadTextures();
    buildScene();

    // Prepare the view.
    m_world_view.setCenter(m_spawn_position);
  }

  void World::update(sf::Time const& dt)
  {
    // Scroll the world, reset player velocity.
    m_world_view.move(0.f, m_scroll_speed * dt.asSeconds());
    m_player_aircraft->setVelocity(0.f, 0.f);

    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    while (!m_command_queue.isEmpty())
      m_scene_graph.onCommand(m_command_queue.pop(), dt);
    adaptPlayerVelocity();

    // Regular update step, adapt position (correct if outside view)
    m_scene_graph.update(dt);
    adaptPlayerPosition();
  }

  void World::draw()
  {
    m_window.setView(m_world_view);
    m_window.draw(m_scene_graph);
  }

  CommandQueue& World::getCommandQueue() noexcept
  {
    return m_command_queue;
  }

  void World::loadTextures()
  {
    m_textures.load(Textures::ID::EAGLE, "../assets/sprites/npcs/eagle.png");
    m_textures.load(Textures::ID::RAPTOR, "../assets/sprites/npcs/raptor.png");
    m_textures.load(Textures::ID::DESERT, "../assets/sprites/npcs/desert.png");
  }

  void World::buildScene()
  {
    // Intilialize the different layers.
    for (std::size_t i = 0; i < static_cast<std::size_t>(World::Layer::LAYER_COUNT); ++i)
    {
      SceneNode::Ptr layer = std::make_unique<SceneNode>();
      m_scene_layers[i] = layer.get();

      m_scene_graph.attachChild(std::move(layer));
    }

    // Prepare the tiled background
    sf::Texture& texture = m_textures.get(Textures::ID::DESERT);
    sf::IntRect texture_rect{m_world_bounds};
    texture.setRepeated(true);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> background_sprite = std::make_unique<SpriteNode>(texture, texture_rect);
    background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
    m_scene_layers[static_cast<size_t>(World::Layer::BACKGROUND)]->attachChild(std::move(background_sprite));

    // Add player's aircraft
    std::unique_ptr<Aircraft> leader = std::make_unique<Aircraft>(Aircraft::Type::EAGLE, m_textures);
    m_player_aircraft = leader.get();
    m_player_aircraft->setPosition(m_spawn_position);
    m_scene_layers[static_cast<std::size_t>(World::Layer::AIR)]->attachChild(std::move(leader));

    // Add two escorting aircraft, placed relatively to the main plane
    std::unique_ptr<Aircraft> left_escort = std::make_unique<Aircraft>(Aircraft::Type::RAPTOR, m_textures);
    left_escort->setPosition(-80.f, 50.f);
    m_player_aircraft->attachChild(std::move(left_escort));

    std::unique_ptr<Aircraft> right_escort = std::make_unique<Aircraft>(Aircraft::Type::RAPTOR, m_textures);
    right_escort->setPosition(80.f, 50.f);
    m_player_aircraft->attachChild(std::move(right_escort));
  }

  void World::adaptPlayerPosition()
  {
    // Keep player's position inside the screen bounds, at least borderDistance units from the border.
    sf::FloatRect view_bounds{m_world_view.getCenter() - m_world_view.getSize() / 2.f, m_world_view.getSize()};
    float const border_distance = 40.f;

    sf::Vector2f position = m_player_aircraft->getPosition();
    position.x = std::max(position.x, view_bounds.left + border_distance);
    position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
    position.y = std::max(position.y, view_bounds.top + border_distance);
    position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
    m_player_aircraft->setPosition(position);
  }

  void World::adaptPlayerVelocity() noexcept
  {
    sf::Vector2f velocity = m_player_aircraft->getVelocity();

    // If moving diagonally, reduce velocity (to have always same velocity).
    if (velocity.x != 0.f && velocity.y != 0.f)
      m_player_aircraft->setVelocity(velocity / std::sqrt(2.f));

    // Add scrolling velocity.
    m_player_aircraft->accelerate(0.f, m_scroll_speed);
  }
}