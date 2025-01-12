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

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <vector>

namespace FastSimDesign {
  struct Command;
  class SceneNode : public sf::Transformable
    , public sf::Drawable
    , private sf::NonCopyable
  {
  public:
    using Ptr = std::unique_ptr<SceneNode>;

  public:
    explicit SceneNode() noexcept; // Default constructor
    virtual ~SceneNode() = default; // Destructor

    void attachChild(Ptr child) noexcept;
    SceneNode::Ptr detachChild(SceneNode const& node) noexcept;

    void update(sf::Time const & dt) noexcept;

    sf::Vector2f getWorldPosition() const noexcept;
    sf::Transform getWorldTransform() const noexcept;

    void onCommand(Command const& command, sf::Time const & dt) noexcept;
    virtual unsigned int getCategory() const noexcept;

  protected:
  private:
    virtual void updateCurrent(sf::Time const & dt) noexcept;
    void updateChildren(sf::Time const & dt) noexcept;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const noexcept;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const noexcept;

  public:
  protected:
  private:
    std::vector<Ptr> m_children;
    SceneNode* m_parent;
  };
}
#endif