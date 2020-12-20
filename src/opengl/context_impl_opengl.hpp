#ifndef ELEMD_OPENGL_CONTEXT_HPP
#define ELEMD_OPENGL_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>
#include <array>
#include <atomic>

#include "elemd/vec2.hpp"
#include "elemd/color.hpp"
#include "../window_impl.hpp"
#include "image_impl_opengl.hpp"

#define UNIFORM_BUFFER_ARRAY_MAX_COUNT 65536/2
#define UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE UNIFORM_BUFFER_ARRAY_MAX_COUNT * sizeof(uniform_rect)


namespace elemd
{
    class ContextImplOpengl : public Context
    {
    public:            

        struct uniform_rect
        {
            color fill_color;
            vec2 vertices[4];
            vec2 border_radius[4];
            vec2 sampler_index[2];
            float line_size;
            float stroke_color[3];
            vec2 uvs[2];
        };

        struct point_vertex
        {
            vec2 position;
        };

        imageImplOpengl* dummy;
        std::vector<imageImplOpengl*> images;

        //std::vector<vertex> rect_vertices = {{vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}};
        std::vector<uint32_t> rect_indices = {0, 1, 2, 1, 3, 2};
        
        std::vector<point_vertex> point_vertices = {
            {vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}
        };

        ///std::vector<uniform_rect> uniforms = {};
        std::vector<uniform_rect> storage = {};
        int last_uniform_cnt = 0;

        int texture_array_size = 32;

        WindowImpl* _window;

        std::atomic<bool> resizing = false;
        std::atomic<bool> rendering = false;
        bool headless = false;
        bool dirty = true;
        

        int shaderProgram = -1;

        GLuint vertex_array_object;
        GLuint vertex_buffer;
        GLuint index_buffer;

        GLuint storageBuffer;

        ContextImplOpengl(Window* window);
        ~ContextImplOpengl();

        void configure_surface();
        void create_storage_buffer();
        void create_vertex_array();
        void create_shader_programm();
        void initialize_resources();
        void update_swapchain(uint32_t width, uint32_t height);

        ///void update_uniforms();
        void update_storage();

        void destroy() override;
    };

} // namespace elemd

#endif // ELEMD_OPENGL_CONTEXT_HPP