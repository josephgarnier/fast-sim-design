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
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace FastSimDesign {
  class Aircraft;
  class World : private sf::NonCopyable
  {
  public:
    enum class Layer : int
    {
      BACKGROUND,
      AIR,
      LAYER_COUNT,
    };

  public:
    explicit World(sf::RenderWindow& windows); // Default constructor
    virtual ~World() = default; // Destructor

    void update(sf::Time const & dt) noexcept;
    void draw() noexcept;
    CommandQueue& getCommandQueue() noexcept;

  protected:
  private:
    void loadTextures() noexcept;
    void buildScene() noexcept;
    void adaptPlayerPosition() noexcept;
    void adaptPlayerVelocity() noexcept;

  public:
  protected:
  private:
    sf::RenderWindow& m_window;
    sf::View m_world_view;
    TextureHolder m_textures;

    SceneNode m_scene_graph;
    std::array<SceneNode*, static_cast<std::size_t>(Layer::LAYER_COUNT)> m_scene_layers;
    CommandQueue m_command_queue;

    sf::FloatRect m_world_bounds;
    sf::Vector2f m_spawn_position;
    float m_scroll_speed;
    Aircraft* m_player_aircraft;
  };
}
#endif