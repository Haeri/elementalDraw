#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <thread>


const int GRID_WIDTH = 700;
const int GRID_HEIGHT = 450;
const int CELL_SIZE = 2;

int grid1[GRID_WIDTH * GRID_HEIGHT] = {0};
int grid2[GRID_WIDTH * GRID_HEIGHT] = {0};
int* current_grid = grid1;
int* next_grid = grid2;
bool flip = false;

bool mouse_click = false;
float mouse_x = 0;
float mouse_y = 0;

bool playing = false;
int generation = 0;
int population = 0;

void draw()
{
    if (mouse_click)
    {
        int index = (int)(mouse_x / CELL_SIZE) + GRID_WIDTH * (int)(mouse_y / CELL_SIZE);
        if (index >= 0)
        {
            grid1[index] = 2;
            grid2[index] = 2;
            
            ++population;
        }
    }
}

int main(void)
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif


    // Configure and create window
    elemd::WindowConfig winc =
        elemd::WindowConfig{"Game Of Life", GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE};
    //winc.transparent = true;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    ctx->_tmp_prepare();
    ctx->set_clear_color(elemd::color(30, 30, 30));



    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        if (i & 7 == 0 || i % 13 == 0 || i % 40 == 0 || i % 39 == 0 || i % 17 == 0 || i % 23 == 0)
        {
            grid1[i] = 1;
            ++population;
        }
    }

    

    // Events
    win->add_mouse_click_listener(
        [&](elemd::mouse_button_event event) { 
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

        draw();
    });

    win->add_key_listener([&](elemd::key_event event) {
        if (event.key == elemd::KEY_SPACE && event.action == elemd::ACTION_PRESS)
        {
            playing = !playing;
        }
    });

    ctx->set_font_size(9);
    double last_time = 0;
    
    // Main loop
    while (win->is_running())
    {
        win->poll_events();
                
        
        // Tick
        if (playing)
        {

            //if (win->now() - last_time < 0.02f)
            //    continue;
        
            //last_time = win->now();
            population = 0;
            if (flip)
            {
                current_grid = grid2;
                next_grid = grid1;
            }
            else
            {
                current_grid = grid1;
                next_grid = grid2;
            }
            flip = !flip;

            for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
            {
                int x = i % GRID_WIDTH;
                int y = i / GRID_WIDTH;

                int neighbors = 0;
                if (x == 0 || x == GRID_WIDTH - 1 || y == 0 || y == GRID_HEIGHT - 1)
                {
                }
                else
                {
                    neighbors += current_grid[i - 1] >= 1 ? 1 : 0;
                    neighbors += current_grid[i + 1] >= 1 ? 1 : 0;
                    neighbors += current_grid[i - GRID_WIDTH] >= 1 ? 1 : 0;
                    neighbors += current_grid[i + GRID_WIDTH] >= 1 ? 1 : 0;
                    neighbors += current_grid[(i - GRID_WIDTH) - 1] >= 1 ? 1 : 0;
                    neighbors += current_grid[(i - GRID_WIDTH) + 1] >= 1 ? 1 : 0;
                    neighbors += current_grid[(i + GRID_WIDTH) - 1] >= 1 ? 1 : 0;
                    neighbors += current_grid[(i + GRID_WIDTH) + 1] >= 1 ? 1 : 0;
                }

                if (current_grid[i] < 1 && neighbors == 3)
                {
                    // Spawn
                    next_grid[i] = 2;
                }
                else if (current_grid[i] >= 1 && (neighbors < 2 || neighbors > 3))
                {
                    // Die
                    next_grid[i] = -1;
                }
                else
                {
                    if (current_grid[i] == -1)
                    {
                        next_grid[i] = 0;
                    }
                    else if (current_grid[i] == 2)
                    {
                        next_grid[i] = 1;
                    }
                    else
                    {
                        next_grid[i] = current_grid[i];
                    }
                }

                population += next_grid[i];
            }

            ++generation;
        }

        // Draw
        for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
        {
            int x = i % GRID_WIDTH;
            int y = i / GRID_WIDTH;


            if (current_grid[i])
            {
                int rad = (CELL_SIZE / 2.0f) - 1;

                if (current_grid[i] == -1)
                {
                    ctx->set_fill_color(elemd::color(250, 30, 30, 200));
                }
                else if (current_grid[i] == 2)
                {
                    ctx->set_fill_color(elemd::color(30, 250, 30, 200));
                }
                else
                {
                    ctx->set_fill_color(elemd::color(220, 220, 220));
                }
                //ctx->fill_circle(x * CELL_SIZE + rad, y * CELL_SIZE + rad, rad);
                ctx->fill_rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE-1, CELL_SIZE-1);
            }
         
            /*
            ctx->draw_text(x * CELL_SIZE, y * CELL_SIZE,
                           std::to_string(i) + " (" + std::to_string(x) + ", " +
            std::to_string(y) + ")");
              */             
        }

        ctx->set_font_size(10);
        ctx->set_fill_color(elemd::color("#1fc600"));
        ctx->draw_text(2, 1,
                        "// generation: " + std::to_string(generation) +
                        " | population: " + std::to_string(population) + 
                        " | delta: " + std::to_string((int)((win->now() - last_time) * 1000)) + "ms");

        last_time = win->now();

        ctx->draw_frame();
        ctx->present_frame();
    }

    // Cleanup
    win->destroy();

    return 0;
}