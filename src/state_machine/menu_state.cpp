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

#include "../gui/button.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

namespace FastSimDesign {
MenuState::MenuState(StateStack* stack, Context context)
  : Parent{stack, context, "MENU"}
{
  sf::Texture const& texture =
      context.textures->get(Textures::ID::TITLE_SCREEN);
  m_background_sprite.setTexture(texture);

  auto play_button =
      std::make_shared<GUI::Button>(*context.fonts, *context.textures);
  play_button->setPosition(100, 250);
  play_button->setText("Play");
  play_button->setCallback([this]() {
    requestStackPop();
    requestStackPush(States::ID::GAME);
  });

  auto settings_button =
      std::make_shared<GUI::Button>(*context.fonts, *context.textures);
  settings_button->setPosition(100, 300);
  settings_button->setText("Settings");
  settings_button->setCallback(
      [this]() { requestStackPush(States::ID::SETTINGS); });

  auto exit_button =
      std::make_shared<GUI::Button>(*context.fonts, *context.textures);
  exit_button->setPosition(100, 350);
  exit_button->setText("Exit");
  exit_button->setCallback([this]() { requestStackPop(); });

  m_gui_container.pack(play_button);
  m_gui_container.pack(settings_button);
  m_gui_container.pack(exit_button);
}

bool MenuState::handleEvent(sf::Event const& event)
{
  m_gui_container.handleEvent(event);
  return false;
}

bool MenuState::update(sf::Time const&) noexcept
{
  return true;
}

void MenuState::draw()
{
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(m_background_sprite);
  window.draw(m_gui_container);
}
} // namespace FastSimDesign