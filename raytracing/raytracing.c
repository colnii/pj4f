/*  
    C语言从零实现光线追踪，效果惊艳 | Daniel Hirsch  https://www.bilibili.com/video/BV1Qq7m6PEuA
    
    Coding Ray Tracing in C  --Daniel Hirsch  https://www.youtube.com/watch?v=2BLRLuczykM
    需要安装SDL2库
    编译：cc raytracing.c -o raytracing -lSDL2 -lm 启动：./raytracing */
#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>

#define WIDTH 1200
#define HEIGHT 900
#define COLOR_WHITE 0xffffff
#define COLOR_BLACK 0x000000
#define COLOR_GREY 0xefefef
#define RAY_COLOR 0xffd43b
#define RAYS_NUMBER 600
#define M_PI 3.14159265358979323846
#define RAY_THINKNESS 1

struct Circle
{
    double x,y;
    double r;
};

struct Ray
{
    double x_t, y_t;
    double angle;
};


void FillCircle(SDL_Surface* psurface, struct Circle* pcircle, Uint32 color)
{
    double r_squared = pow(pcircle->r,2);
    for(double i = pcircle->x - pcircle->r; i <= pcircle->x + pcircle->r; ++i)
    {
        for(double j = pcircle->y - pcircle->r; j <= pcircle->y + pcircle->r; ++j)
        {
            double distance_squared = pow(i-pcircle->x,2)+pow(j-pcircle->y,2);
            if( distance_squared <= r_squared)
            {
                SDL_Rect pixel = (SDL_Rect) {i,j,1,1};
                SDL_FillRect(psurface,&pixel,color);
            }
        }
    }
}

void generate_rays(struct Circle* pcircle, struct Ray rays[RAYS_NUMBER])
{
    for(int i = 0; i < RAYS_NUMBER; ++i)
    {
        double angle = ((double) i / RAYS_NUMBER) * 2 * M_PI;
        struct Ray ray = {pcircle->x, pcircle->y, angle};
        rays[i] = ray;
    }
}

void FillRays(SDL_Surface* psurface, struct Ray rays[RAYS_NUMBER], Uint32 color, struct Circle* pobject)
{
    double r_squared = pow(pobject->r,2);
    for(int i = 0; i < RAYS_NUMBER; ++i)
    {
        struct Ray ray = rays[i];
        
        int end_of_screen = 0;
        int object_hit = 0;
        double step = 1;
        double x_draw = ray.x_t, y_draw = ray.y_t;
        while( !end_of_screen && !object_hit)
        {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);
            SDL_Rect ray_point = (SDL_Rect) {x_draw,y_draw,RAY_THINKNESS,RAY_THINKNESS};
            SDL_FillRect(psurface,&ray_point,color);
            if(x_draw < 0 || y_draw < 0 || x_draw > WIDTH || y_draw > HEIGHT)
            {
                end_of_screen = 1;
            }
            double distance_squared = pow(x_draw-pobject->x,2)+pow(y_draw-pobject->y,2);
            if( distance_squared <= r_squared)
            {
                object_hit = 1;
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* pwindow = SDL_CreateWindow("Raytracing",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* psurface = SDL_GetWindowSurface(pwindow);

    SDL_Rect erase_rect = (SDL_Rect) {0,0,WIDTH,HEIGHT};
    struct Circle circle = {200,200,40};
    struct Circle shadow_circle = {650,300,140};

    struct Ray rays[RAYS_NUMBER];    
    generate_rays(&circle, rays);

    int simulation_running = 1;
    double step = 0;
    SDL_Event event;
    while(simulation_running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                simulation_running = 0;
            }
            if(event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(&circle, rays);
            }
        }
        SDL_FillRect(psurface, &erase_rect, COLOR_BLACK);
        FillRays(psurface, rays, RAY_COLOR,&shadow_circle);
        FillCircle(psurface, &circle, COLOR_WHITE);

        FillCircle(psurface, &shadow_circle, COLOR_WHITE);

        shadow_circle.y += 5*sin(step);
        step += 0.05;

        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(10);
    }
}