#include<iostream>
#include<chrono>
#include<thread>
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
        unsigned int paddle_speed{9};
        unsigned int ball_size{16};
        unsigned int target_frame_duration{1000};               //Default Frame refresh time  =1 sec
        bool exit{0};

        SDL_Rect l_paddle, r_paddle, ball, score_board;
        float velX, velY;
        std::string score;
        unsigned int l_s, r_s;
        bool turn{0};
        
    public:
        Pong();
        ~Pong();
        void run();
        void serve();
        void input();
        void update();
        void render();
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

Pong::~Pong(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Pong::run(){
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    
    serve();
    while(this->exit==false){
        frame_start= SDL_GetTicks();
        update();
        input();
        render();
        frame_end=SDL_GetTicks();
        frame_duration = frame_end - frame_start;
    
        if(frame_duration<target_frame_duration)
        
            std::this_thread::sleep_for(std::chrono::milliseconds(target_frame_duration-frame_duration));
    }
}

void Pong::serve() {
    l_paddle.y=r_paddle.y=(window_height/2)-(l_paddle.h/2);
    if(turn) {
        ball.x=l_paddle.x+(l_paddle.w*4);
        velX=ball_speed/2;
    } else {
        ball.x=r_paddle.x-(r_paddle.w*4);
        velX=-ball_speed/2;
    }
    velY=0;
    ball.y=window_height/2-(ball_size/2);
    turn=!turn;
}

void Pong::input() {
    SDL_Event e;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&e)){ 
        if(e.type==SDL_QUIT) this->exit =true;
        if(e.type==SDL_KEYDOWN) l_paddle.y+= paddle_speed;
        if(e.type==SDL_KEYUP) l_paddle.y-= paddle_speed;
    }
}
void Pong::update() {
 if(SDL_HasIntersection(&ball, &r_paddle)) {
  double rel = (r_paddle.y+(r_paddle.h/2))-(ball.y+(ball_size/2));
  double norm = rel/(r_paddle.h/2);
  double bounce = norm * (5*PI/12);
  velX=-ball_speed*cos(bounce);
  velY=ball_speed*-sin(bounce);
 }
 if(SDL_HasIntersection(&ball, &l_paddle)) {
  double rel = (l_paddle.y+(l_paddle.h/2))-(ball.y+(ball_size/2));
  double norm = rel/(l_paddle.h/2);
  double bounce = norm * (5*PI/12);
  velX=ball_speed*cos(bounce);
  velY=ball_speed*-sin(bounce);
 }
 if(ball.y > r_paddle.y+(r_paddle.h/2)) r_paddle.y+=paddle_speed;
 if(ball.y < r_paddle.y+(r_paddle.h/2)) r_paddle.y-=paddle_speed;
 if(ball.x<=0) {r_s++;serve();}
 if(ball.x+ball_size>=window_width) {l_s++;serve();}
 if(ball.y<=0 || ball.y+ball_size>=window_height) velY=-velY;
 ball.x+=velX;
 ball.y+=velY;
 //score=std::to_string(l_s) + "   " + std::to_string(r_s);
 if(l_paddle.y<0)l_paddle.y=0;
 if(l_paddle.y+l_paddle.h>window_height)l_paddle.y=window_height-l_paddle.h;
 if(r_paddle.y<0)r_paddle.y=0;
 if(r_paddle.y+r_paddle.h>window_height)r_paddle.y=window_height-r_paddle.h;
}



void Pong::render() {
 SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
 SDL_RenderClear(renderer);
//timerFPS = SDL_GetTicks()-lastFrame;
 //if(timerFPS<(1000/60)) {
  //SDL_Delay((1000/60)-timerFPS);
 //}
 

 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
 SDL_RenderFillRect(renderer, &l_paddle);
 SDL_RenderFillRect(renderer, &r_paddle);
 SDL_RenderFillRect(renderer, &ball);
 //write(score, WIDTH/2+FONT_SIZE, FONT_SIZE*2);

 SDL_RenderPresent(renderer);
}


int main(){
    Pong p;

    p.run();
}