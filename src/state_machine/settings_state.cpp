////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "settings_state.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  SettingsState::SettingsState(StateStack* stack, Context context)
    : Parent{stack, context, "SETTINGS"}
    , m_background_sprite{}
    , m_gui_container{}
    , m_binding_buttons{}
    , m_binding_labels{}
  {
    sf::Texture const & texture = context.textures->get(Textures::ID::TITLE_SCREEN);
    m_background_sprite.setTexture(texture);

    // Build key binding buttons and labels.
    addButtonLabel(Player::Action::MOVE_LEFT, 150.f, "Move Left", context);
    addButtonLabel(Player::Action::MOVE_RIGHT, 200.f, "Move Right", context);
    addButtonLabel(Player::Action::MOVE_UP, 250.f, "Move Up", context);
    addButtonLabel(Player::Action::MOVE_DOWN, 300.f, "Move Down", context);

    updateLabels();

    auto back_button = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    back_button->setPosition(80.f, 375.f);
    back_button->setText("Back");
    back_button->setCallback([this]() {
      requestStackPop();
    });

    m_gui_container.pack(back_button);
  }

  bool SettingsState::handleEvent(sf::Event const& event)
  {
    bool is_key_binding = false;

    // Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key.
    for (std::size_t action = 0; action < toUnderlyingType(Player::Action::ACTION_COUNT); ++action)
    {
      if (m_binding_buttons[action]->isActive())
      {
        is_key_binding = true;
        if (event.type == sf::Event::KeyReleased)
        {
          getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
          m_binding_buttons[action]->deactivate();
        }
        break;
      }
    }

    // If pressed button changed key bindings, update labels; otherwise consider other buttons in container.
    if (is_key_binding)
      updateLabels();
    else
      m_gui_container.handleEvent(event);

    return false;
  }

  bool SettingsState::update(sf::Time const&)
  {
    return true;
  }

  void SettingsState::draw()
  {
    sf::RenderWindow& window = *getContext().window;

    window.draw(m_background_sprite);
    window.draw(m_gui_container);
  }

  void SettingsState::updateLabels() noexcept
  {
    Player& player = *getContext().player;

    for (std::size_t action = 0; action < toUnderlyingType(Player::Action::ACTION_COUNT); ++action)
    {
      sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(action));
      m_binding_labels[action]->setText(SFML::toString(key));
    }
  }

  void SettingsState::addButtonLabel(Player::Action action, float y, std::string text, Context context) noexcept
  {
    m_binding_buttons[toUnderlyingType(action)] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    m_binding_buttons[toUnderlyingType(action)]->setPosition(80.f, y);
    m_binding_buttons[toUnderlyingType(action)]->setText(std::move(text));
    m_binding_buttons[toUnderlyingType(action)]->setToggle(true);

    m_binding_labels[toUnderlyingType(action)] = std::make_shared<GUI::Label>("TEST", *context.fonts);
    m_binding_labels[toUnderlyingType(action)]->setPosition(300.f, y + 15.f);

    m_gui_container.pack(m_binding_buttons[toUnderlyingType(action)]);
    m_gui_container.pack(m_binding_labels[toUnderlyingType(action)]);
  }

}