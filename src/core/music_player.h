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

#ifndef FAST_SIM_DESIGN_MUSIC_PLAYER_H
#define FAST_SIM_DESIGN_MUSIC_PLAYER_H

#include "resource_identifiers.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <map>
#include <string>

namespace FastSimDesign {
class MusicPlayer : private sf::NonCopyable
{
public:
  explicit MusicPlayer() noexcept;
  virtual ~MusicPlayer() = default;

  void play(Music::ID theme);
  void stop() noexcept;

  void setPaused(bool paused) noexcept;
  void setVolume(float volume) noexcept;

private:
  sf::Music m_music{};
  std::map<Music::ID, std::string> m_filesnames{};
  float m_volume{100.f};
};
} // namespace FastSimDesign
#endif