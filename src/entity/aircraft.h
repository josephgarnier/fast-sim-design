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

#ifndef FAST_SIM_DESIGN_AIRCRAFT_H
#define FAST_SIM_DESIGN_AIRCRAFT_H

#include "../core/command.h"
#include "../core/resource_identifiers.h"
#include "../gui/animation.h"
#include "projectile.h"

#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
class TextNode;
class Aircraft final : public Entity
{
public:
  enum class Type : uint16_t
  {
    EAGLE,
    RAPTOR,
    AVENGER,
    TYPE_COUNT
  };

private:
  using Parent = Entity;

public:
  explicit Aircraft(
      Aircraft::Type type,
      TextureHolder const& textures,
      FontHolder const& fonts);
  virtual ~Aircraft() = default;

  virtual BitFlags<Category::Type> getCategory() const noexcept override;
  virtual sf::FloatRect getBoundingRect() const noexcept override;
  virtual bool isMarkedForRemoval() const noexcept override;
  float getMaxSpeed() const noexcept;
  bool isAllied() const noexcept;

  void increaseFireRate() noexcept;
  void increaseSpread() noexcept;
  void collectMissiles(uint16_t count) noexcept;

  void fire() noexcept;
  void launchMissile() noexcept;
  void playLocalSound(CommandQueue& commands, SoundEffect::ID effect) noexcept;

private:
  void createBullets(
      SceneNode& node, TextureHolder const& textures) const noexcept;
  void createProjectile(
      SceneNode& node,
      Projectile::Type type,
      float x_offset,
      float y_offset,
      TextureHolder const& textures) const noexcept;
  void createPickup(
      SceneNode& node, TextureHolder const& textures) const noexcept;

  virtual void updateCurrent(
      sf::Time const& dt, CommandQueue& commands) override;
  void updateMovementPattern(sf::Time const& dt) noexcept;
  void checkPickupDrop(CommandQueue& commands) noexcept;
  void checkProjectileLaunch(
      sf::Time const& dt, CommandQueue& commands) noexcept;
  void updateDisplayedTexts() noexcept;
  void updateRollAnimation() noexcept;

  virtual void drawCurrent(
      sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  Aircraft::Type m_type{};
  sf::Sprite m_sprite{};
  Animation m_explosion{};

  Command m_fire_command{};
  Command m_missile_command{};
  Command m_drop_pickup_command{};
  sf::Time m_fire_countdown{sf::Time::Zero};
  bool m_is_firing{false};
  bool m_is_launching_missile{false};
  bool m_show_explosion{true};
  bool m_played_explosion_sound{false};
  bool m_spawned_pickup{false};

  int m_fire_rate_level{1};
  int m_spread_level{1};
  int m_missile_ammo{2};

  float m_travelled_distance{0.f};
  std::size_t m_direction_index{0};

  TextNode* m_health_display{nullptr};
  TextNode* m_missile_display{nullptr};
};
} // namespace FastSimDesign
#endif
