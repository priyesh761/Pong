#include<iostream>
#include<SDL2/SDL.h>
#define PI 3.14159265358979323846


class Pong{
    private:
        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Color color;
        unsigned int window_width{720};
        unsigned int window_height{720};
        unsigned int ball_speed{16};
        unsigned int speed{9};
        unsigned int ball_size{16};
        unsigned int target_frame_duration{1000};               //Default Frame refresh time  =1 sec

        SDL_Rect l_paddle, r_paddle, ball, score_board;
        float velX, velY;
        std::string score;
        unsigned int l_s, r_s;
        bool turn;
        
    public:
        Pong();
        void run();
        unsigned int getScore();

};

Pong::Pong(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        std::cout << "Failed at SDL_Init()" << std::endl;
    else{
        if(SDL_CreateWindowAndRenderer(this->window_width, this->window_height, 0, &window, &renderer) < 0) 
            std::cout << "Failed at SDL_CreateWindowAndRenderer())" << std::endl;
        else{

            color.r=color.g=color.b=255;
            l_s=r_s=0;

            //Set Pad position and dimensions
            l_paddle.x=32;
            r_paddle.x=this->window_width-r_paddle.w-32;
            l_paddle.h=this->window_height/4;
            r_paddle.h=this->window_height/4;
            l_paddle.y=(this->window_height/2)-(l_paddle.h/2);
            r_paddle.y=(this->window_height/2)-(r_paddle.h/2);
            l_paddle.w=12;
            r_paddle.w=12;
            
            ball.w=ball.h= this->ball_size;
        }
    }
}


void Pong::run(){
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    bool running=true;
    while(running){
        frame_start= SDL_GetTicks();

        frame_end=SDL_GetTicks();
        frame_duration = frame_end - frame_start;

        if(frame_duration<target_frame_duration)
            SDL_Delay(target_frame_duration-frame_duration);
    }
}