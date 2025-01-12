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

#ifndef FAST_SIM_DESIGN_GAME_H
#define FAST_SIM_DESIGN_GAME_H

#include "../entity/player.h"
#include "world.h"
#include <SFML/Graphics/Text.hpp>

namespace FastSimDesign {
  class Game : private sf::NonCopyable
  {
  public:
  private:
    static float const PLAYER_SPEED;
    static sf::Time const TIME_PER_FRAME;

  public:
    explicit Game(); // Throw Exception
    virtual ~Game() = default; // Destructor

    void run() noexcept;

  protected:
  private:
    void processEvents() noexcept;
    void update(sf::Time const& elapsed_time) noexcept;
    void render() noexcept;
    void updateStatistic(sf::Time const& elapsed_time);

  public:
  protected:
  private:
    sf::RenderWindow m_window;
    World m_world;
    Player m_player;

    sf::Font m_font;
    sf::Text m_statistics_text;
    sf::Time m_statistics_update_time;
    std::size_t m_statistics_num_frames;
  };
}
#endif