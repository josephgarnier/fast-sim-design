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

#ifndef FAST_SIM_DESIGN_COMPONENT_H
#define FAST_SIM_DESIGN_COMPONENT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace FastSimDesign {
namespace GUI {
class Component
  : public sf::Drawable
  , public sf::Transformable
  , public sf::NonCopyable
{
public:
  using Ptr = std::shared_ptr<Component>;

public:
  explicit Component() = default;
  virtual ~Component() = default;

  virtual bool isSelectable() const noexcept = 0;
  bool isSelected() const noexcept;
  virtual void select() noexcept;
  virtual void deselect() noexcept;

  virtual bool isActive() const noexcept;
  virtual void activate() noexcept;
  virtual void deactivate() noexcept;

  virtual void handleEvent(sf::Event const& event) = 0;

private:
  bool m_is_selected{false};
  bool m_is_active{false};
};
} // namespace GUI
} // namespace FastSimDesign
#endif