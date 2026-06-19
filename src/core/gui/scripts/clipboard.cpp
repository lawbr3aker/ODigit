#include "clipboard.hpp"

void
  core::gui::scripts::clipboard::setClipboard(
    _p(clipboard, QClipboard *)
  ) {
    _clipboard = clipboard;
  }

QString
  core::gui::scripts::clipboard::getText(
  ) {
    return _clipboard->text();
  }