////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "scene_node.h"

#include "../core/command.h"
#include "../utils/math_util.h"
#include "monitor/frame.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cassert>
#include <typeinfo>

namespace FastSimDesign {

bool collision(SceneNode const& left, SceneNode const& right) noexcept
{
  return left.getBoundingRect().intersects(right.getBoundingRect());
}

float distance(SceneNode const& left, SceneNode const& right) noexcept
{
  return Math::length(left.getWorldPosition() - right.getWorldPosition());
}

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
SceneNode::SceneNode(Category::Type category) noexcept
  : SimMonitor::Monitorable{}
  , m_default_category{category}
{
}

void SceneNode::attachChild(Ptr child) noexcept
{
  child->m_parent = this;
  m_children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(SceneNode const& node)
{
  auto found =
      std::find_if(std::begin(m_children), std::end(m_children), [&](Ptr& p) {
        return p.get() == &node;
      });
  assert(found != std::end(m_children));

  Ptr result = std::move(*found);
  result->m_parent = nullptr;
  m_children.erase(found);
  return result;
}

void SceneNode::update(sf::Time const& dt, CommandQueue& commands)
{
  updateCurrent(dt, commands);
  updateChildren(dt, commands);
}

void SceneNode::monitorState(
    SimMonitor::Monitor& monitor,
    SimMonitor::Frame::SceneNode& frame_object) const
{
  // Current node.
  frame_object.category = Category::toString(getCategory());
  frame_object.type = typeid(*this).name();

  // Children node.
  for (auto const& child : m_children)
  {
    SimMonitor::Frame::SceneNode::Ptr child_frame_object =
        std::make_unique<SimMonitor::Frame::SceneNode>();
    child->monitorState(monitor, *child_frame_object);
    frame_object.attachChild(std::move(child_frame_object));
  }
}

sf::Vector2f SceneNode::getWorldPosition() const noexcept
{
  return getWorldTransform() * sf::Vector2f{};
}

sf::Transform SceneNode::getWorldTransform() const noexcept
{
  sf::Transform transform = sf::Transform::Identity;
  for (SceneNode const* node = this; node != nullptr; node = node->m_parent)
    transform = node->getTransform() * transform;

  return transform;
}

void SceneNode::onCommand(Command const& command, sf::Time const& dt) noexcept
{
  if (command.category & getCategory())
    command.action(*this, dt);

  for (auto const& child : m_children)
    child->onCommand(command, dt);
}

BitFlags<Category::Type> SceneNode::getCategory() const noexcept
{
  return m_default_category;
}

sf::FloatRect SceneNode::getBoundingRect() const noexcept
{
  return sf::FloatRect{};
}

bool SceneNode::isMarkedForRemoval() const noexcept
{
  // By default, remove node if entity is destroyed.
  return isDestroyed();
}

bool SceneNode::isDestroyed() const noexcept
{
  // By default, scene node needn't be removed.
  return false;
}

void SceneNode::checkSceneCollision(
    SceneNode& scene_graph, std::set<SceneNode::Pair>& collision_pairs) noexcept
{
  checkNodeCollision(scene_graph, collision_pairs);

  for (Ptr const& child : scene_graph.m_children)
    checkSceneCollision(*child, collision_pairs);
}

void SceneNode::checkNodeCollision(
    SceneNode& node, std::set<SceneNode::Pair>& collision_pairs) noexcept
{
  if (this != &node && collision(*this, node) && !isDestroyed() &&
      !node.isDestroyed())
    collision_pairs.insert(std::minmax(this, &node));

  for (SceneNode::Ptr const& child : m_children)
    child->checkNodeCollision(node, collision_pairs);
}

void SceneNode::removeWrecks() noexcept
{
  // Remove all children which request so.
  auto erase_begin = std::remove_if(
      std::begin(m_children),
      std::end(m_children),
      [](Ptr const& child) {
        return child->isMarkedForRemoval();
      });
  m_children.erase(erase_begin, std::end(m_children));

  // Call function recursively for all remaining children.
  std::for_each(
      std::begin(m_children),
      std::end(m_children),
      [](Ptr const& child) {
        child->removeWrecks();
      });
}

void SceneNode::updateCurrent(sf::Time const&, CommandQueue&)
{
  // Do nothing by default.
}

void SceneNode::updateChildren(sf::Time const& dt, CommandQueue& commands)
{
  for (Ptr const& child : m_children)
    child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // Apply transform of current node.
  states.transform *= getTransform();

  // Draw node and children with changed transform.
  drawCurrent(target, states);
  drawChildren(target, states);

  // Draw bounding rectangle - disabled by default
  drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
  // Do nothing by default.
}

void SceneNode::drawChildren(
    sf::RenderTarget& target, sf::RenderStates states) const
{
  for (Ptr const& child : m_children)
    child->draw(target, states);
}

void SceneNode::drawBoundingRect(
    sf::RenderTarget& target, sf::RenderStates) const
{
  sf::FloatRect rect = getBoundingRect();

  sf::RectangleShape shape;
  shape.setPosition(sf::Vector2f{rect.left, rect.top});
  shape.setSize(sf::Vector2f{rect.width, rect.height});
  shape.setFillColor(sf::Color::Transparent);
  shape.setOutlineColor(sf::Color::Green);
  shape.setOutlineThickness(1.f);

  target.draw(shape);
}

} // namespace FastSimDesign