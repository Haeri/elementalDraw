#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <algorithm>
#include <vector>
#include <functional>
 
struct Slide
{
    std::function<void()> draw;

};


int main(void)
{
 
    // Configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Presenter", 600, 500};
    winc.icon_file = "./res/logo.png";
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // Load image
    elemd::image* img = elemd::image::create("./res/logo.png");
    elemd::image* discrod = elemd::image::create("./res/discord_logo.png");
    elemd::image* github = elemd::image::create("./res/github_screen.png");
    elemd::image* messenger = elemd::image::create("./res/messenger_logo.png");
    elemd::image* messenger2 = elemd::image::create("./res/messenger_screen.png");
    elemd::image* skype = elemd::image::create("./res/skype_logo.png");
    elemd::image* skype2 = elemd::image::create("./res/skype_screen.gif");
    elemd::image* slack = elemd::image::create("./res/slack_screen.png");
    elemd::image* vc = elemd::image::create("./res/vscode_screen.png");
    elemd::image* whatsapp = elemd::image::create("./res/whatsapp_screen.jpg");
    elemd::image* teams = elemd::image::create("./res/teams_screen.jpg");


    ctx->_tmp_register_image(img);
    ctx->_tmp_register_image(discrod);
    ctx->_tmp_register_image(github);
    ctx->_tmp_register_image(messenger);
    ctx->_tmp_register_image(messenger2);
    ctx->_tmp_register_image(skype);
    ctx->_tmp_register_image(skype2);
    ctx->_tmp_register_image(slack);
    ctx->_tmp_register_image(vc);
    ctx->_tmp_register_image(whatsapp);
    ctx->_tmp_register_image(teams);

    int index = 0;
    std::vector<Slide> slides;
    slides.push_back({[&](void) { ctx->draw_image(0, 0, 300, 300, img); }});
    slides.push_back({[&](void) { ctx->draw_image(0, 0, 300, 300, discrod); }});
    slides.push_back({[&](void) { ctx->draw_image(0, 0, 300, 300, github); }});

   
    // Event
    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {

    });

    win->add_key_listener([&](elemd::key_event event) {
        
        if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
        {
            if (event.key == elemd::KEY_LEFT)
            {
                if (index > 0)
                --index;
            }
            if (event.key == elemd::KEY_RIGHT)
            {
                if (index < slides.size()-1)
                ++index;
            }
        }

        if (event.action == elemd::ACTION_PRESS && event.key == elemd::KEY_P) {
            win->set_fullscreen(!win->is_fullscreen());
        }
    });

    ctx->_tmp_prepare();
//    ctx->set_clear_color(dark);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();
        

        //ctx->draw_image(0, 0, 30, 30, img);
        //ctx->draw_image(40, 0, 30, 30, discrod);
        //ctx->draw_image(80, 0, 30, 30, github);
        //ctx->draw_image(120, 0, 30, 30, messenger);
        //ctx->draw_image(160, 0, 30, 30, messenger2);
        //ctx->draw_image(200, 0, 30, 30, skype);
        //ctx->draw_image(0, 40, 30, 30, slack);
        //ctx->draw_image(40, 40, 30, 30, vc);
        //ctx->draw_image(80, 40, 30, 30, whatsapp);
        //ctx->draw_image(120, 40, 30, 30, skype2);
        //ctx->draw_image(160, 40, 30, 30, teams);
        slides[index].draw();

        ctx->draw_frame();
    }

    // Cleanup
    img->destroy();
    discrod->destroy();
    github->destroy();
    messenger->destroy();
    messenger2->destroy();
    skype->destroy();
    skype2->destroy();
    slack->destroy();
    vc->destroy();
    whatsapp->destroy();
    teams->destroy();

    win->destroy();

    return 0;
}