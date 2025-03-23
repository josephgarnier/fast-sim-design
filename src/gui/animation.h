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
#ifndef FAST_SIM_DESIGN_ANIMATION_H
#define FAST_SIM_DESIGN_ANIMATION_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstddef>

namespace FastSimDesign {
  class Animation final : public sf::Drawable
    , public sf::Transformable
  {
  public:
    explicit Animation() = default; // Default constructor
    explicit Animation(sf::Texture const& texture) noexcept; // Default constructor
    Animation(Animation const&) = default; // Copy constructor
    Animation(Animation&&) = default; // Move constructor
    Animation& operator=(Animation const&) = default; // Copy assignment operator
    Animation& operator=(Animation&&) = default; // Move assignment operator
    virtual ~Animation() = default; // Destructor

    void setTexture(sf::Texture const& texture);
    sf::Texture const* getTexture() const noexcept;

    void setFrameSize(sf::Vector2i frame_size) noexcept;
    sf::Vector2i const & getFrameSize() const noexcept;

    void setNumFrames(std::size_t num_frames) noexcept;
    std::size_t getNumFrames() const noexcept;

    void setDuration(sf::Time duration) noexcept;
    sf::Time const & getDuration() const noexcept;

    void setRepeating(bool flag) noexcept;
    bool isRepeat() const noexcept;

    void restart() noexcept;
    bool isFinished() const noexcept;

    sf::FloatRect getLocalBounds() const noexcept;
    sf::FloatRect getGlobalBounds() const noexcept;

    void update(sf::Time const& dt);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    sf::Sprite m_sprite;
    sf::Vector2i m_frame_size;
    std::size_t m_num_frames{0};
    std::size_t m_current_frame{0};
    sf::Time m_duration{sf::Time::Zero};
    sf::Time m_elapsed_time{sf::Time::Zero};
    bool m_repeat{false};
  };
}
#endif