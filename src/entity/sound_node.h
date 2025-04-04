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

#ifndef FAST_SIM_DESIGN_SOUND_NODE_H
#define FAST_SIM_DESIGN_SOUND_NODE_H

#include "../core/resource_identifiers.h"
#include "../gui/scene_node.h"

#include <SFML/System/Vector2.hpp>

namespace FastSimDesign {
class SoundPlayer;
class SoundNode : public SceneNode
{
private:
  using Parent = SceneNode;

public:
  explicit SoundNode(SoundPlayer& player) noexcept;
  virtual ~SoundNode() = default;

  void playSound(SoundEffect::ID sound, sf::Vector2f position);

  virtual BitFlags<Category::Type> getCategory() const noexcept override;

private:
  SoundPlayer& m_sounds;
};
} // namespace FastSimDesign
#endif