#include <iostream>
#include<SDL.h>
#include<math.h>
#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff 
#define COLOR_BLACK 0x00000000
#define RAY_COLOR 0xfc9003
#define RAY_THICKNESS 2
#define COLOR_GRAY  0xefefefef
#define NUMBER_OF_RAYS 500
struct Circle
{
    double x;
    double y;
    double r;
};

struct Ray {
    double x_start, y_start;
    double angle;
};


void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_squared = pow((x - circle.x), 2) + pow((y - circle.y), 2);
            if (distance_squared < radius_squared)
            {
                SDL_Rect pixel = { x,y,1,1 };
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}


void generate_rays(struct Circle circle, struct Ray rays[NUMBER_OF_RAYS])
{
    for (int i = 0; i < NUMBER_OF_RAYS; i++)
    {
        double angle = ((double)i / NUMBER_OF_RAYS) * 2 * M_PI;
        struct Ray ray = { circle.x, circle.y, angle };
        rays[i] = ray;
    }
}

void FillRays(SDL_Surface* surface, struct Ray rays[NUMBER_OF_RAYS], Uint32 color,  struct Circle object)
{
    double radius_squared = pow(object.r, 2);
    for (int i = 0; i < NUMBER_OF_RAYS; i++)
    {
        struct Ray ray = rays[i];
        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;

        double x_draw = ray.x_start;
        double y_draw = ray.y_start;

        while (!end_of_screen && !object_hit)
        {
            x_draw = x_draw + step * cos(ray.angle);
            y_draw = y_draw + step * sin(ray.angle);
            
            SDL_Rect pixel = { x_draw,y_draw,RAY_THICKNESS,RAY_THICKNESS };
            
           
            
            SDL_FillRect(surface, &pixel, color);

            if (x_draw<0 || x_draw > WIDTH)
                end_of_screen = 1;
            if (y_draw<0 || y_draw > HEIGHT)
                end_of_screen = 1;

            double distance_squared = pow((x_draw - object.x), 2) + pow((y_draw - object.y), 2);
            if (distance_squared < radius_squared)
            {
                break;
            }

        }
    }
}

void move_shadow_circle(struct Circle* shadow_circle)
{
    shadow_circle->x += 1;
    shadow_circle->y += 1;
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
     
    SDL_Window* window =    SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = { 200, 200, 50 };
    struct Circle shadow_circle = { 550, 300, 140 };
    SDL_Rect erase_rect = { 0,0,WIDTH,HEIGHT };
    struct Ray rays[NUMBER_OF_RAYS];
    generate_rays(circle, rays);

    double obstacle_speed_y = 1;

    int simulation_running = 1;
    SDL_Event event;
    while (simulation_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                simulation_running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(circle, rays);
            } 
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        FillRays(surface, rays, RAY_COLOR, shadow_circle);
        FillCircle(surface, circle, COLOR_WHITE);
        FillCircle(surface, shadow_circle, COLOR_WHITE);
        
        shadow_circle.y += obstacle_speed_y;

        if (shadow_circle.y - shadow_circle.r < 0)
        {
            obstacle_speed_y = -obstacle_speed_y;
        }

        if (shadow_circle.y + shadow_circle.r > HEIGHT)
        {
            obstacle_speed_y = -obstacle_speed_y;
        }
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }


    SDL_Event event2;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event2)) {
            if (event2.type == SDL_QUIT) {
                running = false;
            }
            
        }
    }
    return 0;
}