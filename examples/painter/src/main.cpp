#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

#include <elemd/context.hpp>
#include <elemd/window.hpp>

const int TARGET_POLL_FREQUENCY = 60;

int WIDTH = 470;
int HEIGHT = 500;
float target_poll_ms = 1.0f / TARGET_POLL_FREQUENCY;

double mouse_x = -100;
double mouse_y = -100;
bool mouse_click = false;
float brush_size = 10;
elemd::color brush_color = elemd::color("#ffffff");

struct brush_paint
{
    elemd::vec2 pos;
    elemd::color color;
    float size;
};

elemd::Image* cust;
std::vector<unsigned char> im_data;
std::vector<brush_paint> points;
std::vector<brush_paint> color_paints;

void draw(elemd::Context* ctx, elemd::Image* brush)
{
    for (auto& point : points)
    {
        ctx->set_fill_color(point.color);
        ctx->draw_image(point.pos.x(), point.pos.y(), point.size, point.size, brush, true);
    }

    ctx->set_fill_color(brush_color);
    ctx->draw_image((float)mouse_x - brush_size / 2.0f, (float)mouse_y - brush_size / 2.0f,
                    brush_size, brush_size, brush, true);

    if (mouse_click)
    {
        points.push_back({{(float)mouse_x - brush_size / 2.0f, (float)mouse_y - brush_size / 2.0f},
                          brush_color,
                          brush_size});
    }

    for (auto& point : color_paints)
    {
        ctx->set_fill_color(point.color);
        ctx->fill_circle(point.pos.x(), point.pos.y(), point.size);
    }

    ctx->draw_frame();
    ctx->present_frame();
}

int main(void)
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    elemd::WindowConfig winc = elemd::WindowConfig{"Painting App", WIDTH, HEIGHT};
    // winc.transparent = true;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    elemd::Image* brush = elemd::Image::create("./res/brush.png");
    ctx->_tmp_register_image(brush);

    win->add_mouse_click_listener([&](elemd::mouse_button_event event) {
        if (event.button == elemd::MOUSE_BUTTON_LEFT &&
            (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT))
        {
            mouse_click = true;
            elemd::vec2 p = elemd::vec2((float)event.x, (float)event.y);

            for (auto& point : color_paints)
            {
                if (p.distance(point.pos) < 10)
                {
                    mouse_click = false;
                    brush_color = point.color;
                    std::cout << "color " << brush_color.rgb() << std::endl;
                    break;
                }
            }
        }
        else
        {
            mouse_click = false;
        }

        draw(ctx, brush);
    });

    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        mouse_x = event.x;
        mouse_y = event.y;

        draw(ctx, brush);
    });

    win->add_scroll_listener([&](elemd::scroll_event event) {
        brush_size = std::clamp(brush_size + (float)event.yoffset, 1.0f, 300.0f);

        draw(ctx, brush);
    });

    ctx->_tmp_prepare();
    ctx->set_clear_color({255, 255, 255, 255});

    elemd::color color_palette[] = {
        elemd::color("#00b894"), elemd::color("#00cec9"), elemd::color("#0984e3"),
        elemd::color("#6c5ce7"), elemd::color("#b2bec3"), elemd::color("#fdcb6e"),
        elemd::color("#e17055"), elemd::color("#d63031"), elemd::color("#e84393"),
        elemd::color("#2d3436"),
    };

    for (int i = 0; i < 10; i++)
    {
        color_paints.push_back({{20 + 25 * i, 20}, color_palette[i], 10});
    }

    while (win->is_running())
    {
        // win->poll_events();
        win->wait_events();

        /*
        for (int y = 0; y < 100; y++)
        {
            for (int x = 0; x < 100; x++)
            {
                ctx->set_fill_color(color_palette[rand() % 10]);
                ctx->fill_rounded_rect(x * 6,y * 6, 5, 5, 3);
            }
        }

        ctx->set_fill_color({200, 0, 0, 100});
        ctx->fill_rect(0, 0, 100, 200);
        */

        // ctx->draw_frame();

        // std::this_thread::sleep_for(std::chrono::duration<float, std::ratio<1>>(target_poll_ms));
    }

    brush->destroy();
    win->destroy();

    return 0;
}