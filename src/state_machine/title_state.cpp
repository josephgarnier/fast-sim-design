////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "title_state.h"
#include "../core/resource_identifiers.h"
#include "../utils/sfml_util.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace FastSimDesign {
  TitleState::TitleState(StateStack* stack, Context context) noexcept
    : Parent{stack, std::move(context)}
    , m_background_sprite{}
    , m_text{}
    , m_show_text{true}
    , m_text_effect_time{sf::Time::Zero}
  {
    m_background_sprite.setTexture(context.textures->get(Textures::ID::TITLE_SCREEN));

    m_text.setFont(context.fonts->get(Fonts::ID::MAIN));
    m_text.setString("Press any key to start");
    centerOrigin(m_text);
    m_text.setPosition(context.window->getView().getSize() / 2.f);
  }

  bool TitleState::handleEvent(sf::Event const& event) noexcept
  {
    // If any key is pressed, trigger the next screen.
    if (event.type == sf::Event::KeyPressed)
    {
      requestStackPop();
      requestStackPush(States::ID::MENU);
    }
    return true;
  }

  bool TitleState::update(sf::Time const & dt) noexcept
  {
    m_text_effect_time += dt;
    if (m_text_effect_time >= sf::seconds(0.5f))
    {
      m_show_text = !m_show_text;
      m_text_effect_time = sf::Time::Zero;
    }
    return true;
  }

  void TitleState::draw() noexcept
  {
    sf::RenderWindow& window = *getContext().window;
    window.draw(m_background_sprite);
    if (m_show_text)
      window.draw(m_text);
  }
}