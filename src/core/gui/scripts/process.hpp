#pragma once

#include <QObject>
#include <QQuickItem>
#include <QFileDialog>
#include <QStandardPaths>

#include <dxflib/dl_dxf.h>

#include <psimpl/psimpl.h>

#include "../../utils/polygon.hpp"
#include "../../utils/image.hpp"
#include "../../utils/dxf.hpp"

#include "../components/editor.hpp"

#include "./config.hpp"

#include "../../macros.h"

namespace core::gui::components {
    class editor;
}

namespace core::gui::scripts {
  class process: public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(bool active MEMBER active NOTIFY activeChanged)

    Q_PROPERTY(core::gui::components::editor * editor MEMBER _editor)
    Q_PROPERTY(core::gui::scripts::config * config MEMBER _config)

    friend class gui::components::editor;

   public:
    Q_INVOKABLE
    core::gui::components::editor *
      set_editor(
        _p(value, core::gui::components::editor *) = nullptr
      );

    Q_INVOKABLE
    core::gui::scripts::config *
      set_config(
        _p(value, core::gui::scripts::config *) = nullptr
      );

    Q_INVOKABLE
    void
      step_path(
        _p(path, QString const&) = ""
      );
    Q_INVOKABLE
    void
      step_detect(
      );
    Q_INVOKABLE
    void
      step_process(
      );
    Q_INVOKABLE
    void
      step_simplify(
        _p(threshold_a, double),
        _p(threshold_b, double)
      );
    Q_INVOKABLE
    void
      step_export_dxf(
        _p(path, QString const&) = ""
      ) const;

   signals:
    void activeChanged();

   public:
    _p(active, bool);

   public:
    _p(_editor, gui::components::editor *) = nullptr;
    _p(_config, gui::scripts::config *) = nullptr;
    _p(_image , std::unique_ptr<utils::image::image>) = nullptr;
    _p(_step  , int);

    _p(_contours      , utils::polygon::polygon_list);
    _p(_contours_final, utils::polygon::polygon_list);
  };
}

Q_DECLARE_METATYPE(core::gui::scripts::process *)
