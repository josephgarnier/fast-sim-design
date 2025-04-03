////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "bloom_effect.h"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
BloomEffet::BloomEffet() noexcept
  : Parent{}
{
  m_shaders.load(
      Shaders::ID::ADD_PASS,
      "../assets/shaders/fullpass.vert",
      "../assets/shaders/add.frag");
  m_shaders.load(
      Shaders::ID::BRIGHTNESS_PASS,
      "../assets/shaders/fullpass.vert",
      "../assets/shaders/brightness.frag");
  m_shaders.load(
      Shaders::ID::DOWN_SAMPLE_PASS,
      "../assets/shaders/fullpass.vert",
      "../assets/shaders/down_sample.frag");
  m_shaders.load(
      Shaders::ID::GAUSSIAN_BLUR_PASS,
      "../assets/shaders/fullpass.vert",
      "../assets/shaders/guassian_blur.frag");
}

void BloomEffet::apply(sf::RenderTexture const& input, sf::RenderTarget& output)
{
  prepareTextures(input.getSize());

  filterBright(input, m_brightness_texture);

  downsample(m_brightness_texture, m_first_pass_textures[0]);
  blurMultipass(m_first_pass_textures);

  downsample(m_first_pass_textures[0], m_second_pass_textures[0]);
  blurMultipass(m_second_pass_textures);

  add(m_first_pass_textures[0],
      m_second_pass_textures[0],
      m_first_pass_textures[1]);
  m_first_pass_textures[1].display();
  add(input, m_first_pass_textures[1], output);
}

void BloomEffet::prepareTextures(sf::Vector2u const& size)
{
  if (m_brightness_texture.getSize() != size)
  {
    m_brightness_texture.create(size.x, size.y);
    m_brightness_texture.setSmooth(true);

    m_first_pass_textures[0].create(size.x / 2, size.y / 2);
    m_first_pass_textures[0].setSmooth(true);
    m_first_pass_textures[1].create(size.x / 2, size.y / 2);
    m_first_pass_textures[1].setSmooth(true);

    m_second_pass_textures[0].create(size.x / 4, size.y / 4);
    m_second_pass_textures[0].setSmooth(true);
    m_second_pass_textures[1].create(size.x / 4, size.y / 4);
    m_second_pass_textures[1].setSmooth(true);
  }
}

void BloomEffet::filterBright(
    sf::RenderTexture const& input, sf::RenderTexture& output)
{
  sf::Shader& brightness = m_shaders.get(Shaders::ID::BRIGHTNESS_PASS);

  brightness.setUniform("source", input.getTexture());
  applyShader(brightness, output);
  output.display();
}

void BloomEffet::blurMultipass(RenderTextureArray& render_textures)
{
  sf::Vector2u texture_size = render_textures[0].getSize();

  for (std::size_t count = 0; count < 2; ++count)
  {
    blur(
        render_textures[0],
        render_textures[1],
        sf::Vector2f{0.f, 1.f / static_cast<float>(texture_size.y)});
    blur(
        render_textures[1],
        render_textures[0],
        sf::Vector2f{1.f / static_cast<float>(texture_size.x), 0.f});
  }
}

void BloomEffet::blur(
    sf::RenderTexture const& input,
    sf::RenderTexture& output,
    sf::Vector2f const& offset_factor)
{
  sf::Shader& gaussian_Blur = m_shaders.get(Shaders::ID::GAUSSIAN_BLUR_PASS);

  gaussian_Blur.setUniform("source", input.getTexture());
  gaussian_Blur.setUniform("offsetFactor", offset_factor);
  applyShader(gaussian_Blur, output);
  output.display();
}

void BloomEffet::downsample(
    sf::RenderTexture const& input, sf::RenderTexture& output)
{
  sf::Shader& down_sampler = m_shaders.get(Shaders::ID::DOWN_SAMPLE_PASS);

  down_sampler.setUniform("source", input.getTexture());
  down_sampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
  applyShader(down_sampler, output);
  output.display();
}

void BloomEffet::add(
    sf::RenderTexture const& source,
    sf::RenderTexture const& bloom,
    sf::RenderTarget& target)
{
  sf::Shader& adder = m_shaders.get(Shaders::ID::ADD_PASS);

  adder.setUniform("source", source.getTexture());
  adder.setUniform("bloom", bloom.getTexture());
  applyShader(adder, target);
}
} // namespace FastSimDesign