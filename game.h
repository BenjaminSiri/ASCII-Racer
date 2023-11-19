#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct Game Game;

struct Game {
	char **board;
	int length;
	char *playerChars;
	int numPlayers;
	int **playerData;
	int numFinishers;
};

void startRandom(Game* self);

Game *newGame(int players, int size){
	Game *new = (Game*)malloc(sizeof(Game));
	new->length = size;

	new->playerChars = (char*)malloc(sizeof(char) * players);
	for(int i=0; i < players; i++){
		new->playerChars[i] = 65+i;
	}

	new->numPlayers = players;

	new->playerData = (int**)malloc(sizeof(int*) * players);
   for (int i = 0; i < players; i++) {
   	new->playerData[i] = (int*)malloc(sizeof(int) * 3);
      for (int j = 0; j < 3; j++) {
     		new->playerData[i][j] = 0;
      }
   }

	new->numFinishers = 0;

	new->board = (char**)malloc(sizeof(char*)*((players * 2)+1));

	for(int i = 0; i < (players*2)+1; i++){
		new->board[i] = (char*)malloc(sizeof(char)*size);
		for(int j = 0; j < size; j++){
			if(j == size-2){
				new->board[i][j] = '|';
			} else {
				if(i%2 == 0){
					new->board[i][j] = '-';
				} else {
					if(j == 0){
						if((i-1)%2 == 0){
							new->board[i][j] = new->playerChars[(i-1)/2];
						}
					} else {
						new->board[i][j] = ' ';
					}
				}
			}
		}
	}

	startRandom(new);

	return new;
}


void startRandom(Game* self){
	for(int i=0; i < self->numPlayers; i++){

		int r = (rand() % 10) + 2;

		self->playerData[i][0] = 0;
		self->playerData[i][1] = r;
	}
}

void changeRandom(Game* self, int clock){
	if(clock % 20 == 0){
		for(int i=0; i<self->numPlayers; i++){
			if(self->playerData[i][1] > 0){

				int r = (rand() % 5) + 2;

				self->playerData[i][1] = r;
			}
		}
	}
}

void fallChance(Game* self){
	for(int i=0; i<self->numPlayers; i++){
		int r = rand() % 1000;
		if(r == 1 && self->playerData[i][1] > 0){
			self->playerData[i][1] = -81;
		}
	}
}

void moveGame(Game* self, int clock){
	for(int i=0; i < self->numPlayers; i++){
		if(self->playerData[i][1] < 0){
			if(clock%2 == 0){
				self->board[(i*2)+1][self->playerData[i][0]] = 'X';
			} else {
				self->board[(i*2)+1][self->playerData[i][0]] = self->playerChars[i];
			}
			self->playerData[i][1] += 2;
		} else {
			if(clock % self->playerData[i][1] == 0){
				self->board[(i*2)+1][self->playerData[i][0]] = ' ';
				self->board[(i*2)+1][self->playerData[i][0]+1] = self->playerChars[i];
				self->playerData[i][0]++;
			}
		}
	}
}

void checkFinish(Game* self){
	for(int i=0; i < self->numPlayers; i++){
		if(self->playerData[i][0] == self->length-1 && self->playerData[i][2] == 0){
			self->numFinishers++;
			self->playerData[i][1] = 0;
			self->playerData[i][2] = self->numFinishers;
		}
	}
}

void writeInit(Game* self){
	for(int i = 0; i < (self->numPlayers*2)+1; i++){
		printf("%s\n",self->board[i]);
	}
	for(int i = 0; i < self->numPlayers; i++){
			printf("%d place: %c\n", i+1, ' ');
		}
}

void writeRep(Game* self){
	printf("\r\033[%dA", (self->numPlayers*3)+1);
	for(int i = 0; i < (self->numPlayers*2)+1; i++){
		printf("%s\n",self->board[i]);
	}

	for(int i = 0; i < self->numPlayers; i++){
		int placeFound = 0;
		for(int j = 0; j < self->numPlayers; j++){
			if(self->playerData[j][2] == i+1){
				printf("%d place: %c\n", i+1, self->playerChars[j]);
				placeFound = 1;
			}
		}
		if(!placeFound){
			printf("%d place: %c\n", i+1, ' ');
		}
	}
}

#endif
