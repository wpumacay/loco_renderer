
#include <string>
#include <cstdint>

#include <glad/gl.h>

#include <loco/renderer/core/vertex_array_object_t.hpp>
#include <spdlog/fmt/bundled/format.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif

namespace loco {
namespace renderer {

VertexArray::VertexArray() { glGenVertexArrays(1, &m_OpenGLId); }

VertexArray::~VertexArray() {
    m_Buffers.clear();
    if (m_OpenGLId != 0) {
        glDeleteVertexArrays(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto VertexArray::AddBuffer(VertexBuffer::uptr buffer, bool is_instanced)
    -> void {
    const auto& buffer_layout = buffer->layout();
    const auto& buffer_elements = buffer_layout.elements();

    const auto STRIDE = buffer_layout.stride();

    glBindVertexArray(m_OpenGLId);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->opengl_id());

    for (const auto& element : buffer_elements) {
        glEnableVertexAttribArray(m_NumAttribIndx);
        glVertexAttribPointer(m_NumAttribIndx, static_cast<int>(element.count),
                              ToOpenGLEnum(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              static_cast<int>(STRIDE),
                              // cppcheck-suppress cstyleCast
                              (const void*)(intptr_t)element.offset);  // NOLINT
        if (is_instanced) {
            glVertexAttribDivisor(m_NumAttribIndx, 1);
        }

        m_NumAttribIndx++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_Buffers.push_back(std::move(buffer));
}

auto VertexArray::Bind() const -> void { glBindVertexArray(m_OpenGLId); }

// NOLINTNEXTLINE
auto VertexArray::Unbind() const -> void { glBindVertexArray(0); }

auto VertexArray::ToString() const -> std::string {
    std::string str_repr = "VertexArray";
    str_repr += fmt::format("(opengl_id={0}, num_attribs={1}, num_buffers={2})",
                            m_OpenGLId, m_NumAttribIndx, m_Buffers.size());
    return str_repr;
}

}  // namespace renderer
}  // namespace loco

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif
