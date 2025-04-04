////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "world.h"

#include "../entity/aircraft.h"
#include "../entity/category.h"
#include "../entity/particle_node.h"
#include "../entity/pickup.h"
#include "../entity/sound_node.h"
#include "../gui/sprite_node.h"
#include "../monitor/frame.h"
#include "../monitor/monitor.h"
#include "../monitor/window/scene_graph_window.h"
#include "../utils/generic_utility.h"
#include "command.h"
#include "gui/post_effect.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <cstddef>
#include <memory>
#include <utility>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// SpawnPoint::Methods
////////////////////////////////////////////////////////////
World::SpawnPoint::SpawnPoint(Aircraft::Type type_, float x_, float y_) noexcept
  : type{type_}
  , x{x_}
  , y{y_}
{
}

////////////////////////////////////////////////////////////
/// World::Methods
////////////////////////////////////////////////////////////
World::World(
    SimMonitor::Monitor& monitor,
    sf::RenderWindow& windows,
    FontHolder& fonts,
    SoundPlayer& sounds)
  : SimMonitor::Monitorable{}
  , m_window{windows}
  , m_world_view{m_window.getDefaultView()}
  , m_fonts{fonts}
  , m_sounds(sounds)
  , m_monitor{monitor}
  , m_world_bounds{0.f, 0.f, m_world_view.getSize().x, 5000.f}
  , m_spawn_position{
        m_world_view.getSize().x / 2.f,
        m_world_bounds.height - m_world_view.getSize().y / 2.f}
{
  m_scene_texture.create(m_window.getSize().x, m_window.getSize().y);

  loadTextures();
  buildScene();

  // Prepare the view.
  m_world_view.setCenter(m_spawn_position);

  // Set model to monitor view.
  m_monitor
      .getWindow<SimMonitor::SceneGraphWindow>(
          SimMonitor::Window::ID::SCENE_GRAPH)
      .setDataModel(&m_scene_graph);
}

World::~World()
{
  m_monitor
      .getWindow<SimMonitor::SceneGraphWindow>(
          SimMonitor::Window::ID::SCENE_GRAPH)
      .unsetDataModel();
}

void World::loadTextures()
{
  m_textures.load(
      Textures::ID::ENTITIES,
      "../assets/sprites/npcs/entities.png");
  m_textures.load(Textures::ID::JUNGLE, "../assets/sprites/npcs/jungle.png");
  m_textures.load(
      Textures::ID::EXPLOSION,
      "../assets/sprites/npcs/explosion.png");
  m_textures.load(Textures::ID::PARTICLE, "../assets/particles/smoke.png");
  m_textures.load(
      Textures::ID::FINISH_LINE,
      "../assets/sprites/npcs/finish_line.png");
}

void World::buildScene()
{
  // Intilialize the different layers.
  for (std::size_t i = 0;
       i < static_cast<std::size_t>(World::Layer::LAYER_COUNT);
       ++i)
  {
    Category::Type category = (i == toUnderlyingType(World::Layer::LOWER_AIR))
                                  ? Category::Type::SCENE_AIR_LAYER
                                  : Category::Type::NONE;

    SceneNode::Ptr layer = std::make_unique<SceneNode>(category);
    m_scene_layers[i] = layer.get();

    m_scene_graph.attachChild(std::move(layer));
  }

  // Prepare the tiled background.
  sf::Texture& jungle_texture = m_textures.get(Textures::ID::JUNGLE);
  jungle_texture.setRepeated(true);

  float view_height = m_world_view.getSize().y;
  sf::IntRect texture_rect{m_world_bounds};
  texture_rect.height += static_cast<int>(view_height);

  // Add the background sprite to the scene.
  std::unique_ptr<SpriteNode> background_sprite =
      std::make_unique<SpriteNode>(jungle_texture, texture_rect);
  background_sprite->setPosition(
      m_world_bounds.left,
      m_world_bounds.top - view_height);
  m_scene_layers[static_cast<size_t>(World::Layer::BACKGROUND)]->attachChild(
      std::move(background_sprite));

  // Add the finish line to the scene.
  sf::Texture& finish_texture = m_textures.get(Textures::ID::FINISH_LINE);
  std::unique_ptr<SpriteNode> finish_sprite =
      std::make_unique<SpriteNode>(finish_texture);
  finish_sprite->setPosition(0.f, -76.f);
  m_scene_layers[static_cast<size_t>(World::Layer::BACKGROUND)]->attachChild(
      std::move(finish_sprite));

  // Add smoke particle node to the scene.
  std::unique_ptr<ParticleNode> smoke_node =
      std::make_unique<ParticleNode>(Particle::Type::SMOKE, m_textures);
  m_scene_layers[static_cast<std::size_t>(World::Layer::LOWER_AIR)]
      ->attachChild(std::move(smoke_node));

  // Add propellant particle node to the scene.
  std::unique_ptr<ParticleNode> propellant_node =
      std::make_unique<ParticleNode>(Particle::Type::PROPELLANT, m_textures);
  m_scene_layers[static_cast<std::size_t>(World::Layer::LOWER_AIR)]
      ->attachChild(std::move(propellant_node));

  // Add sound effect node.
  std::unique_ptr<SoundNode> sound_node = std::make_unique<SoundNode>(m_sounds);
  m_scene_graph.attachChild(std::move(sound_node));

  // Add player's aircraft
  std::unique_ptr<Aircraft> leader =
      std::make_unique<Aircraft>(Aircraft::Type::EAGLE, m_textures, m_fonts);
  m_player_aircraft = leader.get();
  m_player_aircraft->setPosition(m_spawn_position);
  m_scene_layers[static_cast<std::size_t>(World::Layer::UPPER_AIR)]
      ->attachChild(std::move(leader));

  // Add enemy aircraft.
  addEnemies();
}

void World::addEnemies() noexcept
{
  // Add enemies to the spawn point container.
  addEnemy(Aircraft::Type::RAPTOR, 0.f, 500.f);
  addEnemy(Aircraft::Type::RAPTOR, 0.f, 1000.f);
  addEnemy(Aircraft::Type::RAPTOR, 100.f, 1100.f);
  addEnemy(Aircraft::Type::RAPTOR, -100.f, 1100.f);
  addEnemy(Aircraft::Type::AVENGER, -70.f, 1400.f);
  addEnemy(Aircraft::Type::AVENGER, -70.f, 1600.f);
  addEnemy(Aircraft::Type::AVENGER, 70.f, 1400.f);
  addEnemy(Aircraft::Type::AVENGER, 70.f, 1600.f);

  // Sort all enemies according to their y value, such that lower enemies are
  // checked first for spawning.
  std::sort(
      m_enemy_spawn_points.begin(),
      m_enemy_spawn_points.end(),
      [](SpawnPoint left, SpawnPoint right) {
        return left.y < right.y;
      });
}

void World::addEnemy(Aircraft::Type type, float rel_x, float rel_y) noexcept
{
  SpawnPoint spawn{
      type,
      m_spawn_position.x + rel_x,
      m_spawn_position.y - rel_y};
  m_enemy_spawn_points.push_back(std::move(spawn));
}

void World::update(sf::Time const& dt)
{
  // Scroll the world, reset player velocity.
  m_world_view.move(0.f, m_scroll_speed * dt.asSeconds());
  m_player_aircraft->setVelocity(0.f, 0.f);

  // Setup commands to destroy entities, and guide missiles.
  destroyEntitiesOusideView();
  guideMissiles();

  // Forward commands to scene graph, adapt velocity (scrolling, diagonal
  // correction)
  while (!m_command_queue.isEmpty())
    m_scene_graph.onCommand(m_command_queue.pop(), dt);
  adaptPlayerVelocity();

  // Collision detection and response (may destroy entities).
  handleCollisions();

  // Remove all destroyed entities, create new ones.
  m_scene_graph.removeWrecks();
  spawnEnemies();

  // Regular update step, adapt position (correct if outside view)
  m_scene_graph.update(dt, m_command_queue);
  adaptPlayerPosition();

  updateSounds();
}

void World::draw()
{
  if (PostEffect::isSupported())
  {
    m_scene_texture.clear();
    m_scene_texture.setView(m_world_view);
    m_scene_texture.draw(m_scene_graph);
    m_scene_texture.display();
    m_bloom_effect.apply(m_scene_texture, m_window);
  }
  else
  {
    m_window.setView(m_world_view);
    m_window.draw(m_scene_graph);
  }
}

void World::monitorState(SimMonitor::Monitor&, SimMonitor::Frame::World&) const
{
}

CommandQueue& World::getCommandQueue() noexcept
{
  return m_command_queue;
}

bool World::hasAlivePlayer() const noexcept
{
  return !m_player_aircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
  return !m_world_bounds.contains(m_player_aircraft->getPosition());
}

void World::adaptPlayerPosition()
{
  // Keep player's position inside the screen bounds, at least borderDistance
  // units from the border.
  sf::FloatRect view_bounds{
      m_world_view.getCenter() - m_world_view.getSize() / 2.f,
      m_world_view.getSize()};
  float const border_distance = 40.f;

  sf::Vector2f position = m_player_aircraft->getPosition();
  position.x = std::max(position.x, view_bounds.left + border_distance);
  position.x = std::min(
      position.x,
      view_bounds.left + view_bounds.width - border_distance);
  position.y = std::max(position.y, view_bounds.top + border_distance);
  position.y = std::min(
      position.y,
      view_bounds.top + view_bounds.height - border_distance);
  m_player_aircraft->setPosition(position);
}

void World::adaptPlayerVelocity() noexcept
{
  sf::Vector2f velocity = m_player_aircraft->getVelocity();

  // If moving diagonally, reduce velocity (to have always same velocity).
  if (velocity.x != 0.f && velocity.y != 0.f)
    m_player_aircraft->setVelocity(velocity / std::sqrt(2.f));

  // Add scrolling velocity.
  m_player_aircraft->accelerate(0.f, m_scroll_speed);
}

void World::handleCollisions() noexcept
{
  std::set<SceneNode::Pair> collision_pairs;
  m_scene_graph.checkSceneCollision(m_scene_graph, collision_pairs);

  for (SceneNode::Pair pair : collision_pairs)
  {
    if (matchesCategories(
            pair,
            BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT},
            BitFlags<Category::Type>{Category::Type::ENEMY_AIRCRAFT}))
    {
      auto& player = static_cast<Aircraft&>(*pair.first);
      auto& enemy = static_cast<Aircraft&>(*pair.second);

      // Collision: Player damage = enemy's remaining HP.
      player.damage(enemy.getHitpoints());
      enemy.destroy();
    }
    else if (matchesCategories(
                 pair,
                 BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT},
                 BitFlags<Category::Type>{Category::Type::PICKUP}))
    {
      auto& player = static_cast<Aircraft&>(*pair.first);
      auto& pickup = static_cast<Pickup&>(*pair.second);

      // Apply pickup effect to player, destroy projectile.
      pickup.apply(player);
      pickup.destroy();
      player.playLocalSound(m_command_queue, SoundEffect::ID::COLLECT_PICKUP);
    }
    else if (
        matchesCategories(
            pair,
            BitFlags<Category::Type>{Category::Type::ENEMY_AIRCRAFT},
            BitFlags<Category::Type>{Category::Type::ALLIED_PROJECTILE}) ||
        matchesCategories(
            pair,
            BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT},
            BitFlags<Category::Type>{Category::Type::ENEMY_PROJECTILE}))
    {
      auto& aircraft = static_cast<Aircraft&>(*pair.first);
      auto& projectile = static_cast<Projectile&>(*pair.second);

      // Apply projectile damage to aircraft, destroy projectile.
      aircraft.damage(projectile.getDamage());
      projectile.destroy();
    }
  }
}

void World::updateSounds() noexcept
{
  // Set Listener's position to player position.
  m_sounds.setListenerPosition(m_player_aircraft->getWorldPosition());

  // Remove unused sounds.
  m_sounds.removeStoppedSounds();
}

bool World::matchesCategories(
    SceneNode::Pair& colliders,
    BitFlags<Category::Type> type_1,
    BitFlags<Category::Type> type_2) const noexcept
{
  BitFlags<Category::Type> category_1 = colliders.first->getCategory();
  BitFlags<Category::Type> category_2 = colliders.second->getCategory();

  // Make sure first pair entry has category type_1 and second has type_2.
  if (type_1 == category_1 && type_2 == category_2)
  {
    return true;
  }
  else if (type_1 == category_2 && type_2 == category_1)
  {
    std::swap(colliders.first, colliders.second);
    return true;
  }
  else
  {
    return false;
  }
}

void World::spawnEnemies() noexcept
{
  // Spawn all enemies entering the view area (including distance) this frame.
  while (!m_enemy_spawn_points.empty() &&
         m_enemy_spawn_points.back().y > getBattlefieldBounds().top)
  {
    SpawnPoint spawn = m_enemy_spawn_points.back();

    std::unique_ptr<Aircraft> enemy =
        std::make_unique<Aircraft>(spawn.type, m_textures, m_fonts);
    enemy->setPosition(spawn.x, spawn.y);
    enemy->setRotation(180.f);
    m_scene_layers[static_cast<std::size_t>(World::Layer::UPPER_AIR)]
        ->attachChild(std::move(enemy));

    // Enemy is spawned, remove from the list to spawn.
    m_enemy_spawn_points.pop_back();
  }
}

void World::destroyEntitiesOusideView() noexcept
{
  Command command;
  command.name = "DestroyEntitiesOutsideView";
  command.category = BitFlags<Category::Type>{
      Category::Type::PROJECTILE,
      Category::Type::ENEMY_AIRCRAFT};
  command.action = derivedAction<Entity>([this](Entity& e, sf::Time) {
    if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
      e.destroy();
  });

  m_command_queue.push(command);
}

void World::guideMissiles() noexcept
{
  // Setup command that stores all enemies in m_active_enemies.
  Command enemy_collector;
  enemy_collector.name = "CollectEnemies";
  enemy_collector.category =
      BitFlags<Category::Type>{Category::Type::ENEMY_AIRCRAFT};
  enemy_collector.action =
      derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time) {
        if (!enemy.isDestroyed())
          m_active_enemies.push_back(&enemy);
      });

  // Setup command that guides all missiles to the enemy which is currently
  // closest to the player.
  Command missile_guider;
  missile_guider.name = "GuideMissiles";
  missile_guider.category =
      BitFlags<Category::Type>{Category::Type::ALLIED_PROJECTILE};
  missile_guider.action =
      derivedAction<Projectile>([this](Projectile& missile, sf::Time) {
        // Ignore unguided bullets.
        if (!missile.isGuided())
          return;

        float min_distance = std::numeric_limits<float>::max();
        Aircraft const* closest_enemy = nullptr;

        // Find closest enemy.
        for (auto const& enemy : m_active_enemies)
        {
          float enemy_distance = distance(missile, *enemy);
          if (enemy_distance < min_distance)
          {
            closest_enemy = enemy;
            min_distance = enemy_distance;
          }
        }

        if (closest_enemy)
          missile.guideTowards(closest_enemy->getWorldPosition());
      });

  // Push commands, reset active enemies.
  m_command_queue.push(enemy_collector);
  m_command_queue.push(missile_guider);
  m_active_enemies.clear();
}

sf::FloatRect World::getViewBounds() const noexcept
{
  return sf::FloatRect(
      m_world_view.getCenter() - m_world_view.getSize() / 2.f,
      m_world_view.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const noexcept
{
  // Return view bounds + some area at top, where enemies spawn.
  sf::FloatRect bounds = getViewBounds();
  bounds.top -= 100.f;
  bounds.height += 100.f;
  return bounds;
}

} // namespace FastSimDesign