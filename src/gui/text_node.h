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

#ifndef FAST_SIM_DESIGN_TEXT_NODE_H
#define FAST_SIM_DESIGN_TEXT_NODE_H

#include "../core/resource_identifiers.h"
#include "scene_node.h"

#include <SFML/Graphics/Text.hpp>

namespace FastSimDesign {
  class TextNode : public SceneNode
  {
  private:
    using Parent = SceneNode;

  public:
    explicit TextNode(FontHolder const& fonts, std::string text) noexcept; // Default constructor
    virtual ~TextNode() = default; // Destructor

    void setString(std::string text) noexcept;

  private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    sf::Text m_text;
  };
}
#endif