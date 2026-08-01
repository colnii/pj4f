/*  Coding a Starfield Animation in C --Daniel Hirsch https://www.youtube.com/watch?v=tv_0RV4FKvA 
    【中文字幕】用 C 编写星空动画：  https://www.bilibili.com/video/BV1G3Ns66E2S
    需要安装SDL2库
    编译：cc starfield.c -o starfield -lSDL2 -lm 启动：./starfield */

#include<SDL2/SDL.h>
#include<stdlib.h>
#include<math.h>

#define COLOR_WHITE 0xffffff
#define COLOR_BLACK 0x000000
#define WIDTH 900
#define HEIGHT 600
#define STAR_COUNT 300

struct Star
{
    double x,y;
    double vx,vy;
    double speedFactor;
};

struct Star new_star()
{
    double x = ((double) rand() / RAND_MAX * WIDTH - WIDTH/2);
    double y = ((double) rand() / RAND_MAX * HEIGHT - HEIGHT/2);
    // double vx = ((double) rand()) / RAND_MAX - 0.5;
    // double vy = ((double) rand()) / RAND_MAX - 0.5;
    double speedFactor = ((double)rand() / RAND_MAX)*0.5;
    return (struct Star) {x,y,0,0,speedFactor};
}

int move_stars(SDL_Surface *psurface, struct Star *pstars)
{
    for(int i = 0; i < STAR_COUNT; ++i)
    {
        struct Star *pstar = &pstars[i];
        pstar->x = pstar->x + pstar->vx; 
        pstar->y = pstar->y + pstar->vy;
        pstar->vx = pstar->speedFactor * pstar->x / 200;
        pstar->vy = pstar->speedFactor * pstar->y / 200;
        int size = 1 + (int)(6 * pstar->speedFactor);
        SDL_Rect rect = {pstar->x+WIDTH/2,pstar->y+HEIGHT/2,size,size};
        SDL_FillRect(psurface, &rect, COLOR_WHITE);
        if(fabs(pstar->x) > WIDTH / 2 || fabs(pstar->y) > HEIGHT / 2)
        {
            *pstar = new_star();
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *pwindow = SDL_CreateWindow("starfield",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);
    int simulation_runing = 1;
    struct Star stars[STAR_COUNT];
    for(int i = 0; i < STAR_COUNT; ++i)
    {
        stars[i] = new_star();
    }
    while(simulation_runing)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.quit.type == SDL_QUIT)
            {
                simulation_runing = 0;
            }
        }
        SDL_Rect black_rect = {0,0,WIDTH,HEIGHT};
        SDL_FillRect(psurface,&black_rect,COLOR_BLACK);
        move_stars(psurface, stars);
        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(5);
    }
}