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

#ifndef FAST_SIM_DESIGN_SCENE_NODE_H
#define FAST_SIM_DESIGN_SCENE_NODE_H

#include "../entity/category.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>
#include <utility>
#include <set>

namespace FastSimDesign {
  struct Command;
  class CommandQueue;
  class SceneNode : public sf::Transformable
    , public sf::Drawable
    , private sf::NonCopyable
  {
  public:
    using Ptr = std::unique_ptr<SceneNode>;
    using Pair = std::pair<SceneNode*, SceneNode*>;

  public:
    explicit SceneNode(Category::Type category = Category::Type::NONE) noexcept; // Default constructor
    virtual ~SceneNode() = default; // Destructor

    void attachChild(Ptr child) noexcept;
    SceneNode::Ptr detachChild(SceneNode const& node);

    void update(sf::Time const& dt, CommandQueue& commands);

    sf::Vector2f getWorldPosition() const noexcept;
    sf::Transform getWorldTransform() const noexcept;

    void onCommand(Command const& command, sf::Time const& dt) noexcept;
    virtual Category::Type getCategory() const noexcept;

    virtual sf::FloatRect getBoundingRect() const noexcept;
    virtual bool isMarkedForRemoval() const noexcept;
    virtual bool isDestroyed() const noexcept;
    void checkSceneCollision(SceneNode& scene_graph, std::set<SceneNode::Pair>& collision_pairs) noexcept;
    void checkNodeCollision(SceneNode& node, std::set<SceneNode::Pair>& collision_pairs) noexcept;
    void removeWrecks() noexcept;

  private:
    virtual void updateCurrent(sf::Time const& dt, CommandQueue& commands);
    void updateChildren(sf::Time const& dt, CommandQueue& commands);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

  private:
    std::vector<Ptr> m_children;
    SceneNode* m_parent;
    Category::Type m_default_category;
  };

  ////////////////////////////////////////////////////////////
  /// Outside class declarations/definitions
  ////////////////////////////////////////////////////////////
  bool collision(SceneNode const& left, SceneNode const& right) noexcept;
  float distance(SceneNode const& left, SceneNode const& right) noexcept;
}
#endif