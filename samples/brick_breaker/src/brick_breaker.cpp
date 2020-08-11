#ifdef _MSC_VER
    #define EXPORT_API __declspec(dllexport)
#else
    #define EXPORT_API __attribute__((visibility("default")))
#endif


#include <iostream>
#include <vector>
#include <array>
#include <math.h> 

#include <elemd/window.hpp>
#include <elemd/context.hpp>
#include <elemd/color.hpp>
#include <elemd/image.hpp>
#include <elemd/vec2.hpp>


// Constants
const int TARGET_RENDER_FPS = 60;
const int TARGET_POLL_FPS = 30;

// Variables
float target_render_ms = 1.0f / TARGET_RENDER_FPS;
float target_poll_ms = 1.0f / TARGET_POLL_FPS;
double delta_time = 0;
double current_time = 0;
double last_time = 0;
double second_accumulator = 0;
double render_accumulator = 0;
double poll_accumulator = 0;
int frames = 0;
bool reload = false;

// Color Palette
elemd::color bg_color("#212121");
elemd::color puck_color("#03A9F4");
elemd::color ball_color("#D32F2F");


// Game
enum brick_types
{
    AIR = 0,
    BRICK = 1,
    DOUBLE_BRICK = 2,
    DIAMOND_BRICK = 3
};

enum power_up_type { 
    EXTTRA_LIFE = 0 
};

struct brick
{
    brick_types type;
    elemd::vec2 pos;
    int hitpoints = 0;
    elemd::color color;
};

struct power_up
{
    power_up_type type;
    elemd::vec2 pos;
    elemd::color color;
};

int lifes = 3;
int level = 0;
float brick_width = 38;
float brick_height = 18;

elemd::vec2 puck_pos;
elemd::vec2 puck_velocity;
float puck_speed = 100.0f;
float puck_width = 300;
float puck_height = 10;

elemd::vec2 ball_pos;
elemd::vec2 ball_velocity;
float ball_speed = 4;
float ball_radius = 10;

int WIDTH = 2 * 10 + 40 * 10;
int HEIGHT = 500;

const int MAP_ELEMENTS_X = 10;
const int MAP_ELEMENTS_Y = 8;

std::vector<std::vector<int>> levels;
std::vector<brick> bricks;
std::vector<power_up> power_pus;


void loadLevel();

void brickToBall()
{
    int i = 0;
    for (brick& b : bricks)
    {
        if ((ball_pos.x() + ball_radius > b.pos.x()) &&
            (ball_pos.x() - ball_radius < b.pos.x() + brick_width) &&
            (ball_pos.y() + ball_radius > b.pos.y()) &&
            (ball_pos.y() - ball_radius < b.pos.y() + brick_height))
        {
            if (ball_pos.y() < b.pos.y() || ball_pos.y() > b.pos.y() + brick_height)
            {
                ball_velocity.y() *= -1;
            }
            if (ball_pos.x() < b.pos.x() || ball_pos.x() > b.pos.x() + brick_width)
            {
                ball_velocity.x() *= -1;
            }
            --b.hitpoints;
            if (b.hitpoints <= 0)
            {
                if (b.type == BRICK)
                {
                    power_pus.push_back({EXTTRA_LIFE, b.pos, elemd::color(200, 20, 20)});
                }
                bricks.erase(bricks.begin() + i);

                // Check win state
                if (bricks.size() <= 0)
                {
                    ++level;
                    loadLevel();
                }
            }
            return;
        }
        ++i;
    }

}

void puckToBall()
{
    if ((ball_pos.x() + ball_radius > puck_pos.x()) &&
        (ball_pos.x() - ball_radius < puck_pos.x() + puck_width) &&
        (ball_pos.y() + ball_radius > puck_pos.y()) &&
        (ball_pos.y() - ball_radius < puck_pos.y() + puck_height))
    {
        if (ball_pos.y() < puck_pos.y() || ball_pos.y() > puck_pos.y() + brick_height)
        {
            ball_velocity.y() *= -1;
        }
        if (ball_pos.x() < puck_pos.x() || ball_pos.x() > puck_pos.x() + brick_width)
        {
            ball_velocity.x() *= -1;
        }
    }
}

void resetBall()
{
    puck_pos.y() = HEIGHT - 15 - puck_height;
    puck_pos.x() = WIDTH / 2.0f - puck_width / 2.0f;

    ball_pos = elemd::vec2(WIDTH / 2.0f - ball_radius, HEIGHT - 10 - puck_height - 20);
    ball_velocity = elemd::vec2(1) * ball_speed;
}

void gameReset()
{
    lifes = 5;
    loadLevel();
}

void loadLevel()
{
    bricks.clear();
    power_pus.clear();
    resetBall();

    for (int i = 0; i < MAP_ELEMENTS_X; ++i)
    {
        for (int j = 0; j < MAP_ELEMENTS_Y; j++)
        {
            switch (levels[level][j * MAP_ELEMENTS_X + i])
            {
            case BRICK:
                bricks.push_back(
                    {BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 1, elemd::color("#607D8B")});
                break;
            case DOUBLE_BRICK:
                bricks.push_back({DOUBLE_BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 2,
                                  elemd::color("#9C27B0")});
                break;
            case DIAMOND_BRICK:
                bricks.push_back({DOUBLE_BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 4,
                                  elemd::color("#7C4DFF")});
                break;
            }
        }
    }
}


extern "C"
{
    EXPORT_API void app_init()
    {
        levels.push_back({0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                          0, 1, 1, 1, 2, 2, 1, 1, 1, 0,
                          1, 1, 1, 2, 3, 3, 2, 1, 1, 1,
                          1, 1, 1, 1, 2, 2, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 1, 0, 1, 0, 1, 0, 1, 0, 0});

        levels.push_back({0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                          0, 1, 1, 1, 2, 2, 1, 1, 1, 0,
                          1, 1, 1, 2, 3, 3, 2, 1, 1, 1,
                          1, 1, 1, 1, 2, 2, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
                          2, 3, 2, 3, 2, 3, 2, 3, 2, 3,
                          0, 1, 0, 0, 0, 1, 0, 0, 0, 0});
    }

    EXPORT_API void reload_notify()
    {
        reload = true;
    }

    EXPORT_API int app_run(elemd::Window* win, elemd::Context* ctx)
    {
        WIDTH = win->get_width();
        HEIGHT = win->get_height();

        ctx->set_clear_color(bg_color);

         win->add_key_listener([&](elemd::key_event event) {            
            if (event.key == elemd::KEY_LEFT)
            {
                puck_velocity.x() += -1 * puck_speed * poll_accumulator;
            }
            else if (event.key == elemd::KEY_RIGHT)
            {
                puck_velocity.x() += 1 * puck_speed * poll_accumulator;
            }
        });

        loadLevel();

        // Main Loop
        while (win->is_running() && !reload)
        {
            // Timing
            current_time = elemd::Window::now();
            delta_time = (current_time - last_time);
            last_time = current_time;
            second_accumulator += delta_time;
            render_accumulator += delta_time;
            poll_accumulator += delta_time;

            if (second_accumulator >= 1.0)
            {
                std::cout << frames << std::endl;
                frames = 0;
                second_accumulator = 0;
            }

            if (poll_accumulator >= target_poll_ms)
            {
                // Poll Events
                win->poll_events();
                poll_accumulator = 0;
            }

            if (render_accumulator >= target_render_ms)
            {   
                // Bricks

                for (brick& b : bricks)
                {
                    ctx->set_fill_color(b.color);
                    ctx->fill_rounded_rect(b.pos.x(), b.pos.y(), brick_width, brick_height, 2);
                }


                // Puck
                puck_velocity.x() = puck_velocity.x() / (70.0f * render_accumulator);
                puck_pos = puck_pos + puck_velocity;
                if (puck_pos.get_x() > WIDTH - puck_width)
                {
                    puck_pos.x() = WIDTH - puck_width;
                    puck_velocity.x() = 0;
                }
                else if (puck_pos.get_x() < 0)
                {
                    puck_pos.x() = 0;
                    puck_velocity.x() = 0;
                }

                ctx->set_fill_color(puck_color);
                ctx->fill_rect(puck_pos.x(), puck_pos.y(), puck_width, puck_height);


                // Power Up

                int pcnt = 0;
                for (power_up& pu : power_pus)
                {
                    pu.pos = pu.pos + elemd::vec2(0, 80) * render_accumulator;


                    ctx->set_fill_color(pu.color);
                    ctx->fill_rounded_rect(pu.pos.x(), pu.pos.y(), 20, 10, 1);
                }

                for (int i = power_pus.size()-1; i >= 0; --i)
                {
                    if (power_pus[i].pos.y() > HEIGHT)
                    {
                        power_pus.erase(power_pus.begin() + i);
                    }

                }


                // Ball

                ball_pos = ball_pos + ball_velocity;
                if (ball_pos.get_x() > WIDTH - ball_radius)
                {
                    ball_pos.x() = WIDTH - ball_radius;
                    ball_velocity.x() *= -1;
                }
                else if (ball_pos.get_x() < ball_radius)
                {
                    ball_pos.x() = ball_radius;
                    ball_velocity.x() *= -1;
                }
                else if (ball_pos.get_y() < ball_radius)
                {
                    ball_pos.y() = ball_radius;
                    ball_velocity.y() *= -1;
                }
                else if (ball_pos.get_y() > HEIGHT - ball_radius)
                {
                    --lifes;
                    if (lifes < 0)
                    {
                        gameReset();
                    }
                    else
                    {
                        resetBall();
                    }
                }

                
                
                brickToBall();
                puckToBall();

                ctx->set_fill_color(ball_color);
                ctx->fill_circle(ball_pos.x(), ball_pos.y(), ball_radius);


                // Lifes
                ctx->set_fill_color(ball_color);
                for (int i = 0; i < lifes; ++i)
                {
                    ctx->fill_circle(WIDTH - 5 - (i*8), HEIGHT - 5, 3);    
                }

                ctx->draw_frame();

                ++frames;
                render_accumulator = 0;
            }
        }

        if (reload)
        {
            return 1;
        }

        win->destroy();

        return 0;
    }
}