#include <iostream>
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
elemd::color bg_color("#1b262c");
elemd::color puck_color("#ff5f40");
elemd::color brick_color("#24a19c");
elemd::color ball_color("#d92027");


// Game
enum brick_types
{
    AIR = 0,
    BRICK = 1,
    DOUBLE_BRICK = 2,
    DIAMOND_BRICK = 3
};

struct brick
{
    brick_types type;
    elemd::vec2 pos;
    int hit = 0;
    elemd::color color;
};

float brick_width = 38;
float brick_height = 18;

elemd::vec2 puck_pos;
elemd::vec2 puck_velocity;
float puck_speed = 100.0f;
float puck_width = 60;
float puck_height = 10;

elemd::vec2 ball_pos;
elemd::vec2 ball_velocity;
float ball_speed = 4;
float ball_radius = 10;

int WIDTH = 2 * 10 + 40 * 10;
int HEIGHT = 500;

const int MAP_ELEMENTS_X = 10;
const int MAP_ELEMENTS_Y = 6;

int brick_map[MAP_ELEMENTS_Y][MAP_ELEMENTS_X] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 2, 2, 1, 1, 1, 0}, 
    {1, 1, 1, 2, 3, 3, 2, 1, 1, 1},
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 0, 0, 0, 0, 1, 1, 1}
};

std::vector<brick> bricks;

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
            --b.hit;
            if (b.hit <= 0)
            {
                bricks.erase(bricks.begin() + i);
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

void loadLevel()
{
    bricks.clear();

    puck_pos.y() = HEIGHT - 10 - puck_height;
    puck_pos.x() = WIDTH / 2.0f - puck_width / 2.0f;

    ball_pos = elemd::vec2(WIDTH / 2.0f - ball_radius, HEIGHT - 10 - puck_height - 20);
    ball_velocity = elemd::vec2(1) * ball_speed;

    for (int i = 0; i < MAP_ELEMENTS_X; ++i)
    {
        for (int j = 0; j < MAP_ELEMENTS_Y; j++)
        {
            switch (brick_map[j][i])
            {
            case BRICK:
                bricks.push_back(
                    {BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 1, elemd::color("#3797a4")});
                break;
            case DOUBLE_BRICK:
                bricks.push_back({DOUBLE_BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 2,
                                  elemd::color("#d92027")});
                break;
            case DIAMOND_BRICK:
                bricks.push_back({DOUBLE_BRICK, elemd::vec2(10 + i * 40, 10 + j * 20), 4,
                                  elemd::color("#35d0ba")});
                break;
            }
        }
    }
}

extern "C"
{
    __declspec(dllexport) void app_init()
    { 
    }

    __declspec(dllexport) void reload_notify()
    {
        reload = true;
    }

    __declspec(dllexport) int app_run(elemd::Window* win, elemd::Context* ctx)
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
                    loadLevel();
                }
                
                brickToBall();
                puckToBall();

                ctx->set_fill_color(ball_color);
                ctx->fill_circle(ball_pos.x(), ball_pos.y(), ball_radius);


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