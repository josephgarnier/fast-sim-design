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

#ifndef FAST_SIM_DESIGN_MODAL_DIALOG_H
#define FAST_SIM_DESIGN_MODAL_DIALOG_H

#include <SFML/System/Time.hpp>

#include <memory>
#include <string>

namespace FastSimDesign {
  class ModalDialog
  {
  public:
    using Ptr = std::unique_ptr<ModalDialog>;

  public:
    explicit ModalDialog(std::string title) noexcept; // Default constructor
    ModalDialog(ModalDialog const&) = default; // Copy constructor
    ModalDialog(ModalDialog&&) = default; // Move constructor
    ModalDialog& operator=(ModalDialog const&) = default; // Copy assignment operator
    ModalDialog& operator=(ModalDialog&&) = default; // Move assignment operator
    virtual ~ModalDialog() = default; // Destructor

    bool isOpen() const noexcept { return m_open; }
    void close();
    std::string const& getTitle() const noexcept { return m_title; }

    void updateModal(sf::Time const& dt);

  private:
    virtual void draw(sf::Time const& dt) = 0;

  protected:
    std::string m_title;
    bool m_open = true;
  };
}
#endif