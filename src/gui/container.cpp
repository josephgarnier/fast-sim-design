////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
  namespace GUI {
    ////////////////////////////////////////////////////////////
    /// Statics
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// Methods
    ////////////////////////////////////////////////////////////
    Container::Container() noexcept
      : Parent{}
      , m_children{}
      , m_selected_child{-1}
    {
    }

    void Container::pack(Component::Ptr component)
    {
      m_children.push_back(component);
      if (!hasSelection() && component->isSelectable())
        select(m_children.size() - 1);
    }

    bool Container::isSelectable() const noexcept
    {
      return false;
    }

    void Container::handleEvent(sf::Event const& event)
    {
      // If we have selected a child then give it events ; the active component is the one that should receive the events instead of the container managing it.
      if (hasSelection() && m_children[static_cast<std::size_t>(m_selected_child)]->isActive())
      {
        m_children[static_cast<std::size_t>(m_selected_child)]->handleEvent(event);
      } else if (event.type == sf::Event::KeyReleased)
      {
        if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
        {
          selectPrevious();
        } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        {
          selectNext();
        } else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter)
        {
          if (hasSelection())
            m_children[static_cast<std::size_t>(m_selected_child)]->activate();
        }
      }
    }

    void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
      states.transform *= getTransform();
      for (Component::Ptr const& child : m_children)
      {
        target.draw(*child, states);
      }
    }

    bool Container::hasSelection() const noexcept
    {
      return m_selected_child >= 0;
    }

    void Container::select(std::size_t index) noexcept
    {
      if (m_children[index]->isSelectable())
      {
        if (hasSelection())
          m_children[static_cast<std::size_t>(m_selected_child)]->deselect();

        m_children[index]->select();
        m_selected_child = static_cast<int>(index);
      }
    }

    void Container::selectNext() noexcept
    {
      if (!hasSelection())
        return;

      // Search next component that is selectable, wrap around if necessary.
      int next = m_selected_child;
      do
      {
        next = (next + 1) % static_cast<int>(m_children.size());
      } while (!m_children[static_cast<std::size_t>(next)]->isSelectable());

      // Select that component.
      select(static_cast<std::size_t>(next));
    }

    void Container::selectPrevious() noexcept
    {
      if (!hasSelection())
        return;

      // Search previous component that is selectable, wrap around if necessary.
      std::size_t prev = static_cast<std::size_t>(m_selected_child);
      do
      {
        prev = (prev + m_children.size() - 1) % m_children.size(); // We need "+ length" to avoid negative indexes, due to the way modulus works in C++ (it use truncated division).
      } while (!m_children[prev]->isSelectable());

      // Select that component.
      select(prev);
    }

  }
}