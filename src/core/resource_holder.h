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

#ifndef FAST_SIM_DESIGN_RESOURCE_HOLDER_H
#define FAST_SIM_DESIGN_RESOURCE_HOLDER_H

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>

namespace FastSimDesign {
template<typename Resource, typename Identifier>
class ResourceHolder final
{
public:
  explicit ResourceHolder() = default;
  ResourceHolder(ResourceHolder const&) = default;
  ResourceHolder(ResourceHolder&&) = default;
  ResourceHolder& operator=(ResourceHolder const&) = default;
  ResourceHolder& operator=(ResourceHolder&&) = default;
  virtual ~ResourceHolder() = default;

  void load(Identifier id, std::string const& filePath)
  {
    std::unique_ptr<Resource> resource{std::make_unique<Resource>()};
    if (!resource->loadFromFile(filePath))
      throw std::runtime_error(
          "ResourceHolder::load - Failed to load " + filePath);

    insertResource(id, std::move(resource));
  }

  template<typename Parameter>
  void load(
      Identifier id, std::string const& filePath, Parameter const& secondParam)
  {
    std::unique_ptr<Resource> resource{std::make_unique<Resource>()};
    if (!resource->loadFromFile(filePath, secondParam))
      throw std::runtime_error(
          "ResourceHolder::load - Failed to load " + filePath);

    insertResource(id, std::move(resource));
  }

  Resource& get(Identifier id) noexcept
  {
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
  }

  Resource const& get(Identifier id) const
  {
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
  }

private:
  void insertResource(Identifier id, std::unique_ptr<Resource> resource)
  {
    auto inserted = m_resource_map.emplace(id, std::move(resource));
    assert(inserted.second);
  }

private:
  std::map<Identifier, std::unique_ptr<Resource>> m_resource_map{};
};

} // namespace FastSimDesign
#endif