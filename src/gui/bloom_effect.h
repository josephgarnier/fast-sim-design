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

#ifndef FAST_SIM_DESIGN_BLOOM_EFFECT_H
#define FAST_SIM_DESIGN_BLOOM_EFFECT_H

#include "post_effect.h"
#include "../core/resource_identifiers.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <array>

namespace FastSimDesign {
  class BloomEffet : public PostEffect
  {
  private:
    using Parent = PostEffect;
    using RenderTextureArray = std::array<sf::RenderTexture, 2>;

  public:
    explicit BloomEffet() noexcept; // Default constructor
    virtual ~BloomEffet() = default; // Destructor

    virtual void apply(sf::RenderTexture const& input, sf::RenderTarget& output) override;

  private:
    void prepareTextures(sf::Vector2u const& size);

    void filterBright(sf::RenderTexture const& input, sf::RenderTexture& output);
    void blurMultipass(RenderTextureArray& render_textures);
    void blur(sf::RenderTexture const& input, sf::RenderTexture& output, sf::Vector2f const& offset_factor);
    void downsample(sf::RenderTexture const& input, sf::RenderTexture& output);
    void add(sf::RenderTexture const& source, sf::RenderTexture const& bloom, sf::RenderTarget& target);

  private:
    ShaderHolder m_shaders{};

    sf::RenderTexture m_brightness_texture{};
    RenderTextureArray m_first_pass_textures{};
    RenderTextureArray m_second_pass_textures{};
  };
}
#endif