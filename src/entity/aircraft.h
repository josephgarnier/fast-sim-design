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

#ifndef FAST_SIM_DESIGN_AIRCRAFT_H
#define FAST_SIM_DESIGN_AIRCRAFT_H

#include "entity.h"
#include "../core/resource_identifiers.h"

#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
  class Aircraft : public Entity
  {
  public:
    enum class Type : uint16_t
    {
      EAGLE,
      RAPTOR,
    };

  private:
    using Parent = Entity;

  private:
    static Textures::ID toTextureID(Aircraft::Type type);

  public:
    explicit Aircraft(Aircraft::Type type, TextureHolder const& textures); // Default constructor
    virtual ~Aircraft() = default; // Destructor

    virtual unsigned int getCategory() const noexcept override;

  protected:
  private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    Aircraft::Type m_type;
    sf::Sprite m_sprite;
  };
}
#endif
