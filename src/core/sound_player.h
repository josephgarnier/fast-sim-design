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

#ifndef FAST_SIM_DESIGN_SOUND_PLAYER_H
#define FAST_SIM_DESIGN_SOUND_PLAYER_H

#include "resource_identifiers.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

#include <list>

namespace FastSimDesign {
class SoundPlayer : private sf::NonCopyable
{
public:
  explicit SoundPlayer();
  virtual ~SoundPlayer() = default;

  void play(SoundEffect::ID effect);
  void play(SoundEffect::ID effect, sf::Vector2f position);

  void removeStoppedSounds();
  void setListenerPosition(sf::Vector2f position);
  sf::Vector2f getListenerPosition() const noexcept;

private:
  SoundBufferHolder m_sound_buffers{};
  std::list<sf::Sound> m_sounds{};
};
} // namespace FastSimDesign
#endif