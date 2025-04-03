////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "frame.h"

#include <algorithm>

namespace FastSimDesign {
namespace SimMonitor {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
void Frame::SceneNode::attachChild(Ptr child)
{
  child->m_parent = this;
  m_children.push_back(std::move(child));
}

Frame::SceneNode::Ptr Frame::SceneNode::detachChild(SceneNode const& child)
{
  auto found = std::find_if(
      std::begin(m_children),
      std::end(m_children),
      [&child](Ptr& node) { return node.get() == &child; });
  assert(found != std::end(m_children));

  Ptr result = std::move(*found);
  result->m_parent = nullptr;
  m_children.erase(found);
  return result;
}

} // namespace SimMonitor
} // namespace FastSimDesign