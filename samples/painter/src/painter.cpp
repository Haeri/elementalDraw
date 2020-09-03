#include <thread>
#include <chrono>
#include <vector>

#include <elemd/window.hpp>
#include <elemd/context.hpp>

const int TARGET_POLL_FREQUENCY = 60;

int WIDTH = 470;
int HEIGHT = 500;
float target_poll_ms = 1.0f / TARGET_POLL_FREQUENCY;

double mouse_x = -100;
double mouse_y = -100;
bool mouse_click = false;

std::vector<elemd::vec2> points;

void draw(elemd::Context* ctx)
{
    ctx->set_fill_color({210, 210, 210});

    for (auto& point : points)
    {
        ctx->fill_circle(point.x(), point.y(), 3);
    }

    ctx->fill_circle(mouse_x, mouse_y, 4);

    if (mouse_click)
    {
        points.push_back({(float)mouse_x, (float)mouse_y});
    }

    ctx->draw_frame();
}

int main(void)
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    elemd::WindowConfig winc = elemd::WindowConfig{"Painting App", WIDTH, HEIGHT};
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    win->add_mouse_click_listener([&](elemd::mouse_button_event event) {
        if (event.button == elemd::MOUSE_BUTTON_LEFT &&
            (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT))
        {
            mouse_click = true;
        }
        else
        {
            mouse_click = false;
        }
    });

    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        mouse_x = event.x;
        mouse_y = event.y;

        draw(ctx);
    });

    ctx->set_clear_color({255, 255, 255});

    while (win->is_running())
    {
        win->poll_events();
        std::this_thread::sleep_for(std::chrono::duration<float, std::ratio<1>>(target_poll_ms));
    }

    win->destroy();
    return 0;
}