#include<stdio.h>
#include<raylib.h>

#define WIDTH 900
#define HEIGHT 600
#define MAX_WAVES 1000
#define WAVE_SPEED 100
#define CAR_SPEED 0/1.41
#define CAR_ACCELERATION 20

int curr_waves = 0;

struct Car{
    float x,y;
    float vx, vy;
};

struct SoundWave{
    float x,y,r;
};

struct Car car;

struct SoundWave waves[MAX_WAVES];

void draw_car(float dt)
{
    DrawCircle(car.x,car.y,30,WHITE);
    if(car.x > WIDTH || car.x < 0)  car.x = (int)(car.x + WIDTH) % WIDTH;
    if(car.y > HEIGHT || car.y < 0) car.y = (int)(car.y + HEIGHT) % HEIGHT;
    
    car.x += car.vx*dt;
    car.y += car.vy*dt;
}

void emit_new_wave()
{
    waves[curr_waves++ % MAX_WAVES] = (struct SoundWave){car.x,car.y,30};
}

int active_waves()
{
    return curr_waves < MAX_WAVES ? curr_waves : MAX_WAVES;
}

void draw_waves()
{
    for(int i = 0; i < active_waves(); i++)
    {
        DrawCircleLines(waves[i].x,waves[i].y,waves[i].r,GREEN);
    }
}

void propagate_waves(float dt)
{
    for(int i = 0; i < active_waves(); i++)
    {
        waves[i].r += WAVE_SPEED * dt;
    }
}


int main()
{
    InitWindow(WIDTH, HEIGHT, "Doppler Simulator");

    car = (struct Car){WIDTH/2, HEIGHT/2, CAR_SPEED, CAR_SPEED};
    float interval = 0;
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if((interval += dt) > 0.3f)
        {
            emit_new_wave();
            interval = 0.0f;
        }

        if(IsKeyDown(KEY_RIGHT)) car.vx += CAR_ACCELERATION;
        if(IsKeyDown(KEY_LEFT)) car.vx -= CAR_ACCELERATION;
        if(IsKeyDown(KEY_DOWN)) car.vy += CAR_ACCELERATION;
        if(IsKeyDown(KEY_UP)) car.vy -= CAR_ACCELERATION;


        propagate_waves(dt);

        BeginDrawing();

        ClearBackground(BLACK);

        draw_car(dt);
        draw_waves();
        EndDrawing();
    }
}