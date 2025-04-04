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

#include "../state_machine/state.h"
#include "component.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cstdint>
#include <functional>
#include <memory>

namespace FastSimDesign {
class SoundPlayer;
namespace GUI {
class Button final : public Component
{
public:
  enum class Type : uint16_t
  {
    NORMAL,
    SELECTED,
    PRESSED,
    TYPE_COUNT,
  };

public:
  using Ptr = std::shared_ptr<Button>;

private:
  using Parent = Component;
  using Callback = std::function<void()>;

public:
  explicit Button(State::Context context);
  virtual ~Button() = default;

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
  virtual void draw(
      sf::RenderTarget& target, sf::RenderStates states) const override;
  void changeTexture(Button::Type button_type) noexcept;

private:
  Button::Callback m_callback{};
  sf::Sprite m_sprite{};
  sf::Text m_text{};
  bool m_is_toggle{false};
  SoundPlayer* m_sounds{nullptr};
};
} // namespace GUI
} // namespace FastSimDesign
#endif