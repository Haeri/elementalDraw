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

elemd::image* cust;
std::vector<unsigned char> im_data;
std::vector<elemd::vec2> points;

void draw(elemd::Context* ctx)
{
    ctx->set_fill_color({30, 30, 30});

    for (auto& point : points)
    {
        ctx->fill_circle(point.x(), point.y(), 3);
    }

    ctx->fill_circle(mouse_x, mouse_y, 4);

    if (mouse_click)
    {
        points.push_back({(float)mouse_x, (float)mouse_y});
    }

//    ctx->draw_image(0, 0, cust->get_width(), cust->get_height(), cust);

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

        //draw(ctx);
    });



    //elemd::image* img = elemd::image::create("./elemd_res/elemd_icon.png");
    //ctx->_tmp_register_image(img);

    /*
    int im_width = 200;
    int im_height = 200;
    unsigned char ardata[] = {233, 456, 111}; 

    im_data.reserve(im_width * im_height * 4);
    for (int y = 0; y < im_height; ++y)
    {
        for (int x = 0; x < im_width; ++x)
        {
            im_data.push_back(255);
            im_data.push_back(0);
            im_data.push_back(0);
            im_data.push_back(255);
        }
    }
    */
    //cust = elemd::image::create(im_width, im_height, 4, im_data.data());
    //ctx->_tmp_register_image(cust);

     
    ctx->_tmp_prepare();
    ctx->set_clear_color({30, 30, 30});


    elemd::color color_palette[] = {
        elemd::color("#5ccccf"),
        elemd::color("#77d3d0"),
        elemd::color("#c0d6c8"),
        elemd::color("#d6d6bc"),
        elemd::color("#e6cdae"),
        elemd::color("#f1c5a0"),
        elemd::color("#f8bc92"),
        elemd::color("#ffa781"),
        elemd::color("#ff9f83"),
        elemd::color("#fe6786"),
    };


    while (win->is_running())
    {
        win->poll_events();
        //win->wait_events();


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


        ctx->draw_frame();
        
        //std::this_thread::sleep_for(std::chrono::duration<float, std::ratio<1>>(target_poll_ms));
    }

    //cust->destroy();
    //img->destroy();
    win->destroy();


    return 0;
}