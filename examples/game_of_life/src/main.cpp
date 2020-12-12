#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <thread>


const int GRID_WIDTH = 300;
const int GRID_HEIGHT = 200;
const int CELL_SIZE = 4;
bool grid1[GRID_WIDTH * GRID_HEIGHT] = {0};
bool grid2[GRID_WIDTH * GRID_HEIGHT] = {0};
bool* current_grid = grid1;
bool* next_grid = grid2;
bool* grid;
bool flip = false;

bool mouse_click = false;
float mouse_x = 0;
float mouse_y = 0;
bool playing = false;

void draw()
{
    if (mouse_click)
    {
        int index = (int)(mouse_x / CELL_SIZE) + GRID_WIDTH * (int)(mouse_y / CELL_SIZE);
        if (index >= 0)
        {
            grid1[index] = 1;
            grid2[index] = 1;
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
    winc.transparent = true;
    winc.icon_file = "./res/logo.png";
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    ctx->_tmp_prepare();
    ctx->set_clear_color(elemd::color(0, 0, 0, 30));
    

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

            if (win->now() - last_time < 0.08f)
                continue;
        
            last_time = win->now();
        
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
                    neighbors += current_grid[i - 1];
                    neighbors += current_grid[i + 1];
                    neighbors += current_grid[i - GRID_WIDTH];
                    neighbors += current_grid[i + GRID_WIDTH];
                    neighbors += current_grid[(i - GRID_WIDTH) - 1];
                    neighbors += current_grid[(i - GRID_WIDTH) + 1];
                    neighbors += current_grid[(i + GRID_WIDTH) - 1];
                    neighbors += current_grid[(i + GRID_WIDTH) + 1];
                }

                if (!current_grid[i] && neighbors == 3)
                {
                    // Spawn
                    next_grid[i] = 1;
                }
                else if (current_grid[i] && (neighbors < 2 || neighbors > 3))
                {
                    // Life
                    next_grid[i] = 0;
                }
                else
                {
                    next_grid[i] = current_grid[i];
                }
            }
        }

        // Draw
        ctx->set_fill_color(elemd::color(220, 220, 220));
        for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
        {
            int x = i % GRID_WIDTH;
            int y = i / GRID_WIDTH;


            if (current_grid[i])
            {
                int rad = CELL_SIZE / 2 - 1;
                ctx->fill_circle(x * CELL_SIZE + rad, y * CELL_SIZE + rad, rad);
            }
         
            /*
            ctx->draw_text(x * CELL_SIZE, y * CELL_SIZE,
                           std::to_string(i) + " (" + std::to_string(x) + ", " +
            std::to_string(y) + ")");
              */             
        }

        ctx->draw_frame();
    }

    // Cleanup
    win->destroy();

    return 0;
}