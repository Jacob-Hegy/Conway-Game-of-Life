#include <iostream>
#include <SDL.h>
#include <math.h>
#include "cell.h"
#include <windows.h>

using namespace std;

const int x = 200, y = 200; // Dimensions of the grid

void fillInitial(cell life[200][200]) { // Function to fill the array with the proper data for each cell and randomly assign which are alive
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			life[i][j].x = i;
			life[i][j].y = j;
			life[i][j].oldStatus = round(rand() % 2); // If given a random value of 0, the cell is dead, if the value is 1, the cell is alive
		}
	}
}

void nextGen(cell life[200][200]) // Appropriately carries over the data for the next generation to the statuses of each cell
{
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			life[i][j].oldStatus = life[i][j].newStatus;
}

void fillGrid(cell life[200][200], SDL_Renderer *renderer) // Properly paints each pixel to represent a dead or living cell
{
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (life[i][j].oldStatus == 1)
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawPoint(renderer, i, j);
		}
	}
}

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "STD could not initialize! SDL Error: " << SDL_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("Hello SDL World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (window == NULL) {
		cout << "Could not create window! SDL Error: " << SDL_GetError() << endl;
	}
	SDL_Event windowEvent;
	int nSum = 0;
	cell life[200][200]; // The grid is 200x200//
	fillInitial(life);
	cout << "Loop start" << endl;
	while (true){
		if (SDL_PollEvent(&windowEvent))
			if (SDL_QUIT == windowEvent.type)
				break;
		SDL_RenderClear(renderer);
		fillGrid(life, renderer);
		SDL_RenderPresent(renderer);
		//Sleep(10000);
		SDL_RenderClear(renderer);
			for (int i = 0; i < x; i++) { // Check the validity of each neighboring cell and add up the total life count around each cell
				for (int j = 0; j < y; j++) {
					if (j - 1 > 0) // Cell above
						nSum += life[i][j - 1].oldStatus;
					if (j + 1 < y) // Cell below
						nSum += life[i][j + 1].oldStatus;
					if (i - 1 > 0) { // Cells to the left
						nSum += life[i - 1][j].oldStatus;
						if (j - 1 > 0)
							nSum += life[i - 1][j - 1].oldStatus;
						if (j + 1 < y)
							nSum += life[i - 1][j + 1].oldStatus;
					}
					if (i + 1 < x) { // Cells to the right
						nSum += life[i + 1][j].oldStatus;
						if (j - 1 > 0)
							nSum += life[i + 1][j - 1].oldStatus;
						if (j + 1 < y)
							nSum += life[i + 1][j + 1].oldStatus;
					}
					if (life[i][j].oldStatus == 1) { // If the cell is alive to begin with
						if (nSum >= 2 && nSum <= 3) // If the cell has between 2 and 3 neighbors, it lives
							life[i][j].newStatus = 1;
						else // If the cell has less than 2 neighbors or more than 3 neighbors, it dies
							life[i][j].newStatus = 0;
					}
					else { // If the cell is dead to begin with
						if (nSum == 3) // If the cell has exactly 3 neighbors, it lives
							life[i][j].newStatus = 1;
						else // If the cell has less than or more than 3 neighbors, it dies
							life[i][j].newStatus = 0;
					}
					nSum = 0;
				}
			}
			nextGen(life); // Give the cells their new statuses
			fillGrid(life, renderer); // Paint the cells accordingly
			SDL_RenderPresent(renderer); // Show the new rendered window
			Sleep(100); // Sleep for 100 milliseconds so there are five generations per second
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
