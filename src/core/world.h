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

#include "../entity/aircraft.h"
#include "../gui/bloom_effect.h"
#include "../gui/scene_node.h"
#include "../monitor/monitorable.h"
#include "command_queue.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

namespace sf {
class RenderWindow;
}
namespace FastSimDesign {
class Aircraft;
class World final
  : private sf::NonCopyable
  , public SimMonitor::Monitorable
{
public:
  enum class Layer : uint16_t
  {
    BACKGROUND,
    LOWER_AIR,
    UPPER_AIR,
    LAYER_COUNT,
  };
  struct SpawnPoint
  {
    explicit SpawnPoint(Aircraft::Type type_, float x_, float y_) noexcept;
    SpawnPoint(SpawnPoint const&) = default;
    SpawnPoint(SpawnPoint&&) = default;
    SpawnPoint& operator=(SpawnPoint const&) = default;
    SpawnPoint& operator=(SpawnPoint&&) = default;
    virtual ~SpawnPoint() = default;

    Aircraft::Type type;
    float x{0.f};
    float y{0.f};
  };

public:
  using SimMonitor::Monitorable::monitorState;

public:
  explicit World(
      SimMonitor::Monitor& monitor,
      sf::RenderWindow& windows,
      FontHolder& fonts);
  virtual ~World();

  void update(sf::Time const& dt);
  void draw();
  CommandQueue& getCommandQueue() noexcept;

  virtual void monitorState(
      SimMonitor::Monitor& monitor,
      SimMonitor::Frame::World& frame_object) const override final;

  bool hasAlivePlayer() const noexcept;
  bool hasPlayerReachedEnd() const;

protected:
private:
  void loadTextures();
  void buildScene();
  void adaptPlayerPosition();
  void adaptPlayerVelocity() noexcept;
  void handleCollisions() noexcept;
  bool matchesCategories(
      SceneNode::Pair& colliders,
      BitFlags<Category::Type> type_1,
      BitFlags<Category::Type> type_2) const noexcept;

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
  sf::RenderTexture m_scene_texture{};
  sf::View m_world_view{};
  TextureHolder m_textures{};
  FontHolder& m_fonts;
  SimMonitor::Monitor& m_monitor;

  SceneNode m_scene_graph{};
  std::array<SceneNode*, static_cast<std::size_t>(Layer::LAYER_COUNT)>
      m_scene_layers{};
  CommandQueue m_command_queue{};

  sf::FloatRect m_world_bounds{};
  sf::Vector2f m_spawn_position{};
  float m_scroll_speed{-50.f};
  Aircraft* m_player_aircraft{nullptr};

  std::vector<SpawnPoint> m_enemy_spawn_points{};
  std::vector<Aircraft*> m_active_enemies{};

  BloomEffet m_bloom_effect{};
};
} // namespace FastSimDesign
#endif