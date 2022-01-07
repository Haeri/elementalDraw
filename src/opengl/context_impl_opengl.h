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

//#define UNIFORM_BUFFER_ARRAY_MAX_COUNT 65536*2
//#define UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE UNIFORM_BUFFER_ARRAY_MAX_COUNT * sizeof(uniform_rect)

namespace elemd
{
    class ContextImplOpengl : public Context
    {
    public:        
        int MAX_UNIFORM_RECT_PER_BLOCK_COUNT = -1;            

        struct uniform_rect
        {
            class color color;      // 32   1
            vec2 vertices[4];       // 64   2
            float border_radius[4]; // 32   1        
            float sampler_index;    //  
            float use_tint;         // 
            vec2 resolution;        // 32   1
            vec2 uvs[2];            // 32   1
            float line_width[4];    // 32   1
            float shadow_size;      //
            float is_msdf[3];       // 16   1
        };                          // 256


        struct point_vertex
        {
            vec2 position;
        };

        enum draw_call_type
        {
            UNSET,
            COLOR,
            SCISSOR,
            SCISSOR_CLEAR,
            TEXTURE_INCREMENT
        };
        struct draw_call_chain
        {
            int instance_index;
            int scissor_index;
            draw_call_type type;
        };
        struct scissor_primitive
        {
            float x;
            float y;
            float width;
            float height;
        };

       
        imageImplOpengl* dummy;
        std::vector<imageImplOpengl*> images;

        //std::vector<vertex> rect_vertices = {{vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}};
        std::vector<uint32_t> rect_indices = {0, 1, 2, 1, 3, 2};
        
        std::vector<point_vertex> rect_vertices = {
            {vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}
        };

        std::vector<scissor_primitive> scissor_primitives = {};
        std::vector<draw_call_chain> draw_call_indices = {};
        int current_color_call_cnt = 0;
        draw_call_type current_type = UNSET;

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
        int instance_offset_location = -1;

        GLuint vertex_array_object;
        GLuint vertex_buffer;
        GLuint index_buffer;

        GLuint storageBuffer;

        ContextImplOpengl(Window* window);
        ~ContextImplOpengl();

        void configure_surface();
        void create_uniform_buffer();
        //void create_storage_buffer();
        void create_vertex_array();
        void create_shader_programm();
        void initialize_resources();
        void update_viewport(uint32_t width, uint32_t height);

        void update_uniform_buffer();
        //void update_storage_buffer();

        void queue_color_call();
        void destroy() override;
    };

} // namespace elemd

#endif // ELEMD_OPENGL_CONTEXT_HPP