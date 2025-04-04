////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "music_player.h"

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
MusicPlayer::MusicPlayer() noexcept
{
  m_filesnames[Music::ID::MENU_THEME] = "../assets/musics/menu_theme.ogg";
  m_filesnames[Music::ID::MISSION_THEME] = "../assets/musics/mission_theme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
  std::string filename = m_filesnames[theme];

  if (!m_music.openFromFile(filename))
    throw std::runtime_error("Music " + filename + " could not be loaded.");

  m_music.setVolume(m_volume);
  m_music.setLoop(true);
  m_music.play();
}

void MusicPlayer::stop() noexcept
{
  m_music.stop();
}

void MusicPlayer::setPaused(bool paused) noexcept
{
  if (paused)
    m_music.pause();
  else
    m_music.play();
}

void MusicPlayer::setVolume(float volume) noexcept
{
  m_volume = volume;
}
} // namespace FastSimDesign