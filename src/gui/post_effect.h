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

#ifndef FAST_SIM_DESIGN_POST_EFFECT_H
#define FAST_SIM_DESIGN_POST_EFFECT_H

#include <SFML/System/NonCopyable.hpp>

namespace sf {
  class Shader;
  class RenderTarget;
  class RenderTexture;
}

namespace FastSimDesign {
  class PostEffect : public sf::NonCopyable
  {
  public:
    static bool isSupported() noexcept;

  protected:
    static void applyShader(sf::Shader const& shader, sf::RenderTarget& output) noexcept;

  private:
  public:
    explicit PostEffect() = default; // Default constructor
    virtual ~PostEffect() = default; // Destructor

    virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
  };
}
#endif