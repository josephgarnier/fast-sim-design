////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "menu_state.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
  MenuState::MenuState(StateStack* stack, Context context)
    : Parent{stack, std::move(context), "MENU"}
    , m_background_sprite{}
    , m_options{}
    , m_option_index{0}
  {
    sf::Texture& texture = context.textures->get(Textures::ID::TITLE_SCREEN);
    sf::Font& font = context.fonts->get(Fonts::ID::MAIN);

    m_background_sprite.setTexture(texture);

    // A simple menu demonstration
    sf::Text play_option;
    play_option.setFont(font);
    play_option.setString("Play");
    centerOrigin(play_option);
    play_option.setPosition(context.window->getView().getSize() / 2.f);
    m_options.push_back(play_option);

    sf::Text exit_option;
    exit_option.setFont(font);
    exit_option.setString("Exit");
    centerOrigin(exit_option);
    exit_option.setPosition(play_option.getPosition() + sf::Vector2f{0.f, 50.f});
    m_options.push_back(exit_option);

    updateOptionText();
  }

  bool MenuState::handleEvent(sf::Event const& event)
  {
    if (event.type != sf::Event::KeyPressed)
      return false;
    if (event.key.code == sf::Keyboard::Return)
    {
      if (m_option_index == static_cast<size_t>(OptionNames::PLAY))
      {
        requestStackPop();
        requestStackPush(States::ID::GAME);
      } else if (m_option_index == static_cast<size_t>(OptionNames::EXIT))
      {
        // The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
        requestStackPop();
      }
    } else if (event.key.code == sf::Keyboard::Up)
    {
      if (m_option_index > 0)
        m_option_index--;
      else
        m_option_index = m_options.size() - 1;

      updateOptionText();
    } else if (event.key.code == sf::Keyboard::Down)
    {
      if (m_option_index < m_options.size() - 1)
        m_option_index++;
      else
        m_option_index = 0;

      updateOptionText();
    }

    return true;
  }

  bool MenuState::update(sf::Time const &) noexcept
  {
    return true;
  }

  void MenuState::draw()
  {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(m_background_sprite);
    for (auto const& text : m_options)
      window.draw(text);
  }

  void MenuState::updateOptionText()
  {
    if (m_options.empty())
      return;

    for (auto& text : m_options)
      text.setFillColor(sf::Color::White);

    m_options[m_option_index].setFillColor(sf::Color::Red);
  }
}