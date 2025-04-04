////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "sound_player.h"

#include "resource_identifiers.h"

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace FastSimDesign {
namespace {
// Sound coordinate system, point of view of a player in front of the screen:
// X = left; Y = up; Z = back (out of the screen)
float const listener_z = 300.f;
float const attenuation = 8.f;
float const min_distance_2d = 200.f;
float const min_distance_3d =
    std::sqrt(min_distance_2d * min_distance_2d + listener_z * listener_z);
} // namespace

////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
SoundPlayer::SoundPlayer()
{
  m_sound_buffers.load(
      SoundEffect::ID::ALLIED_GUN_FIRE,
      "../assets/effects/allied_gunfire.wav");
  m_sound_buffers.load(SoundEffect::ID::BUTTON, "../assets/effects/button.wav");
  m_sound_buffers.load(
      SoundEffect::ID::COLLECT_PICKUP,
      "../assets/effects/collect_pickup.wav");
  m_sound_buffers.load(
      SoundEffect::ID::ENEMY_GUN_FIRE,
      "../assets/effects/enemy_gunfire.wav");
  m_sound_buffers.load(
      SoundEffect::ID::EXPLOSION_1,
      "../assets/effects/explosion_01.wav");
  m_sound_buffers.load(
      SoundEffect::ID::EXPLOSION_2,
      "../assets/effects/explosion_02.wav");
  m_sound_buffers.load(
      SoundEffect::ID::LAUNCH_MISSILE,
      "../assets/effects/launch_missile.wav");

  // Listener points towards the screen (default in SFML).
  sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
  play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
  m_sounds.push_back(sf::Sound{});
  sf::Sound& sound = m_sounds.back();

  sound.setBuffer(m_sound_buffers.get(effect));
  sound.setPosition(position.x, -position.y, 0.f);
  sound.setAttenuation(attenuation);
  sound.setMinDistance(min_distance_3d);

  sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
  m_sounds.remove_if([](sf::Sound const& s) {
    return s.getStatus() == sf::Sound::Stopped;
  });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
  sf::Listener::setPosition(position.x, -position.y, listener_z);
}

sf::Vector2f SoundPlayer::getListenerPosition() const noexcept
{
  sf::Vector3f position = sf::Listener::getPosition();
  return sf::Vector2f{position.x, -position.y};
}

} // namespace FastSimDesign