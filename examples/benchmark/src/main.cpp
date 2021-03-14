#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <thread>
#include <vector>

const int GRID_WIDTH = 900;
const int GRID_HEIGHT = 600;
const int CELL_SIZE = 2;

const int END_GENERATION = 8200;

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

double starting_time = 0;

std::vector<double> stats;

int main(void)
{

    // Configure and create window
    elemd::WindowConfig winc =
        elemd::WindowConfig{"Benchmark", GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE};
    winc.native_pixel_size = true;
    winc.resizeable = false;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    ctx->_tmp_prepare();
    ctx->set_clear_color(elemd::color(20, 20, 20));

    std::cout << "-------------- STARTING BENCHMARK --------------\n";
    
    std::cout << "ELEMD:   " << ELEMD_VERSION << "\n";
    std::cout << "BACKEND: " << ELEMD_BACKEND << "\n";
    std::cout << "WIDTH:   " << win->get_width() << "\n";
    std::cout << "HEIGHT:  " << win->get_height() << "\n";

    stats.reserve(END_GENERATION);


    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        if (i & 7 == 0 || i % 13 == 0 || i % 40 == 0 || i % 39 == 0 || i % 17 == 0 || i % 23 == 0)
        {
            grid1[i] = 1;
            ++population;
        }
    }

    

    // Events
    win->add_key_listener([&](elemd::key_event event) {
        if (event.key == elemd::KEY_SPACE && event.action == elemd::ACTION_PRESS)
        {
            if (!playing)
            {
                playing = true;
                starting_time = win->now();
            }
        }
    });

//    ctx->set_font_size(9);
    double last_time = 0;
    
    // Main loop
    while (win->is_running() && generation <= END_GENERATION)
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

            
            stats.push_back(win->now() - last_time);
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
        /*
        ctx->set_font_size(30);
        ctx->set_fill_color(elemd::color("#1fc600"));
        ctx->draw_text(2, 20,
                        "// generation: " + std::to_string(generation) +
                        " | population: " + std::to_string(population) + 
                        " | delta: " + std::to_string((int)((win->now() - last_time) * 1000)) + "ms");
          */              
        last_time = win->now();

        ctx->draw_frame();


    }

    int time = (int)((win->now() - starting_time) * 1000);
    std::cout << "---------------------------------------------\n";
    std::cout << "FINAL TIME:       " << time << "ms\n";
    std::cout << "FINAL GENERATION: " << generation << "\n";
    std::cout << "FINAL POPULATION: " << population << "\n";
    std::cout << "AVERAGE MS:       " << (time / (float)generation) << "\n";
    std::cout << "-------------- ENDING BENCHMARK --------------\n";

    for (double &i : stats)
    {
        std::cout << i*1000 << std::endl;
    }

    // Cleanup
    win->destroy();

    return 0;
}