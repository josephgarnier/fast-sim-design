////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "game_over_state.h"

#include "../core/resource_identifiers.h"
#include "../entity/player.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
GameOverState::GameOverState(StateStack* stack, Context context)
  : Parent{stack, context, "GAME_OVER"}
{
  sf::Font& font = context.fonts->get(Fonts::ID::MAIN);
  sf::Vector2f window_size{context.window->getSize()};

  m_game_over_text.setFont(font);
  if (context.player->getMissionStatus() ==
      Player::MissionStatus::MISSION_FAILURE)
    m_game_over_text.setString("Mission failed!");
  else
    m_game_over_text.setString("Mission successful!");

  m_game_over_text.setCharacterSize(70);
  SFML::centerOrigin(m_game_over_text);
  m_game_over_text.setPosition(0.5f * window_size.x, 0.4f * window_size.y);
}

bool GameOverState::handleEvent(sf::Event const&)
{
  return false;
}

bool GameOverState::update(sf::Time const& dt) noexcept
{
  // Show state for 3 seconds, after return to menu.
  m_elapsed_time += dt;
  if (m_elapsed_time > sf::seconds(3))
  {
    requestStateClear();
    requestStackPush(States::ID::MENU);
  }
  return false;
}

void GameOverState::draw()
{
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());

  // Create dark, semitransparent background
  sf::RectangleShape background_shape;
  background_shape.setFillColor(sf::Color(0, 0, 0, 150));
  background_shape.setSize(window.getView().getSize());

  window.draw(background_shape);
  window.draw(m_game_over_text);
}
} // namespace FastSimDesign