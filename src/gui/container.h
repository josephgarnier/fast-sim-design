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

#ifndef FAST_SIM_DESIGN_CONTAINER_H
#define FAST_SIM_DESIGN_CONTAINER_H

#include "component.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cstddef>
#include <vector>

namespace FastSimDesign {
  namespace GUI {
    class Container : public Component
    {
    private:
      using Parent = Component;
      using Parent::select;

    public:
      explicit Container() noexcept; // Default constructor
      virtual ~Container() = default; // Destructor

      void pack(Component::Ptr component);

      virtual bool isSelectable() const noexcept override;
      virtual void handleEvent(sf::Event const& event) override;

    private:
      virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

      bool hasSelection() const noexcept;
      void select(std::size_t index) noexcept;
      void selectNext() noexcept;
      void selectPrevious() noexcept;

    private:
      std::vector<Component::Ptr> m_children;
      int m_selected_child;
    };
  }
}
#endif