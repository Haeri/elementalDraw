#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <elemd/video.hpp>
#include <math.h> 
#include <algorithm>
#include <vector>
#include <functional>


enum State
{
    IN,
    SLIDE,
    OUT
};

struct Slide
{
    std::function<void(Slide*, float)> draw;
    float transition_in = 0;
    float transition_out = 0;
    
    State state = IN;
    float in_progress = 0;
    float out_progress = 0;
};


struct MorphSlide : Slide
{
    int state = 0;
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


    // Load video
    //elemd::video* sea = elemd::video::create("./res/Sea.mp4");

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
    slides.push_back({[&](Slide* s, float dt) {
                          if (s->state == IN)
                          {
                              float p = s->in_progress / s->transition_in;
                              ctx->draw_image(30, 30 + (20 - p*20), 300, 300, img);
                          }
                          else
                          {
                              ctx->draw_image(30, 30, 300, 300, img);
                          }
                      },
                      0.1f});
    slides.push_back({[&](Slide* s, float dt) { ctx->draw_image(0, 0, 300, 300, discrod); }, 3});
    slides.push_back({[&](Slide* s, float dt) { ctx->draw_image(0, 0, 300, 300, github); }, 3});

   
    // Event
    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {

    });

    win->add_key_listener([&](elemd::key_event event) {
        
        if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
        {
            if (event.key == elemd::KEY_LEFT)
            {
                if (index > 0)
                {
                    --index;
                    slides[index].state = IN;
                }
            }
            if (event.key == elemd::KEY_RIGHT)
            {
                if (index < slides.size() - 1)
                {
                    ++index;
                    slides[index].state = IN;
                }
            }
        }

        if (event.action == elemd::ACTION_PRESS && event.key == elemd::KEY_P) {
            win->set_fullscreen(!win->is_fullscreen());
        }
    });

    ctx->_tmp_prepare();
//    ctx->set_clear_color(dark);

    double last_time = win->now();
    float dt = 0;

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        if (slides[index].state == IN && slides[index].transition_in > 0)
        {
            dt = win->now() - last_time;
            slides[index].in_progress += dt; 
            last_time = win->now();

            if (slides[index].in_progress >= slides[index].transition_in)
            {
                slides[index].state = SLIDE;
                slides[index].in_progress = 0;
            }
        }
        slides[index].draw(&slides[index], dt);
        

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