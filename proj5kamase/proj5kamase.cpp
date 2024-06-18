/*
 *	Written for CS490 proj5 racing threads
 *	Code displays a race where each racer has a randomly assigned easing function that determines how fast it goes.
 *  Each racer competes in its own thread.
 *	By Betsy Kamas
 *	Some code used from class
 * 	November 17 2023
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <ctime>
#include <vector>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL2_framerate.h>
#include <SDL_mixer.h>

#include "easing.h"


using namespace std;

bool winner = false;
char whoWon = 'n';

void racePercent(int function, int WindowWidth, float currentWidth, float& racerX, int racerW, char competer){
	if(!winner){
		switch(function){
			case 0: 
				racerX = (WindowWidth * easeInOutQuart(currentWidth / WindowWidth));
				break;
			case 1: 
				racerX = (WindowWidth * easeInOutExpo(currentWidth / WindowWidth));
				break;
			case 2: 
				racerX = (WindowWidth * easeInCirc(currentWidth / WindowWidth));
				break;
			case 3: 
				racerX = ( WindowWidth * easeInQuart(currentWidth / WindowWidth));
				break;
			default: break;
		}
		if(racerX + racerW >= WindowWidth - 70){
			winner = true;
			whoWon = competer;
		}
	}
	
}

int main(int argc, char* argv[]) {
	srand(time(nullptr));
	const int numRacers = 4;
	const int racerW = 80;
	
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        cerr << "SDL_Init() fail... " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }
	
	SDL_Init(SDL_INIT_VIDEO);
 
	FPSmanager fps;
    SDL_initFramerate(&fps);

	Mix_Music *backgroundMusic;
	Mix_Music *victoryMusic;
    Mix_Chunk *go;
    Mix_Chunk *win;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    backgroundMusic = Mix_LoadMUS("background.wav");
    go = Mix_LoadWAV("go.wav");
    win = Mix_LoadWAV("win.wav");
	victoryMusic = Mix_LoadMUS("victoryMusic.wav");

	SDL_Surface *image = IMG_Load("race-track.jpg");
	int WW = image->w * 1.75;
	int WH = image->h * 1.75;	

	SDL_Window* window = SDL_CreateWindow("proj5 kamase: race ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WW, WH, SDL_WINDOW_SHOWN );
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	SDL_Surface *player1 = IMG_Load("cobra1.png");
    int CPW = (player1->w) / 8;
    int CPH = (player1->h) / 2;
	SDL_Texture *cobra = SDL_CreateTextureFromSurface(renderer, player1);
    SDL_FreeSurface(player1);  // no need now that we have a texture
	player1 = nullptr;	image = nullptr;
	
	SDL_Surface *player2 = IMG_Load("fox1.png");
    int FPW = (player2->w) / 8;
    int FPH = (player2->h) / 2;
	SDL_Texture *fox = SDL_CreateTextureFromSurface(renderer, player2);
    SDL_FreeSurface(player2);  // no need now that we have a texture
	player2 = nullptr;
	
	SDL_Surface *player3 = IMG_Load("hedgehog1.png");
    int HPW = (player3->w) / 4;
    int HPH = (player3->h) / 2;
	SDL_Texture *hedgehog = SDL_CreateTextureFromSurface(renderer, player3);
    SDL_FreeSurface(player3);  // no need now that we have a texture
	player3 = nullptr;
		
	SDL_Surface *player4 = IMG_Load("squirrel1.png");
    int SPW = (player4->w) / 8;
    int SPH = (player4->h) / 2;
	SDL_Texture *squirrel = SDL_CreateTextureFromSurface(renderer, player4);
    SDL_FreeSurface(player4);  // no need now that we have a texture
	player4 = nullptr;

	SDL_Surface *welcome = IMG_Load("welcome.png");
    int welcW = (welcome->w);
    int welcH = (welcome->h);
	SDL_Texture *startMsg = SDL_CreateTextureFromSurface(renderer, welcome);
    SDL_FreeSurface(welcome);  // no need now that we have a texture
	welcome = nullptr;
	
	SDL_Rect msg = {(WW/2 - welcW/2), 25, WW/2.5, (WH - 50)};

	int CFCNT = 8;
	int FFCNT = 8;
	int HFCNT = 4;
	int SFCNT = 8;
	
	int frame = 0;
	float width = 0.0;

	float foxX = 0.0;
	float hedgehogX = 0.0;
	float cobraX = 0.0;
	float squirrelX = 0.0;
	
	vector <thread> racers;
	
	int order[numRacers] = {0};
	for(int i = 0; i < numRacers; i++){	// init array
		order[i] = i;
	}

	for(int i = 0; i < numRacers; i++){		// this for loop randomizes which racer gets what function
		int j = rand() % numRacers;
		swap(order[i], order[j]);
	}		
	
	SDL_Rect hbox;
	SDL_Rect H;
	SDL_Rect sbox;
	SDL_Rect S;
	SDL_Rect fbox;
	SDL_Rect F;
	SDL_Rect cbox;
	SDL_Rect C;

	SDL_Event event;
    bool quit = false;
	bool start = false;
	
	Mix_PlayMusic(backgroundMusic, -1);
	
	while (!quit && !start) { /* for the actual race */
        SDL_RenderClear(renderer);
		while (SDL_PollEvent(&event)) {
            switch (event.type) {
				case SDL_QUIT:     quit = true;  break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:  quit = true;  break;
						case SDLK_RETURN: start = true; break;
						}break;
				default: break;
			}
        }
		SDL_framerateDelay(&fps);

		SDL_RenderCopy(renderer, t, nullptr, nullptr); // display background image
		
		hbox = { (int)hedgehogX, (WH - 1) / 2, racerW, racerW};
		H = { ((frame++)/30 % HFCNT) * HPW, 0, HPW, HPH};
		SDL_RenderCopy(renderer, hedgehog, &H, &hbox ); // display hedgehog
	
		sbox = { (int)squirrelX, hbox.y + hbox.h + 10, racerW, racerW};
		S = { ((frame++)/10 % SFCNT) * SPW, 0, SPW, SPH};
		SDL_RenderCopy(renderer, squirrel, &S, &sbox ); // display squirrel
	
		fbox = { (int)foxX, hbox.y - racerW, racerW, racerW};
		F = { ((frame++)/20 % FFCNT) * FPW, 0, FPW, FPH};
		SDL_RenderCopy(renderer, fox, &F, &fbox ); // display fox
		
		cbox = { (int)cobraX, fbox.y - racerW - 10, racerW, racerW};
		C = { ((frame++)/20 % CFCNT) * CPW, 0, CPW, CPH};
		SDL_RenderCopy(renderer, cobra, &C, &cbox ); // display cobra
		
		SDL_RenderCopy(renderer, startMsg, nullptr, &msg);
		
		SDL_RenderPresent(renderer);
		
	}
	if(!quit){
		SDL_RenderCopy(renderer, t, nullptr, nullptr); // display background image
		SDL_RenderCopy(renderer, hedgehog, &H, &hbox ); // display hedgehog
		SDL_RenderCopy(renderer, squirrel, &S, &sbox ); // display squirrel
		SDL_RenderCopy(renderer, fox, &F, &fbox ); // display fox
		SDL_RenderCopy(renderer, cobra, &C, &cbox ); // display cobra
		SDL_RenderPresent(renderer);
		Mix_PlayChannel(1, go, 0);
		SDL_Delay(3000);
	}
	
    while (!quit && !winner) { /* for the actual race */
        SDL_RenderClear(renderer);
		while (SDL_PollEvent(&event)) {
            switch (event.type) {
				case SDL_QUIT:     quit = true;  break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:  quit = true;  break;
						}break;
				default: break;
			}
        }
		
		SDL_framerateDelay(&fps);

		// cycling through the sprite sheet for animation effect
		SDL_RenderCopy(renderer, t, nullptr, nullptr); // display background image

		hbox = { (int)hedgehogX, (WH - 1) / 2, racerW, racerW};
		H = { ((frame++)/30 % HFCNT) * HPW, HPH, HPW, HPH};
		SDL_RenderCopy(renderer, hedgehog, &H, &hbox ); // display hedgehog
	
		sbox = { (int)squirrelX, hbox.y + hbox.h + 10, racerW, racerW};
		S = { ((frame++)/10 % SFCNT) * SPW, SPH, SPW, SPH};
		SDL_RenderCopy(renderer, squirrel, &S, &sbox ); // display squirrel
	
		fbox = { (int)foxX, hbox.y - racerW, racerW, racerW};
		F = { ((frame++)/20 % FFCNT) * FPW, FPH, FPW, FPH};
		SDL_RenderCopy(renderer, fox, &F, &fbox ); // display fox
		
		cbox = { (int)cobraX, fbox.y - racerW - 10, racerW, racerW};
		C = { ((frame++)/20 % CFCNT) * CPW, CPH, CPW, CPH};
		SDL_RenderCopy(renderer, cobra, &C, &cbox ); // display cobra
		
		width += WW / 500.0;

		SDL_RenderPresent(renderer);

		thread threadPlayerOne(racePercent, order[0], WW, width, ref(foxX), fbox.w, 'f');
		//threadPlayerOne.join();
		racers.push_back(move(threadPlayerOne));

		thread threadPlayerTwo(racePercent, order[1], WW, width, ref(hedgehogX), hbox.w, 'h');
		//threadPlayerTwo.join();
		racers.push_back(move(threadPlayerTwo));

		thread threadPlayerThree(racePercent, order[2], WW, width, ref(cobraX), cbox.w, 'c');
		//threadPlayerThree.join();
		racers.push_back(move(threadPlayerThree));
		
		thread threadPlayerFour(racePercent, order[3], WW, width, ref(squirrelX), sbox.w, 's');
		//threadPlayerFour.join();
		racers.push_back(move(threadPlayerFour));
		
		
    }
	
	for(auto &r: racers){
		r.join();
	}

	int bkgdX = WW-1; 
	int wx = WW - 70 - racerW; // x poistion of winning 
	int wy = 0;
	int yvals[4] = {cbox.y, fbox.y, hbox.y, sbox.y};

	SDL_Rect winnerBkgd;
		
	bool halfway = false;
	bool fullway = false;
	float sizeInc = racerW;
	
	Mix_HaltMusic();
	Mix_PlayChannel(1, win, 0);

	Mix_PlayMusic(victoryMusic, -1);	

	while(!quit) {	/* for moving sprite to center of screen */
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:  quit = true;  break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:  quit = true;  break;
                default:  break;
                }
            default:  break;
            }
        }
		
		SDL_framerateDelay(&fps);
		winnerBkgd = {bkgdX, 0, WW-bkgdX, WH};
		int winnerX = (WW / 2) - (sizeInc / 2);
		int winnerY = (WH / 2) - (sizeInc / 2);
		
		switch(whoWon){
			case 'f':
				SDL_SetRenderDrawColor(renderer, 0xCD, 0x7A, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				if (bkgdX <= WW - 110){
					F = { ((frame++)/10 % FFCNT) * FPW, 0, FPW, FPH};
				}
				fbox.x = wx;
				fbox.y = yvals[1];
				fbox.w = sizeInc;
				fbox.h = sizeInc;
				SDL_RenderCopy(renderer, fox, &F, &fbox );
				break;
			case 'h':
				SDL_SetRenderDrawColor(renderer, 0xA9, 0xA9, 0xA9, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				if (bkgdX <= WW - 110){
					H = { ((frame++)/15 % HFCNT) * HPW, 0, HPW, HPH};
				}
				hbox.x = wx;
				hbox.y = yvals[2];
				hbox.w = sizeInc;
				hbox.h = sizeInc;
				SDL_RenderCopy(renderer, hedgehog, &H, &hbox );
				break;
			case 'c':
				SDL_SetRenderDrawColor(renderer, 0xC3, 0x44, 0xE5, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				if (bkgdX <= WW - 110){
					C = { ((frame++)/10 % CFCNT) * CPW, 0, CPW, CPH};
				}
				cbox.x = wx;
				cbox.y = yvals[0];
				cbox.w = sizeInc;
				cbox.h = sizeInc;
				SDL_RenderCopy(renderer, cobra, &C, &cbox );
				break;
			case 's':
				SDL_SetRenderDrawColor(renderer, 0x80, 0x40, 0x0B, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				if (bkgdX <= WW - 110){
					S = { ((frame++)/5 % SFCNT) * SPW, 0, SPW, SPH};
				}
				sbox.x = wx;
				sbox.y = yvals[3];
				sbox.w = sizeInc;
				sbox.h = sizeInc;
				SDL_RenderCopy(renderer, squirrel, &S, &sbox );
				break;
			default: break;
		}
		
		if(bkgdX >= 0){
			bkgdX-=WW/100;
		}else 
			fullway = true;

		if(bkgdX <= wx && wx >= winnerX){
			wx -= WW / 200;
			for(int i = 0; i < 4; i++){
				if(yvals[i] > winnerY)
					yvals[i] -= WH / 200;
				if(yvals[i] < winnerY)
					yvals[i] += WH / 200;
			}
			sizeInc += 2;
		}else
			halfway = true;
		
		if(fullway && halfway)
			break;
			

		SDL_RenderPresent(renderer);

    }
	
	SDL_Surface *text = IMG_Load("winner.png");
	int mw = text->w;
	int mh = text->h;
	SDL_Texture *winner = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);  // no need now that we have a texture
	text = nullptr;
	
	SDL_Rect message = {(WW/2) - (mw*3 / 2), 10, mw * 3, mh * 3};
	
	while(!quit) {	/* While loop for winner screen with idle*/
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:  quit = true;  break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:  quit = true;  break;
                default:  break;
                }
            default:  break;
            }
        }
		
		SDL_framerateDelay(&fps);
		
		switch(whoWon){
			case 'f':
				SDL_SetRenderDrawColor(renderer, 0xCD, 0x7A, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				SDL_RenderCopy(renderer, winner, nullptr, &message);
				F = { ((frame++)/10 % FFCNT) * FPW, 0, FPW, FPH};
				SDL_RenderCopy(renderer, fox, &F, &fbox );
				break;
			case 'h':
				SDL_SetRenderDrawColor(renderer, 0xA9, 0xA9, 0xA9, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				H = { ((frame++)/15 % HFCNT) * HPW, 0, HPW, HPH};
				SDL_RenderCopy(renderer, winner, nullptr,& message);
				SDL_RenderCopy(renderer, hedgehog, &H, &hbox );
				break;
			case 'c':
				SDL_SetRenderDrawColor(renderer, 0xC3, 0x44, 0xE5, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				C = { ((frame++)/10 % CFCNT) * CPW, 0, CPW, CPH};
				SDL_RenderCopy(renderer, winner, nullptr, &message);
				SDL_RenderCopy(renderer, cobra, &C, &cbox );
				break;
			case 's':
				SDL_SetRenderDrawColor(renderer, 0x80, 0x40, 0x0B, 0xFF);
				SDL_RenderFillRect(renderer, &winnerBkgd);
				S = { ((frame++)/5 % SFCNT) * SPW, 0, SPW, SPH};
				SDL_RenderCopy(renderer, winner, nullptr, &message);
				SDL_RenderCopy(renderer, squirrel, &S, &sbox );
				break;
			default: break;
		}
		
		SDL_RenderPresent(renderer);
	}
	

	SDL_DestroyTexture(t);
	t = nullptr;
	
	SDL_DestroyTexture(startMsg);
	startMsg = nullptr;
	
	SDL_DestroyTexture(winner);
	winner = nullptr;
	
    SDL_DestroyTexture(cobra);
	cobra = nullptr;
	
	SDL_DestroyTexture(fox);
	fox = nullptr;

	SDL_DestroyTexture(hedgehog);
	hedgehog = nullptr;
	
	SDL_DestroyTexture(squirrel);
	squirrel = nullptr;
	
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	
    SDL_DestroyWindow(window);
	window = nullptr;
	
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeMusic(victoryMusic);
    Mix_FreeChunk(go);
    Mix_FreeChunk(win);
    
	
	IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}