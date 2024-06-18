/*
 *	The inspiration for the aesthetics came from the classic youtube video The Duck Song (https://www.youtube.com/watch?v=MtN1YnoL46Q).
 * 	Code needs to be compiled with the SDL_gfx, SDL_image, and SDL_mixer libraries to work.
 *
 *	Written for CS490 proj4 Sprite sheet animations
 *	Code displays a game that has a player trying to keep a ball in the air for as y hits as possible before it resets the score.
 *	By Betsy Kamas
 *	Some code used from class
 * 	October 20 2023
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL2_framerate.h>
#include <SDL_mixer.h>


using namespace std;


bool checkCollision(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w >= b.x && a.x <= b.x + b.w && a.y + a.h >= b.y && a.y <= b.y + b.h) {
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        cerr << "SDL_Init() fail... " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_Music* music = Mix_LoadMUS("night-ambience-17064.mp3");
    if (music == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1);
	
	SDL_Init(SDL_INIT_VIDEO);
   
	FPSmanager fps;
    SDL_initFramerate(&fps);

	SDL_Surface *image = IMG_Load("trees.jpg");
	int WW = image->w;
	int WH = image->h;	

	SDL_Window* window = SDL_CreateWindow("proj4 kamase: Lemon Ducky ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WW, WH, SDL_WINDOW_SHOWN );
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	image = nullptr;
	
	SDL_Surface *player = IMG_Load("ducky.png");
    int PH = (player->h) / 4;
    SDL_Texture *ducky = SDL_CreateTextureFromSurface(renderer, player);
    SDL_FreeSurface(player);  // no need now that we have a texture
	player = nullptr;
	
	SDL_Surface *object = IMG_Load("lemon.png");
	int LW = object->w / 6;
	int LH = object->h / 6;
	SDL_Texture *lemon = SDL_CreateTextureFromSurface(renderer, object);
    SDL_FreeSurface(object);  // no need now that we have a texture
	object = nullptr;

	float bx = (WW - LW) / 2;
    float by = 0;
    float vy = 0;
    float vx = 0;

	int duckZoom = 5*PH;
	
    SDL_Rect box;
    box.x = WW / 2;
    box.y = WH - 1 - duckZoom;
    box.w = duckZoom;
    box.h = duckZoom;
	
	int SpriteW = 32;
	int SpriteH = 32;
	int SpriteY = 0;	
	
	int FCNT = 6;
	int frame = 0;
	int score = 0;
	int hscore = 0;
	
	float prevX = 0;
	
	float midduck, temp;
		
	bool click = false;
	bool collide = false;

	SDL_Event event;
    bool quit = false;
    while (!quit) {
        SDL_RenderClear(renderer);
		while (SDL_PollEvent(&event)) {
            switch (event.type) {
				case SDL_QUIT:     quit = true;  break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:  quit = true;  break;
						case SDLK_SPACE:  vy -= 12;  break;
					}break;
				case SDL_MOUSEMOTION:
					midduck = event.motion.x - box.w / 2;  break;
				case SDL_MOUSEBUTTONDOWN:
					switch (event.button.button){
						case SDL_BUTTON_LEFT: click = true; break;
					} break;
			default: break;
			}
        }
		
		// velocities & x,y lemon ball
		vy++;
        by += vy;
		vx *= 0.98;
        bx += vx;
		
		// keep lemon  from going off the bottom
		if(by + LH > WH - 1) {
            vy = -(vy * 0.7);
            by = WH - 1 - LH;
			if(fabs(vy) < 2) {
                vx *= 0.9;
            }
		}
		
		// keep lemon from going off the right
		if(bx + LW > WW - 1) {
            vx = -(vx * 0.6);
            bx = WW - 1 - LW;
        }
		
		// keep lemon from going off left
		if(bx <= 0) {
            vx = -(vx * 0.6);
            bx = 0;
        }
		
		// keeping player from going off screen
		if( midduck < -15 )
			box.x = -15; 	// left side
		else if(midduck  + box.w/2  >= WW - box.w/2)
			box.x = WW - box.w + 20; 	// right side
		else
			box.x = midduck;	// within screen
		
		// for easing
		box.x += (prevX - box.x) * 0.9;
		
		// choosing which sprite based on movement
		if( midduck < box.x)
			SpriteY = SpriteW * 1;	// move left
		else if( midduck - (box.w / 5) > box.x )
			SpriteY = SpriteW * 2; 	// move right 	
		else
			SpriteY = SpriteW * 0;
		
		SDL_framerateDelay(&fps);

		// cycling through the sprite sheet for animation effect
		SDL_RenderCopy(renderer, t, nullptr, nullptr); // display background image
		SDL_Rect duck = { ((frame++)/3 % FCNT) * SpriteW, SpriteY, SpriteW, SpriteH};
		SDL_RenderCopy(renderer, ducky, &duck, &box ); // display duck
		
		SDL_Rect lem = {(int)round(bx), (int)round(by), LW, LH};
		
		// check from click event if ball and player collide
		if(click){
			collide = checkCollision(box, lem);
			if(collide){					// collided
				score++;					
				vx = (bx - box.x)*.62; 		// scale velocities in x&y
				vy = (by - box.y)*.62; 
				hscore = max(score, hscore); // set highscore
			}
			else{	// missed ball
				score = 0;
				collide = false;
			}
			click = false; //reset
		}
		 // if ball hits bottom reset
		if( lem.y == WH - lem.h - 1 )
			score = 0;
			
		//display lemon
		SDL_RenderCopyEx(renderer, lemon, nullptr, &lem, bx*1.5, nullptr, SDL_FLIP_NONE); 

		string s = "Score: " + to_string(score);
		string hs = "High Score: " +to_string(hscore);
		
		stringColor(renderer, 5, 5, "Click to keep the lemon in the air.", 0xffffffff);
		stringColor(renderer, 5, 20,"Press SPACE to lob lemon",  0xffffffff);
		stringColor(renderer, 5, 35, s.c_str(),  0xffffffff);
		stringColor(renderer, 5, 50, hs.c_str(),  0xffffffff);
		SDL_RenderPresent(renderer);

		prevX = box.x;
    }
	
	SDL_DestroyTexture(t);
	t = nullptr;
	
    SDL_DestroyTexture(ducky);
	ducky = nullptr;
	
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	
    SDL_DestroyWindow(window);
	window = nullptr;
	
	Mix_FreeMusic(music);
    Mix_CloseAudio();
	
	IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}