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
#ifndef FAST_SIM_DESIGN_LABEL_H
#define FAST_SIM_DESIGN_LABEL_H

#include "component.h"
#include "../core/resource_identifiers.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>

namespace FastSimDesign {
  namespace GUI {
    class Label : public Component
    {
    public:
      using Ptr = std::shared_ptr<Label>;
    private:
      using Parent = Component;

    public:
      explicit Label(std::string text, FontHolder const& fonts) noexcept; // Default constructor
      virtual ~Label() = default; // Destructor

      virtual bool isSelectable() const noexcept override;
      void setText(std::string const& text) noexcept;
      virtual void handleEvent(sf::Event const& event) noexcept override;

    private:
      virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
      sf::Text m_text;
    };
  }
}
#endif