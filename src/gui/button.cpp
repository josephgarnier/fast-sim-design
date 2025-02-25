////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "button.h"
#include "../core/resource_identifiers.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/Rect.hpp>

namespace FastSimDesign {
  namespace GUI {
    ////////////////////////////////////////////////////////////
    /// Statics
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// Methods
    ////////////////////////////////////////////////////////////
    Button::Button(FontHolder const& fonts, TextureHolder const& textures)
      : m_callback{}
      , m_normal_texture{textures.get(Textures::ID::BUTTON_NORMAL)}
      , m_selected_texture{textures.get(Textures::ID::BUTTON_SELECTED)}
      , m_pressed_texture{textures.get(Textures::ID::BUTTON_PRESSED)}
      , m_sprite{}
      , m_text{"", fonts.get(Fonts::ID::MAIN), 16}
      , m_is_toggle{false}
    {
      m_sprite.setTexture(m_normal_texture);

      sf::FloatRect bounds = m_sprite.getLocalBounds();
      m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(Button::Callback callback) noexcept
    {
      m_callback = std::move(callback);
    }

    void Button::setText(std::string const& text) noexcept
    {
      m_text.setString(text);
      SFML::centerOrigin(m_text);
    }

    void Button::setToggle(bool flag) noexcept
    {
      m_is_toggle = flag;
    }

    bool Button::isSelectable() const noexcept
    {
      return true;
    }

    void Button::select() noexcept
    {
      Parent::select();
      m_sprite.setTexture(m_selected_texture);
    }

    void Button::deselect() noexcept
    {
      Parent::deselect();
      m_sprite.setTexture(m_normal_texture);
    }

    void Button::activate() noexcept
    {
      Parent::activate();

      // If we are toggle then we should show that the button is pressed and thus "toggled".
      if (m_is_toggle)
        m_sprite.setTexture(m_pressed_texture);

      if (m_callback)
        m_callback();

      // If we are not a toggle then deactivate the button since we are just momentarily activated.
      if (!m_is_toggle)
        desactivate();
    }

    void Button::desactivate() noexcept
    {
      Parent::desactivate();
      if (m_is_toggle)
      {
        // Reset texture to right one depending on if we are selected or not.
        if (isSelected())
          m_sprite.setTexture(m_selected_texture);
        else
          m_sprite.setTexture(m_normal_texture);
      }
    }

    void Button::handleEvent(sf::Event const&) noexcept
    {
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
      states.transform *= getTransform();
      target.draw(m_sprite, states);
      target.draw(m_text, states);
    }
  }
}