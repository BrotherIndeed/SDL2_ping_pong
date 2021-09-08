#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#define WIDTH 1000
#define HEIGHT 720
#define FONT_SIZE 32
int  BALL_SPEED = 16;
int SPEED = 9;
#define SIZE 16
#define PI 3.14159265358979323846
void input();
int store;

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
bool running;
int frameCount, timerFPS, lastFrame, fps;
float multiplyer =1;
SDL_Rect l_paddle, r_paddle, ball, score_board;
float velX, velY;
std::string score;
std::string gamepoint;
int l_s, r_s;
float level=0;
bool turn;

void serve() {
    velX=velX*(level/5);
    velY=velY*(level/5);
    SPEED = 9;
 l_paddle.y=r_paddle.y=(HEIGHT/2)-(l_paddle.h/2);
 if(turn) {
  ball.x=l_paddle.x+(l_paddle.w*4);
  velX=BALL_SPEED/2;
 } else {
  ball.x=r_paddle.x-(r_paddle.w*4);
  velX=-BALL_SPEED/2;
 }
 velY=0;
 ball.y=HEIGHT/2-(SIZE/2);
 turn=!turn;
 
}

void write(std::string text, int x, int y) {
 SDL_Surface *surface;
 SDL_Texture *texture;
 const char* t = text.c_str();
 surface = TTF_RenderText_Solid(font, t, color);
 texture = SDL_CreateTextureFromSurface(renderer, surface);
 score_board.w=surface->w;
 score_board.h=surface->h;
 score_board.x=x-score_board.w;
 score_board.y=y-score_board.h;
 SDL_FreeSurface(surface);
 SDL_RenderCopy(renderer, texture, NULL, &score_board);
 SDL_DestroyTexture(texture);
}
void end()
{
    gamepoint="You Win, Press Enter to exit";
     
     int frameStart = SDL_GetTicks();
     int frameTime=SDL_GetTicks() - frameStart;
    
    SDL_RenderPresent(renderer);
     while(running) 
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
        frameStart = SDL_GetTicks();
        SDL_RenderClear(renderer);
        write(gamepoint, WIDTH/2+FONT_SIZE*2, FONT_SIZE*2);
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;

if (1000 > frameTime) {
    SDL_Delay(1000/60 - frameTime);
}
input();
 }
}

void update() {
 if(SDL_HasIntersection(&ball, &r_paddle)) {
  double rel = (r_paddle.y+(r_paddle.h/2))-(ball.y+(SIZE/2));
  double norm = rel/(r_paddle.h/2);
  double bounce = norm * (5*PI/12);
  velX=-BALL_SPEED*cos(bounce);
  velY=BALL_SPEED*-sin(bounce);
 }
 if(SDL_HasIntersection(&ball, &l_paddle)) {
  double rel = (l_paddle.y+(l_paddle.h/2))-(ball.y+(SIZE/2));
  double norm = rel/(l_paddle.h/2);
  double bounce = norm * (5*PI/12);
  velX=BALL_SPEED*cos(bounce);
  velY=BALL_SPEED*-sin(bounce);
 }
 if(ball.y > r_paddle.y+(r_paddle.h/2)) r_paddle.y+=SPEED;
 if(ball.y < r_paddle.y+(r_paddle.h/2)) r_paddle.y-=SPEED;
 if(ball.x<=0) {r_s++;serve();}
 if(ball.x+SIZE>=WIDTH) {l_s++;serve();}
 if(ball.y<=0 || ball.y+SIZE>=HEIGHT) velY=-velY;
 ball.x+=velX*(multiplyer);
 ball.y+=velY*(multiplyer);
 level = l_s-r_s+1;
 if(level<=0)
 {
     level =1;
     l_s=r_s=0;
 }
  if(level>=10)
 {
     level =10;
     
 }
 multiplyer = 1 + level/15;
 store = level;
 score="Level " + std::to_string(store-1);
 if(l_paddle.y<0)l_paddle.y=0;
 if(l_paddle.y+l_paddle.h>HEIGHT)l_paddle.y=HEIGHT-l_paddle.h;
 if(r_paddle.y<0)r_paddle.y=0;
 if(r_paddle.y+r_paddle.h>HEIGHT)r_paddle.y=HEIGHT-r_paddle.h;
}

void input() {

 SDL_Event e;
 int y,x;
 Uint32 buttons;
 buttons = SDL_GetMouseState(&x, &y);
 const Uint8 *keystates = SDL_GetKeyboardState(NULL);
 while(SDL_PollEvent(&e)) if(e.type==SDL_QUIT) running=false;
 if(keystates[SDL_SCANCODE_ESCAPE]) running=false;
 if(keystates[SDL_SCANCODE_RETURN]) running=false;
 l_paddle.y=y-(l_paddle.h/2);
 if(keystates[SDL_SCANCODE_UP]) {l_s++;SDL_Delay(100);};
 
}

void render() {
 SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
 SDL_RenderClear(renderer);

 frameCount++;
 timerFPS = SDL_GetTicks()-lastFrame;
 if(timerFPS<(1000/60)) {
  SDL_Delay((1000/60)-timerFPS);
 }

 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
 SDL_RenderFillRect(renderer, &l_paddle);
 SDL_RenderFillRect(renderer, &r_paddle);
 SDL_RenderFillRect(renderer, &ball);
 write(score, WIDTH/2+FONT_SIZE, FONT_SIZE*2);
 if(level>=10)
 {
     gamepoint="game point " + std::to_string(l_s-r_s-9);
     write(gamepoint, WIDTH+FONT_SIZE-32, FONT_SIZE*2);
 }
 if(l_s-r_s>=12)
 {
     end();
 }

 SDL_RenderPresent(renderer);
}
void changecolor()
{int x= level;
    color.r=(x*100)%255;
    color.g=(x*50)%255;
    color.b=(x*75)%255;
}
int main(int argc, char *argv[]) {

    


 if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
 if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) std::cout << "Failed at SDL_CreateWindowAndRenderer())" << std::endl;
 TTF_Init();
 font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);
 running=1;
 static int lastTime = 0;

 SDL_ShowCursor(SDL_DISABLE);
 SDL_SetRelativeMouseMode(SDL_TRUE);

 color.r=color.g=color.b=255;
 l_s=r_s=0;
 l_paddle.x=32; l_paddle.h=HEIGHT/4;
 l_paddle.y=(HEIGHT/2)-(l_paddle.h/2);
 l_paddle.w=12;
 r_paddle=l_paddle;
 r_paddle.x=WIDTH-r_paddle.w-32;
 ball.w=ball.h=SIZE;

 serve();

 while(running) {



  lastFrame=SDL_GetTicks();
  if(lastFrame>=(lastTime+1000)) {
   lastTime=lastFrame;
   fps=frameCount;
   frameCount=0;
  }

  changecolor();
  update();
  input();
  render();
 }
 TTF_CloseFont(font);
 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
 SDL_Quit();
}
