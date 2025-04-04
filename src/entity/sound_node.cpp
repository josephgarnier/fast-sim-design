////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "sound_node.h"

#include "../core/sound_player.h"
#include "../entity/category.h"

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
SoundNode::SoundNode(SoundPlayer& player) noexcept
  : Parent{}
  , m_sounds{player}
{
}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
  m_sounds.play(sound, std::move(position));
}

BitFlags<Category::Type> SoundNode::getCategory() const noexcept
{
  return BitFlags<Category::Type>{Category::Type::SOUND_EFFECT};
}
} // namespace FastSimDesign