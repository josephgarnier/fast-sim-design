////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "animation.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Animation::Animation(sf::Texture const& texture) noexcept
  : m_sprite{texture}
{
}

void Animation::setTexture(sf::Texture const& texture)
{
  m_sprite.setTexture(texture);
}

sf::Texture const* Animation::getTexture() const noexcept
{
  return m_sprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frame_size) noexcept
{
  m_frame_size = std::move(frame_size);
}

sf::Vector2i const& Animation::getFrameSize() const noexcept
{
  return m_frame_size;
}

void Animation::setNumFrames(std::size_t num_frames) noexcept
{
  m_num_frames = num_frames;
}

std::size_t Animation::getNumFrames() const noexcept
{
  return m_num_frames;
}

void Animation::setDuration(sf::Time duration) noexcept
{
  m_duration = std::move(duration);
}

sf::Time const& Animation::getDuration() const noexcept
{
  return m_duration;
}

void Animation::setRepeating(bool flag) noexcept
{
  m_repeat = flag;
}

bool Animation::isRepeat() const noexcept
{
  return m_repeat;
}

void Animation::restart() noexcept
{
  m_current_frame = 0;
}

bool Animation::isFinished() const noexcept
{
  return m_current_frame >= m_num_frames;
}

sf::FloatRect Animation::getLocalBounds() const noexcept
{
  return sf::FloatRect{getOrigin(), static_cast<sf::Vector2f>(getFrameSize())};
}

sf::FloatRect Animation::getGlobalBounds() const noexcept
{
  return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time const& dt)
{
  sf::Time time_per_frame = m_duration / static_cast<float>(m_num_frames);
  m_elapsed_time += dt;

  sf::Vector2i texture_bounds{m_sprite.getTexture()->getSize()};
  sf::IntRect texture_rect = m_sprite.getTextureRect();

  if (m_current_frame == 0)
    texture_rect = sf::IntRect{0, 0, m_frame_size.x, m_frame_size.y};

  // While we have a frame to process.
  while (m_elapsed_time >= time_per_frame &&
         (m_current_frame <= m_num_frames || m_repeat))
  {
    // Move the texture rect left.
    texture_rect.left += texture_rect.width;

    // If we reach the end of the texture.
    if (texture_rect.left + texture_rect.width > texture_bounds.x)
    {
      // Move it down one line.
      texture_rect.left = 0;
      texture_rect.top += texture_rect.height;
    }

    // And progress to next frame.
    m_elapsed_time -= time_per_frame;
    if (m_repeat)
    {
      m_current_frame = (m_current_frame + 1) % m_num_frames;
      if (m_current_frame == 0)
        texture_rect = sf::IntRect{0, 0, m_frame_size.x, m_frame_size.y};
    }
    else
    {
      ++m_current_frame;
    }
  }

  m_sprite.setTextureRect(texture_rect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(m_sprite, states);
}

} // namespace FastSimDesign