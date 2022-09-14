
#include <iostream>

#include <glad/gl.h>

#include <loco/renderer/window/window_t.hpp>
#include <loco/renderer/shader/program_t.hpp>
#include <loco/renderer/core/vertex_buffer_t.hpp>
#include <loco/renderer/core/vertex_array_t.hpp>
#include "loco/renderer/core/vertex_buffer_layout_t.hpp"
#include <loco/renderer/core/texture_t.hpp>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 texcoord;

    out vec3 frag_color;
    out vec2 tex_coord;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
        frag_color = color;
        tex_coord = texcoord;
    }
)";

constexpr const char* FRAG_SHADER_SRC = R"(
    #version 330 core

    in vec3 frag_color;
    in vec2 tex_coord;
    out vec4 output_color;

    uniform sampler2D u_texture;

    void main() {
        output_color = texture(u_texture, tex_coord) * vec4(frag_color, 1.0f);
    }
)";

auto main() -> int {
    // NOLINTNEXTLINE
    auto IMAGE_PATH =
        std::string(loco::renderer::RESOURCES_PATH) + "images/awesomeface.png";

    auto window =
        std::make_unique<loco::renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);

    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == loco::renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = std::make_unique<loco::renderer::Program>(
        "basic_2d", VERT_SHADER_SRC, FRAG_SHADER_SRC);

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
    /*|      pos     |     color      |  texture  */
        -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 2.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 0.0F, 0.0F, 1.0F, 2.0F, 2.0F, // NOLINT
        -0.5F,  0.5F, 1.0F, 1.0F, 1.0F, 0.0F, 2.0F // NOLINT
    };
    constexpr uint32_t NUM_VERTICES = 6;

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3  // second triangle
    };
    // clang-format on

    loco::renderer::BufferLayout layout = {
        {"position", loco::renderer::eElementType::FLOAT_2, false},
        {"color", loco::renderer::eElementType::FLOAT_3, false},
        {"texcoord", loco::renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_unique<loco::renderer::VertexBuffer>(
        layout, loco::renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto ibo = std::make_unique<loco::renderer::IndexBuffer>(
        loco::renderer::eBufferUsage::STATIC, NUM_VERTICES, buffer_indices);

    auto vao = std::make_unique<loco::renderer::VertexArray>();
    vao->AddVertexBuffer(std::move(vbo));
    vao->SetIndexBuffer(std::move(ibo));

    auto texture =
        std::make_unique<loco::renderer::Texture>(IMAGE_PATH.c_str());

    while (window->active()) {
        window->Begin();
        program->Bind();
        texture->Bind();
        vao->Bind();

        glDrawElements(GL_TRIANGLES, NUM_VERTICES, GL_UNSIGNED_INT, nullptr);

        vao->Unbind();
        texture->Unbind();
        program->Unbind();
        window->End();
    }

    return 0;
}
