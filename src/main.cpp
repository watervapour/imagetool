#include <iostream>
#include <fstream>
#include <filesystem>

#include "image.h"

#include <SDL2/SDL.h>
SDL_Surface* gWindowSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
bool setupGraphics();
void drawGraphics(imageFormat);
void close();

int main(int argc, char** argv){
	std::cout << "argc: " << argc <<"\n";
	std::filesystem::path p;
	if(argc == 1) {
		// no commandline arguments
		std::cout << "Insufficent args, using default file.\n";
		p = "testfiles/imageCAT.ppm";
	} else {
		p = argv[1];
		std::cout << "path provided! \n";
	}

	// verify file exists
	bool fileExists = std::filesystem::exists(p);
	if(!fileExists){
		std::cerr << "No file, aborting.\n";
		return 1;
	}
	std::cout << p << "\n"; 
	std::cout << "Size: " << std::filesystem::file_size(p) << " bytes\n"; 

	
	std::filebuf buffer;
	if(!buffer.open(p, std::ios::binary|std::ios::in) ){
		std::cerr << "Failed to open file.\n";
		return 1;
	} else {
		std::cout << "File read success.\n";
	}

	if(!setupGraphics())
		return 1;
	
	std::cerr << "Reminder: colour correction for gamme, etc. is not implemented\n";
	
	ifPPM imageToHandle;
	imageToHandle.decode(&buffer);

	SDL_Event e;
	bool quit = false;
	while(!quit){
		drawGraphics(imageToHandle);
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT) { quit=true; break;}
			else if(e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_q:
						quit = true;
						break;
				}
			}
		}
	}

	close();
	return 0;
}

bool setupGraphics(){
	bool success = true;
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "Could not initialise SDL: " << SDL_GetError() << "\n";
		success = false;
	} else {
		gWindow = SDL_CreateWindow("Image tool", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			std::cerr << "Could not create window: " << SDL_GetError() << "\n";
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				std::cerr << "Could not create renderer: " << SDL_GetError() << "\n";
				success = false;
			}
		}
	}

	return success;
}

void drawGraphics(imageFormat input){
	int width = input.img.getWidth();
	int height = input.img.getHeight();
	SDL_SetWindowSize(gWindow, width, height);	

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xDD, 0xDD);
	SDL_RenderClear(gRenderer);

	SDL_Rect pixel = {0,0,1,1};
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			colour c = input.img.getPixelColour(x,y); 
			SDL_SetRenderDrawColor(gRenderer, c.channel[0], c.channel[1], c.channel[2], (char)c.channel[3]);
			pixel.x = x;
			pixel.y = y;
			SDL_RenderFillRect( gRenderer, &pixel);	
		}
	}

	SDL_RenderPresent(gRenderer);
}

void close(){
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_Quit;
}
