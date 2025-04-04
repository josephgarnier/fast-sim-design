////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "../core/command_queue.h"
#include "../gui/scene_node.h"
#include "../gui/text_node.h"
#include "../utils/generic_utility.h"
#include "../utils/math_util.h"
#include "../utils/sfml_util.h"
#include "category.h"
#include "core/resource_identifiers.h"
#include "entity_data.h"
#include "pickup.h"
#include "projectile.h"
#include "sound_node.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <glm/trigonometric.hpp>

#include <memory>

namespace FastSimDesign {
namespace {
std::unordered_map<Aircraft::Type, AircraftData> Data_Table =
    initializeAircraftData();
}

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Aircraft::Aircraft(
    Aircraft::Type type, TextureHolder const& textures, FontHolder const& fonts)
  : Parent{Data_Table[type].hit_point}
  , m_type{type}
  , m_sprite{textures.get(Data_Table[type].texture), Data_Table[type].m_texture_rect}
  , m_explosion{textures.get(Textures::ID::EXPLOSION)}
{
  m_explosion.setFrameSize(sf::Vector2i{256, 256});
  m_explosion.setNumFrames(16);
  m_explosion.setDuration(sf::seconds(1));

  SFML::centerOrigin(m_sprite);
  SFML::centerOrigin(m_explosion);

  m_fire_command.name = "CreateBullets";
  m_fire_command.category =
      BitFlags<Category::Type>{Category::Type::SCENE_AIR_LAYER};
  m_fire_command.action = [this, &textures](SceneNode& node, sf::Time) {
    createBullets(node, textures);
  };

  m_missile_command.name = "CreateProjectile";
  m_missile_command.category =
      BitFlags<Category::Type>{Category::Type::SCENE_AIR_LAYER};
  m_missile_command.action = [this, &textures](SceneNode& node, sf::Time) {
    createProjectile(node, Projectile::Type::MISSILE, 0.f, 0.5f, textures);
  };

  m_drop_pickup_command.name = "DropPickup";
  m_drop_pickup_command.category =
      BitFlags<Category::Type>{Category::Type::SCENE_AIR_LAYER};
  m_drop_pickup_command.action = [this, &textures](SceneNode& node, sf::Time) {
    createPickup(node, textures);
  };

  std::unique_ptr<TextNode> health_display =
      std::make_unique<TextNode>(fonts, "");
  m_health_display = health_display.get();
  attachChild(std::move(health_display));

  if (getCategory() ==
      BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT})
  {
    std::unique_ptr<TextNode> missile_display =
        std::make_unique<TextNode>(fonts, "");
    missile_display->setPosition(0, 70.f);
    m_missile_display = missile_display.get();
    attachChild(std::move(missile_display));
  }

  updateDisplayedTexts();
}

void Aircraft::createBullets(
    SceneNode& node, TextureHolder const& textures) const noexcept
{
  Projectile::Type type = isAllied() ? Projectile::Type::ALLIED_BULLET
                                     : Projectile::Type::ENEMY_BULLET;

  switch (m_spread_level)
  {
    case 1:
      createProjectile(node, type, 0.0f, 0.5f, textures);
      break;
    case 2:
      createProjectile(node, type, -0.33f, 0.33f, textures);
      createProjectile(node, type, +0.33f, 0.33f, textures);
      break;
    case 3:
      createProjectile(node, type, -0.5f, 0.33f, textures);
      createProjectile(node, type, 0.0f, 0.5f, textures);
      createProjectile(node, type, +0.5f, 0.33f, textures);
      break;
  }
}

void Aircraft::createProjectile(
    SceneNode& node,
    Projectile::Type type,
    float x_offset,
    float y_offset,
    TextureHolder const& textures) const noexcept
{
  std::unique_ptr<Projectile> projectile =
      std::make_unique<Projectile>(type, textures);

  sf::Vector2f offset{
      x_offset * m_sprite.getGlobalBounds().width,
      y_offset * m_sprite.getGlobalBounds().height};
  sf::Vector2f velocity{0, projectile->getMaxSpeed()};

  float sign = isAllied() ? -1.f : 1.f;
  projectile->setPosition(getWorldPosition() + offset * sign);
  projectile->setVelocity(velocity * sign);
  node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(
    SceneNode& node, TextureHolder const& textures) const noexcept
{
  auto type = static_cast<Pickup::Type>(
      Math::randomInt(toUnderlyingType(Pickup::Type::TYPE_COUNT)));

  std::unique_ptr<Pickup> pickup = std::make_unique<Pickup>(type, textures);
  pickup->setPosition(getWorldPosition());
  pickup->setVelocity(0.f, 1.f);
  node.attachChild(std::move(pickup));
}

void Aircraft::updateDisplayedTexts() noexcept
{
  m_health_display->setString(std::to_string(getHitpoints()) + "HP");
  m_health_display->setPosition(0.f, 50.f);
  m_health_display->setRotation(-getRotation());

  if (m_missile_display)
  {
    if (m_missile_ammo == 0)
      m_missile_display->setString("");
    else
      m_missile_display->setString("M: " + std::to_string(m_missile_ammo));
  }
}

void Aircraft::updateRollAnimation() noexcept
{
  if (Data_Table[m_type].has_rool_animation)
  {
    sf::IntRect texture_rect = Data_Table[m_type].m_texture_rect;

    // Roll left: Texture rect offset once.
    if (getVelocity().x < 0.f)
      texture_rect.left += texture_rect.width;

    // Roll right: Texture rect offset twice.
    else if (getVelocity().x > 0.f)
      texture_rect.left += 2 * texture_rect.width;

    m_sprite.setTextureRect(texture_rect);
  }
}

BitFlags<Category::Type> Aircraft::getCategory() const noexcept
{
  if (isAllied())
    return BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT};
  else
    return BitFlags<Category::Type>{Category::Type::ENEMY_AIRCRAFT};
}

sf::FloatRect Aircraft::getBoundingRect() const noexcept
{
  return getWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const noexcept
{
  return isDestroyed() && (m_explosion.isFinished() || !m_show_explosion);
}

float Aircraft::getMaxSpeed() const noexcept
{
  return Data_Table[m_type].speed;
}

bool Aircraft::isAllied() const noexcept
{
  return m_type == Aircraft::Type::EAGLE;
}

void Aircraft::increaseFireRate() noexcept
{
  if (m_fire_rate_level < 10)
    ++m_fire_rate_level;
}

void Aircraft::increaseSpread() noexcept
{
  if (m_spread_level < 3)
    ++m_spread_level;
}

void Aircraft::collectMissiles(uint16_t count) noexcept
{
  m_missile_ammo += count;
}

void Aircraft::fire() noexcept
{
  // Only ships with fire interval != 0 are able to fire.
  if (Data_Table[m_type].fire_interval != sf::Time::Zero)
    m_is_firing = true;
}

void Aircraft::launchMissile() noexcept
{
  if (m_missile_ammo > 0)
  {
    m_is_launching_missile = true;
    --m_missile_ammo;
  }
}

void Aircraft::playLocalSound(
    CommandQueue& commands, SoundEffect::ID effect) noexcept
{
  sf::Vector2f world_position = getWorldPosition();

  Command command;
  command.name = "PlaySound";
  command.category = BitFlags<Category::Type>{Category::Type::SOUND_EFFECT};
  command.action = derivedAction<SoundNode>(
      [effect, world_position](SoundNode& node, sf::Time) {
        node.playSound(effect, world_position);
      });

  commands.push(command);
}

void Aircraft::updateCurrent(sf::Time const& dt, CommandQueue& commands)
{
  // Update texts.
  updateDisplayedTexts();
  updateRollAnimation();

  // Entity has been destroyed: Possibly drop pickup, mark for removal.
  if (isDestroyed())
  {
    checkPickupDrop(commands);
    m_explosion.update(dt);

    // Play explosion sound only once.
    if (!m_played_explosion_sound)
    {
      SoundEffect::ID sound_effect = (Math::randomInt(2) == 0)
                                         ? SoundEffect::ID::EXPLOSION_1
                                         : SoundEffect::ID::EXPLOSION_2;
      playLocalSound(commands, sound_effect);

      m_played_explosion_sound = true;
    }
    return;
  }

  // Check if bullets or missiles are fired.
  checkProjectileLaunch(dt, commands);

  // Update enemy movement pattern; apply velocity.
  updateMovementPattern(dt);
  Parent::updateCurrent(dt, commands);
}

void Aircraft::updateMovementPattern(sf::Time const& dt) noexcept
{
  // Enemy airplane: Movement pattern
  const std::vector<Direction>& directions = Data_Table[m_type].directions;
  if (!directions.empty())
  {
    // Moved long enough in current direction: Change direction.
    if (m_travelled_distance > directions[m_direction_index].distance)
    {
      m_direction_index = (m_direction_index + 1) % directions.size();
      m_travelled_distance = 0.f;
    }

    // Compute velocity from direction.
    float radians = glm::radians(directions[m_direction_index].angle + 90.f);
    float vx = getMaxSpeed() * std::cos(radians);
    float vy = getMaxSpeed() * std::sin(radians);

    setVelocity(vx, vy);

    m_travelled_distance += getMaxSpeed() * dt.asSeconds();
  }
}

void Aircraft::checkPickupDrop(CommandQueue& commands) noexcept
{
  if (!isAllied() && Math::randomInt(3) == 0 && !m_spawned_pickup)
    commands.push(m_drop_pickup_command);

  m_spawned_pickup = true;
}

void Aircraft::checkProjectileLaunch(
    sf::Time const& dt, CommandQueue& commands) noexcept
{
  // Enemies try to fire all the time.
  if (!isAllied())
    fire();

  // Check for automatic gunfire, allow only in intervals.
  if (m_is_firing && m_fire_countdown <= sf::Time::Zero)
  {
    // Interval expired: We can fire a new bullet.
    commands.push(m_fire_command);
    playLocalSound(
        commands,
        isAllied() ? SoundEffect::ID::ALLIED_GUN_FIRE
                   : SoundEffect::ID::ENEMY_GUN_FIRE);

    m_fire_countdown += Data_Table[m_type].fire_interval /
                        (static_cast<float>(m_fire_rate_level) + 1.f);
    m_is_firing = false;
  }
  else if (m_fire_countdown > sf::Time::Zero)
  {
    // Interval not expired: Decrease it further.
    m_fire_countdown -= dt;
    m_is_firing = false;
  }

  // Check for missile launch.
  if (m_is_launching_missile)
  {
    commands.push(m_missile_command);
    playLocalSound(commands, SoundEffect::ID::LAUNCH_MISSILE);

    m_is_launching_missile = false;
  }
}

void Aircraft::drawCurrent(
    sf::RenderTarget& target, sf::RenderStates states) const
{
  if (isDestroyed() && m_show_explosion)
    target.draw(m_explosion, states);
  else
    target.draw(m_sprite, states);
}
} // namespace FastSimDesign