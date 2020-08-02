#include "Pong.h"

Pong::Pong(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        std::cout << "Failed at SDL_Init()" << std::endl;
    else{
         
        if(SDL_CreateWindowAndRenderer(this->window_width, this->window_height, 0, &window, &renderer) < 0) 
            std::cout << "Failed at SDL_CreateWindowAndRenderer())" << std::endl;
        else{
            SDL_DisplayMode dm;
            if(SDL_GetCurrentDisplayMode(0,&dm)==0){
                window_width=dm.w-200;
                window_height=dm.h-200;
                std::cout<<dm.h<<' '<<dm.w;
                SDL_SetWindowSize(window, window_width, window_height);
                SDL_SetWindowPosition(window,100,100);
            }   
            
            SDL_SetWindowTitle(window, "Pong");
            keystates = SDL_GetKeyboardState(NULL);
            if(keystates==nullptr)
            exit=true,std::cout<<"Keystate null";
            color.r=color.g=color.b=255;
            l_score=r_score=0;
            
            //Set Pad position and dimensions
            l_paddle.h=this->window_height/4;
            r_paddle.h=this->window_height/4;
            l_paddle.w=12;
            r_paddle.w=12;
            l_paddle.x=32;
            r_paddle.x=this->window_width-r_paddle.w-32;
            l_paddle.y=(this->window_height/2)-(l_paddle.h/2);
            r_paddle.y=(this->window_height/2)-(r_paddle.h/2);
            
            ball.w=ball.h= this->ball_size;
            ball.x=this->window_width/2 - this->ball_size/2;
            ball.y=this->window_height/2 - this->ball_size/2;

            TTF_Init();
            score_font = TTF_OpenFont("Peepo.ttf",font_size);
            if(score_font==nullptr)
                std::cout<<"score_font not found "<<TTF_GetError()<<'\n';
 
            
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
        input();
        update();
        render();
        frame_end=SDL_GetTicks();
        frame_duration = frame_end - frame_start;
    
        if(frame_duration<target_frame_duration)    
            std::this_thread::sleep_for(std::chrono::milliseconds(target_frame_duration-frame_duration));
    }
}

void Pong::serve() {
    ball_speed=15;
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
    SDL_PumpEvents();
    while(SDL_PollEvent(&e)!=0){ 
        //std::cout<<e.type<<'\n';
        if(e.type==SDL_QUIT) this->exit =true;
    }
    if(keystates[SDL_SCANCODE_DOWN]) l_paddle.y+= paddle_speed;
    if(keystates[SDL_SCANCODE_UP]) l_paddle.y-= paddle_speed;
    
}
void Pong::update() {
   
    if(SDL_HasIntersection(&ball, &r_paddle)) {
    //    std::cout<<"right intersect\n";
        ball_speed+=2;
        double rel = (int)(r_paddle.y+(r_paddle.h/2))-(int)(ball.y+(ball_size/2));
        double norm = rel/(r_paddle.h/2);
        double bounce = norm * (5*PI/12);
        velX=-ball_speed*cos(bounce);
        velY= ball_speed*-sin(bounce);
    }
    if(SDL_HasIntersection(&ball, &l_paddle)) {
    //    std::cout<<"left intersect\n";
        double rel = (int)(l_paddle.y+(l_paddle.h/2))-(int)(ball.y+(ball_size/2));
        double norm = rel/(l_paddle.h/2);
        double bounce = norm * (5*PI/12);
        velX=ball_speed*cos(bounce);
        velY=ball_speed*-sin(bounce);
        
    }
    if(ball.y > r_paddle.y+(r_paddle.h/2))
        r_paddle.y+=paddle_speed;
    if(ball.y < r_paddle.y+(r_paddle.h/2)) 
        r_paddle.y-=paddle_speed;
    if(ball.x<=0){ 
        r_score++;
        serve();
    }
    if(ball.x+ball_size>=window_width){ 
        l_score++;
        serve();
    }
    
    if(ball.y<=0 || ball.y+ball_size>=window_height) 
        velY=-velY;
    
    ball.x+=velX;
    ball.y+=velY;
    
    score=std::to_string(l_score) + "   " + std::to_string(r_score);
    
    if(l_paddle.y<0)
        l_paddle.y=0;
    if(r_paddle.y<0)
        r_paddle.y=0;
    if(l_paddle.y+l_paddle.h>window_height)
        l_paddle.y=window_height-l_paddle.h;
    if(r_paddle.y+r_paddle.h>window_height)
        r_paddle.y=window_height-r_paddle.h;

}



void Pong::render() {
 SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
 SDL_RenderClear(renderer);


 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
 SDL_RenderFillRect(renderer, &l_paddle);
 SDL_RenderFillRect(renderer, &r_paddle);
 SDL_RenderFillRect(renderer, &ball);
 writeScore(score, window_width/2+font_size, font_size*2);

 SDL_RenderPresent(renderer);
}

void Pong::writeScore(std::string text, int x, int y) {

 SDL_Surface *surface;
 SDL_Texture *texture;
 const char* t = text.c_str();
 surface = TTF_RenderText_Solid(score_font, t, color);
 texture = SDL_CreateTextureFromSurface(renderer, surface);
 if(surface==nullptr)
    std::cout<<"surface not found\n";
 if(score_font==nullptr)
    std::cout<<"score_font not found"<<'\n';
 score_board.w=surface->w;
 score_board.h=surface->h;
 score_board.x=x-score_board.w;
 score_board.y=y-score_board.h;
 SDL_FreeSurface(surface);
 SDL_RenderCopy(renderer, texture, NULL, &score_board);
 SDL_DestroyTexture(texture);

 //std::cout<<score<<'\n';
}


