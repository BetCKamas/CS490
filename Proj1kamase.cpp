/*
	Betsy Kamas
	CS490m Proj1
	9-22-2023
	
	This code is based upon the first piece of given code for this class: test.cpp. It has been modified to gently
	slide through the different colors by incrementing/decrementing the r, g, or b value by 1 each time. If the
	individual value of the color will exceed the defined 0-255, they are forced to stay within those limits due
	to the work	of the constrain function.
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL.h>

using namespace std;

const int WIDTH = 300;		// These are used for the window size
const int HEIGHT = 300;
const int MINRGB = 0;		// Used as parameters for constrain function
const int MAXRGB = 255;

Uint32 myrand(){			// This function creates 3 random numbers using the rand() function and XORs them together
							// for more randomness
	Uint32 result = rand();
	Uint32 A = rand() << 15;
	Uint32 B = rand() << 30;
	
	result = result ^ A ^ B;
	
	return result;
}

inline int constrain(int x, int A, int B){
	return min(max(x,A),B);
}

int main(int argc, char* args[]) {
	srand(time(nullptr));
	SDL_Window *w = nullptr;
	SDL_Surface *s = nullptr;

	w = SDL_CreateWindow("Proj1: kamase", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Event e;
	
	Uint32 red = myrand();		// creating random values for each color
	Uint32 green = myrand();
	Uint32 blue = myrand();
	
	while(SDL_PollEvent(&e) == 0 || e.type != SDL_QUIT){
		int value = rand() % 3;				// This if/else tree determines based on a random value whether to change
		if(value == 2)						// the amount of red green or blue and changes them based on their previous value
			red = constrain((red+((rand()%3)-1)), MINRGB, MAXRGB);
		else if(value == 1)
			green = constrain((green+((rand()%3)-1)), MINRGB, MAXRGB); 
		else
			blue = constrain((blue+((rand()%3)-1)), MINRGB, MAXRGB);
		s = SDL_GetWindowSurface(w);
		SDL_FillRect(s, nullptr, SDL_MapRGB(s->format, red, green, blue));	// filling with the color
		SDL_UpdateWindowSurface(w);											// displaying the color
	}

	SDL_DestroyWindow(w);
	SDL_Quit();

	return EXIT_SUCCESS;
}
