#pragma once

// clang-format off
#include <utility>

#include <loco/utils/common.hpp>
#include <loco/renderer/common.hpp>
#include <loco/renderer/input/callbacks.hpp>
#include <loco/renderer/window/window_properties.hpp>
// clang-format on

namespace loco {
namespace renderer {
// Forward declare (we'll use the parent-child relationship)
class CWindow;
}  // namespace renderer
}  // namespace loco

namespace loco {
namespace renderer {

class IWindowImpl {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(IWindowImpl);

    LOCO_DEFINE_SMART_POINTERS(IWindowImpl);

 public:
    explicit IWindowImpl(CWindowProperties properties)
        : m_Properties(std::move(properties)) {}

    virtual ~IWindowImpl() = default;

    virtual auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void = 0;

    virtual auto RegisterMousePressCallback(const MousePressCallback& callback)
        -> void = 0;

    virtual auto RegisterMouseMoveCallback(const MouseMoveCallback& callback)
        -> void = 0;

    virtual auto RegisterScrollCallback(const ScrollCallback& callback)
        -> void = 0;

    virtual auto RegisterResizeCallback(const ResizeCallback& callback)
        -> void = 0;

    virtual auto EnableCursor() -> void = 0;

    virtual auto DisableCursor() -> void = 0;

    virtual auto Begin() -> void = 0;

    virtual auto End() -> void = 0;

    virtual auto RequestClose() -> void = 0;

 protected:
    CWindowProperties m_Properties;
};

}  // namespace renderer
}  // namespace loco
