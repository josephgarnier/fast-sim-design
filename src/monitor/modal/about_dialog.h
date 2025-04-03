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

#ifndef FAST_SIM_DESIGN_ABOUT_DIALOG_H
#define FAST_SIM_DESIGN_ABOUT_DIALOG_H

#include "modal_dialog.h"

namespace FastSimDesign {
class AboutDialog : public ModalDialog
{
private:
  using Parent = ModalDialog;

public:
  explicit AboutDialog() noexcept;
  AboutDialog(AboutDialog const&) = default;
  AboutDialog(AboutDialog&&) = default;
  AboutDialog& operator=(AboutDialog const&) = default;
  AboutDialog& operator=(AboutDialog&&) = default;
  virtual ~AboutDialog() = default;

private:
  virtual void draw(sf::Time const& dt) override;
};
} // namespace FastSimDesign
#endif