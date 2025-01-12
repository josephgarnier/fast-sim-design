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
#include "../entity/category.h"
#include "../core/command.h"
#include "../utils/generic_utility.h"
#include <cassert>

namespace FastSimDesign {
  SceneNode::SceneNode() noexcept
    : m_children{}
    , m_parent{nullptr}
  {
  }

  void SceneNode::attachChild(Ptr child) noexcept
  {
    child->m_parent = this;
    m_children.push_back(std::move(child));
  }

  SceneNode::Ptr SceneNode::detachChild(SceneNode const& node) noexcept
  {
    auto found = std::find_if(std::begin(m_children), std::end(m_children), [&](Ptr& p) {
      return p.get() == &node;
    });
    assert(found != std::end(m_children));

    Ptr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);
    return result;
  }

  void SceneNode::update(sf::Time const & dt) noexcept
  {
    updateCurrent(dt);
    updateChildren(dt);
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

  void SceneNode::onCommand(Command const& command, sf::Time const & dt) noexcept
  {
    if (command.category & getCategory())
      command.action(*this, dt);

    for (auto const& child : m_children)
      child->onCommand(command, dt);
  }

  unsigned int SceneNode::getCategory() const noexcept
  {
    return toUnderlyingType(Category::Type::SCENE);
  }

  void SceneNode::updateCurrent(sf::Time const & dt) noexcept
  {
    // Do nothing by default.
  }

  void SceneNode::updateChildren(sf::Time const & dt) noexcept
  {
    for (Ptr const& child : m_children)
      child->update(dt);
  }

  void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
  {
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
  }

  void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const noexcept
  {
    // Do nothing by default.
  }

  void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const noexcept
  {
    for (Ptr const& child : m_children)
      child->draw(target, states);
  }
}