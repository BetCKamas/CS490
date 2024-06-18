/*
	Betsy Kamas
	CS490 Proj2
	9-23-2023
	
	This code takes an image of the user's chosing at the cmd line and manipulates it. The image's pixels are swapped with
	neighboring pixels in a random order until the progam window is closed. 
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>

using namespace std;

inline int xyOffset(int x, int y, SDL_Surface *s){
	return y*s->w + x; 	// y coord * width of surface + x coord
}

inline void offsetXY(int offset, SDL_Surface *s, int &x, int &y){
	y = offset / s->w;	// setting vertical location by dividing by width
	x = offset % s->w;	// setting horizontal location by mod by width
	return;
}

inline int constrain(int g, int A, int B){
	return min(max(g,A),B);	// makes it so it cannot go past the image bounds
}

int main(int argc, char* args[]) {
	srand(time(nullptr));	// setting seed

	SDL_Window *window = nullptr;	// window struct
	SDL_Surface *screen = nullptr;	// screen surface that will be applied to window
	SDL_Surface *image = nullptr;	// surface that will hold the image to be applied to the screen 
	
	//const char* imageName = args[1];	// grabbing file name from cmd line arg
	image = SDL_LoadBMP("amos.bmp");		// setting image surface as picture
	
	int width = image->w;			// for pixel width of image
	int height = image->h;			// for pixwl height of image
	int tp = width*height;			// for total pixels of image
	int order[tp];					// array that will hold the random order the pixels will be accessed
		
	for(int i = 0; i < tp; i++)		// init array
		order[i] = i;
	
	window = SDL_CreateWindow("Proj 2: kamase", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);			// open window and initializes it with the image
	SDL_BlitSurface(image, NULL, screen, NULL);
	
	SDL_Event e;
	
	while(SDL_PollEvent(&e) == 0 || e.type != SDL_QUIT){
		for(int i = 0; i < tp; i++){		// this for loop randomizes the order of when the pixels will be visited.
			int j = rand() % tp;
			swap(order[i], order[j]);
		}
		
		int x, y = 0;	// coords of pixels
		
		for(int i = 0; i < tp; i++){
			Uint32 *p = (Uint32 *)screen->pixels;							// pointer to pixels
			offsetXY(order[i], screen, x, y);								// taking the random pixel location, the surface it is applied to & setting x/y coords
			int xx = constrain((x+((rand()%3)-1)), 0, (screen->w)-1);		// deciding swap value with regard for left/right neighbor pixel or staying in place
			int yy = constrain((y+((rand()%3)-1)), 0, (screen->h)-1);		// deciding swap value with regard for up/down neighbor pixel or staying in place
			swap(p[xyOffset(x,y,screen)],p[xyOffset(xx,yy,screen)]);		// swapping original pixel with another based on above values
		}
		
		SDL_UpdateWindowSurface(window);	// update screen with swaps
	}

	SDL_FreeSurface(image);		// frees memory of structs
	SDL_FreeSurface(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
