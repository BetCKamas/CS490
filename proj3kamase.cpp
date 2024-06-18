/*
	This progam takes in a bmp image as a command line arguement and loads the image in a window that is initially the exact
	size of the image. The window can be resized by the user dragging the edges of the window and the image size adjusts to 
	the size of the window based on	how the size of it is changed. 
	
	The point of this program is to allow the user to change the RGB values of the textures's color mode using different keys.
	
	"q" decreases & "e" increases the red value;
	"a" decreases & "d" increases the green value;
	"z" decreases & "c" increases the blue value.
	
	Based on code done in class and on previous assignments plus a couple new things. Only this file needs to be complied
	with linker libraries to mingw32, SDL2_gfx, SDL2main, & SDL2. The exe needs to be rumnwith a bmp file to load properly.
	Betsy Kamas 
	October 6 2023

*/

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

using namespace std;

int main(int argc, char* args[]) {
	SDL_Window *w = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Texture *t = nullptr;
	SDL_Surface *image = nullptr;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		cerr << "SDL_Init() fail... " << SDL_GetError() << endl;
		exit(EXIT_FAILURE);
	}

	const char* imageName = args[1];	// grabbing file name from cmd line arg
	image = SDL_LoadBMP(imageName);		// setting image surface as picture
	
	int width = image->w;
	int height = image->h;
	
	w = SDL_CreateWindow("proj3: kamase resize and color mode change", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, (height+15), (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
	
	renderer = SDL_CreateRenderer(w, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	t = SDL_CreateTextureFromSurface(renderer, image);
	
	int top, left, bottom, right = 0;

	SDL_Event e;
		
	Uint8 red, green, blue = 0;
	
	SDL_Rect r; // rectangle for the image to display
	r.x = 0;
	r.y = 0;
	r.w = width;
	r.h = height;
	
	SDL_Rect background; // rectangle for the background of the string
	background.x = 0;
	background.y = height;
	background.w = width;
	background.h = 15;

	bool quit = false;
	while(!quit){
		while(SDL_PollEvent(&e)){
			SDL_GetWindowSize(w, &width, &height);
			SDL_GetTextureColorMod(t, &red, &green, &blue); // sets color mode values
			string s = "red: "+ to_string(red)+" green: " + to_string(green)+" blue: " + to_string(blue); // setting string to be displayed

			SDL_RenderClear(renderer); // fill the scene with white
			SDL_RenderCopy(renderer, t, nullptr, &r); // display image
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // set black
			SDL_RenderFillRect(renderer, &background); // add in bottom
			
			switch(e.type){
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
					case SDLK_ESCAPE{quit = true; break;}
					case SDLK_q {if(red > 0){ red--;}} break;
					case SDLK_e {if(red < 255){ red++;}} break;
					case SDLK_a {if(green > 0){ green--;}} break;
					case SDLK_d {if(green < 255){ green++;}} break;
					case SDLK_z {if(blue > 0){ blue--;}} break;
					case SDLK_c {if(blue < 255){ blue++;}} break;
					default: break;	 }
					break;	
				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
							SDL_GetWindowSize(w, &width, &height);
							r.w = width;
							r.h = height-15;
							background.y = height;
					}
				break;
				default:
					break;				
			}
			
			SDL_SetTextureColorMod(t, red, green, blue); // changes color mode value of texture based on above keystrokes
			stringRGBA(renderer, 10, (height - 10), s.c_str(), 0xff, 0xff, 0xff, 0xff); // loading string that displays the current value of r,g,b
			SDL_RenderPresent(renderer); // show render to screen	
		}

	}
	

	SDL_DestroyRenderer(renderer); // destroy memory and quit
	SDL_FreeSurface(image);
	SDL_DestroyWindow(w);
	SDL_Quit();

	return EXIT_SUCCESS;

}