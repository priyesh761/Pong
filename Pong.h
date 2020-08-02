

#include<iostream>
#include<chrono>
#include<thread>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#define PI 3.14159265358979323846


class Pong{
    private:
        SDL_Renderer* renderer{nullptr};
        SDL_Window* window{nullptr};
        SDL_Color color;
        int window_width{1080};
        int window_height{720};
        int ball_speed{15};
        int paddle_speed{10};
        int ball_size{16};
        int target_frame_duration{1000/60};               //Default Frame refresh rate  
        const Uint8 *keystates;
        bool exit{0};

        SDL_Rect l_paddle, r_paddle, ball, score_board;
        float velX, velY;
        std::string score;
        TTF_Font* score_font{nullptr};
        int font_size{30};
        int l_score, r_score;
        bool turn{0};
        
    public:
        Pong();
        ~Pong();
        void run();
        void serve();
        void input();
        void update();
        void render();
        void writeScore(std::string , int, int) ;

};
