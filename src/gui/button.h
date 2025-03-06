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

#ifndef FAST_SIM_DESIGN_BUTTON_H
#define FAST_SIM_DESIGN_BUTTON_H

#include "component.h"
#include "../core/resource_identifiers.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <functional>
#include <memory>

namespace FastSimDesign {
  namespace GUI {
    class Button : public Component
    {
    public:
      using Ptr = std::shared_ptr<Button>;

    private:
      using Parent = Component;
      using Callback = std::function<void()>;

    public:
      explicit Button(FontHolder const& fonts, TextureHolder const& textures); // Default constructor
      virtual ~Button() = default; // Destructor

      void setCallback(Button::Callback callback) noexcept;
      void setText(std::string const& text) noexcept;
      void setToggle(bool flag) noexcept;

      virtual bool isSelectable() const noexcept override;
      virtual void select() noexcept override;
      virtual void deselect() noexcept override;

      virtual void activate() noexcept override;
      virtual void deactivate() noexcept override;

      virtual void handleEvent(sf::Event const& event) noexcept override;

    private:
      virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
      Button::Callback m_callback;
      sf::Texture const& m_normal_texture;
      sf::Texture const& m_selected_texture;
      sf::Texture const& m_pressed_texture;
      sf::Sprite m_sprite;
      sf::Text m_text;
      bool m_is_toggle;
    };
  }
}
#endif