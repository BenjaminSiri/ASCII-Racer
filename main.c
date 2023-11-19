#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "game.h"


int main (int argc, char* argv[]){

	srand(time(NULL));

	Game *myGame;

	if(argc == 1){
		myGame = newGame(4, 50);
	} else {
		myGame = newGame(atoi(argv[1]), atoi(argv[2]));
	}

	writeInit(myGame);
	int clock = 0;

	while(myGame->numPlayers != myGame->numFinishers){
		moveGame(myGame, clock);
		checkFinish(myGame);
		writeRep(myGame);

		clock++;
		fallChance(myGame);
		changeRandom(myGame, clock);

		usleep(100000);
	}

	return 0;
}
