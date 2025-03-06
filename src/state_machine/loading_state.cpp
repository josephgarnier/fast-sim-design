////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "loading_state.h"
#include "../core/resource_identifiers.h"
#include "../utils/sfml_util.h"
#include "state_identifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  LoadingState::LoadingState(StateStack* stack, Context context)
    : Parent{stack, context, "LOADING"}
    , m_loading_text{}
    , m_progress_bar_background{}
    , m_progress_bar{}
    , m_loading_task{}
  {
    sf::RenderWindow& window = *getContext().window;
    sf::Font& font = context.fonts->get(Fonts::ID::MAIN);
    sf::Vector2f view_size = window.getView().getSize();

    m_loading_text.setFont(font);
    m_loading_text.setString("Loading Resources");
    SFML::centerOrigin(m_loading_text);
    m_loading_text.setPosition(view_size.x / 2.f, view_size.y / 2.f + 50.f);

    m_progress_bar_background.setFillColor(sf::Color::White);
    m_progress_bar_background.setSize(sf::Vector2f(view_size.x - 20, 10));
    m_progress_bar_background.setPosition(10, m_loading_text.getPosition().y + 40);

    m_progress_bar.setFillColor(sf::Color(100, 100, 100));
    m_progress_bar.setSize(sf::Vector2f(200, 10));
    m_progress_bar.setPosition(10, m_loading_text.getPosition().y + 40);

    setCompletion(0.f);

    m_loading_task.execute();
  }

  bool LoadingState::handleEvent(sf::Event const&)
  {
    return true;
  }

  bool LoadingState::update(sf::Time const&) noexcept
  {
    if (m_loading_task.isFinished())
    {
      requestStackPop();
      requestStackPush(States::ID::GAME);
    } else
    {
      setCompletion(m_loading_task.getCompletion());
    }
    return true;
  }

  void LoadingState::draw()
  {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(m_loading_text);
    window.draw(m_progress_bar_background);
    window.draw(m_progress_bar);
  }

  void LoadingState::setCompletion(float percent) noexcept
  {
    if (percent > 1.f) // clamp
      percent = 1.f;

    m_progress_bar.setSize(sf::Vector2f(m_progress_bar_background.getSize().x * percent, m_progress_bar.getSize().y));
  }
}