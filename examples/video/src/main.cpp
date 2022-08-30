#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/video/video.hpp>
#include <elemd/window.hpp>

int main(void)
{
    // configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Video Player", 1280 / 2, 720 / 2};
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // Load video
    elemd::Video* video = elemd::Video::create("./res/waves.mp4");
    ctx->_tmp_register_image(video->get_frame(0));

    ctx->_tmp_prepare();
    // ctx->set_clear_color(light);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        video->read_next();
        ctx->_tmp_update_image(video->get_frame(0));
        ctx->draw_image(0, 0, video->get_width() / 2, video->get_height() / 2, video->get_frame(0));

        ctx->draw_frame();
        ctx->present_frame();
    }

    video->destroy();

    // Cleanup
    win->destroy();

    return 0;
}