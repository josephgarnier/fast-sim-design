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

#ifndef FAST_SIM_DESIGN_WORLD_H
#define FAST_SIM_DESIGN_WORLD_H

#include "resource_identifiers.h"
#include "command_queue.h"
#include "../gui/scene_node.h"
#include "../entity/aircraft.h"
#include "../monitor/monitorable.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <vector>

namespace FastSimDesign {
  class Aircraft;
  class World final : private sf::NonCopyable
    , public SimMonitor::Monitorable
  {
  public:
    enum class Layer : uint16_t
    {
      BACKGROUND,
      AIR,
      LAYER_COUNT,
    };
    struct SpawnPoint
    {
      explicit SpawnPoint(Aircraft::Type type_, float x_, float y_) noexcept; // Default constructor
      SpawnPoint(SpawnPoint const&) = default; // Copy constructor
      SpawnPoint(SpawnPoint&&) = default; // Move constructor
      SpawnPoint& operator=(SpawnPoint const&) = default; // Copy assignment operator
      SpawnPoint& operator=(SpawnPoint&&) = default; // Move assignment operator
      virtual ~SpawnPoint() = default; // Destructor

      Aircraft::Type type;
      float x;
      float y;
    };

  public:
    using SimMonitor::Monitorable::monitorState;

  public:
    explicit World(SimMonitor::Monitor& monitor, sf::RenderWindow& windows, FontHolder& fonts); // Default constructor
    virtual ~World(); // Destructor

    void update(sf::Time const& dt);
    void draw();
    CommandQueue& getCommandQueue() noexcept;

    virtual void monitorState(SimMonitor::Monitor& monitor, SimMonitor::Frame::World& frame_object) const override final;

    bool hasAlivePlayer() const noexcept;
    bool hasPlayerReachedEnd() const;

  protected:
  private:
    void loadTextures();
    void buildScene();
    void adaptPlayerPosition();
    void adaptPlayerVelocity() noexcept;
    void handleCollisions() noexcept;
    bool matchesCategories(SceneNode::Pair& colliders, BitFlags<Category::Type>  type_1, BitFlags<Category::Type>  type_2) const noexcept;

    void addEnemies() noexcept;
    void addEnemy(Aircraft::Type type, float rel_x, float rel_y) noexcept;
    void spawnEnemies() noexcept;
    void destroyEntitiesOusideView() noexcept;
    void guideMissiles() noexcept;
    sf::FloatRect getViewBounds() const noexcept;
    sf::FloatRect getBattlefieldBounds() const noexcept;

  public:
  protected:
  private:
    sf::RenderWindow& m_window;
    sf::View m_world_view;
    TextureHolder m_textures;
    FontHolder& m_fonts;
    SimMonitor::Monitor& m_monitor;

    SceneNode m_scene_graph;
    std::array<SceneNode*, static_cast<std::size_t>(Layer::LAYER_COUNT)> m_scene_layers;
    CommandQueue m_command_queue;

    sf::FloatRect m_world_bounds;
    sf::Vector2f m_spawn_position;
    float m_scroll_speed;
    Aircraft* m_player_aircraft;

    std::vector<SpawnPoint> m_enemy_spawn_points;
    std::vector<Aircraft*> m_active_enemies;
  };
}
#endif