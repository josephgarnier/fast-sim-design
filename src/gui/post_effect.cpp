////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "post_effect.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////
  bool PostEffect::isSupported() noexcept
  {
    return sf::Shader::isAvailable();
  }

  void PostEffect::applyShader(sf::Shader const& shader, sf::RenderTarget& output) noexcept
  {
    sf::Vector2f outside_size = static_cast<sf::Vector2f>(output.getSize());

    sf::VertexArray vertices{sf::TrianglesStrip, 4};
    vertices[0] = sf::Vertex{sf::Vector2f{0, 0}, sf::Vector2f{0, 1}};
    vertices[1] = sf::Vertex{sf::Vector2f{outside_size.x, 0}, sf::Vector2f{1, 1}};
    vertices[2] = sf::Vertex{sf::Vector2f{0, outside_size.y}, sf::Vector2f{0, 0}};
    vertices[3] = sf::Vertex{sf::Vector2f{outside_size}, sf::Vector2f{1, 0}};

    sf::RenderStates states;
    states.shader = &shader;
    states.blendMode = sf::BlendNone;

    output.draw(vertices, states);
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////

}