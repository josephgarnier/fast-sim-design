////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "pause_state.h"
#include "../core/resource_identifiers.h"
#include "../utils/sfml_util.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
  PauseState::PauseState(StateStack* stack, Context context) noexcept
    : Parent{stack, std::move(context)}
    , m_background_sprite{}
    , m_paused_text{}
    , m_instruction_text{}
  {
    sf::Font& font = context.fonts->get(Fonts::ID::MAIN);
    sf::Vector2f view_size = context.window->getView().getSize();

    m_paused_text.setFont(font);
    m_paused_text.setString("Game Paused");
    m_paused_text.setCharacterSize(70);
    centerOrigin(m_paused_text);
    m_paused_text.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    m_instruction_text.setFont(font);
    m_instruction_text.setString("(Press Backspace to return to the main menu)");
    centerOrigin(m_instruction_text);
    m_instruction_text.setPosition(0.5f * view_size.x, 0.6f * view_size.y);
  }

  bool PauseState::handleEvent(sf::Event const& event) noexcept
  {
    if (event.type != sf::Event::KeyPressed)
      return false;

    if (event.key.code == sf::Keyboard::Escape)
    {
      requestStackPop();
    }

    if (event.key.code == sf::Keyboard::BackSpace)
    {
      requestStateClear();
      requestStackPush(States::ID::MENU);
    }
    return false;
  }

  bool PauseState::update(sf::Time const & dt) noexcept
  {
    return false;
  }

  void PauseState::draw() noexcept
  {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape background_shape;
    background_shape.setFillColor(sf::Color(0, 0, 0, 150));
    background_shape.setSize(window.getView().getSize());

    window.draw(background_shape);
    window.draw(m_paused_text);
    window.draw(m_instruction_text);
  }
}