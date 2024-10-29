// Welcome to our code
// developers: Rawan Ahmad, Tala Chehayeb, Cynthia Khalil, Rafic Rachidi
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "Battleship.h"

int main()
{
  // introduction
  printf("\nWelcome to battleship game! \nCoded by seacoders!\n\n");
  printf("The goal of the game is to sink all 4 of the opponent's ships. The first player to sink all of the opponent's ships wins.");
  
  // for a smoother transition between turns fix delayTime to 1200 or 1500
  const int delayTime = 0;
  // we fixed it at 0 to not risk errors due to the different hardware used by the user

  // ask for difficulty
  int correct = 0;
  char *difficulty = (char *)malloc(sizeof(char) * 4);
  while (correct == 0)
  {
    printf("Choose one of the two tracking difficulty levels:\nEasy or Hard? ");
    scanf("%s", difficulty);
    if (toupper(difficulty[0]) == 'E' || toupper(difficulty[0]) == 'H')
      break;
    else
      printf("Invalid input.\n");
  }

  // ask for names
  char *name1 = (char *)malloc(sizeof(char) * 100);
  char *name2 = (char *)malloc(sizeof(char) * 100);
  printf("\nFor player 1, please enter your name: ");
  scanf("%s", name1);
  printf("For player 2, please enter your name: ");
  scanf("%s", name2);
  system("clear");

  // randomly choosing a player and store player 1 in name 1 for later
  int randomPlayer = rand() % (2) + 1;
  printf("\nA random player is chosen to play first!\n\n ");
  if (randomPlayer != 1)
  {
    char *temp = (char *)malloc(sizeof(char) * 100);
    temp = name1;
    name1 = name2;
    name2 = temp;
    free(temp);
  }

  // adding the ships
  const int SIZE = 10;
  char **player1;
  player1 = (char **)malloc(sizeof(char *) * SIZE);
  for (int i = 0; i < SIZE; i++)
  {
    player1[i] = (char *)malloc(sizeof(char) * SIZE);
  }
  createArray(player1, SIZE);

  char **player2;
  player2 = (char **)malloc(sizeof(char *) * SIZE);
  for (int i = 0; i < SIZE; i++)
  {
    player2[i] = (char *)malloc(sizeof(char) * SIZE);
  }
  createArray(player2, SIZE);

  // each player adds his own ships
  insertingInTurns(name1, name2, player1, player2, "Carrier", 5, delayTime);
  insertingInTurns(name1, name2, player1, player2, "Battleship", 4, delayTime);
  insertingInTurns(name1, name2, player1, player2, "Destroyer", 3, delayTime);
  insertingInTurns(name1, name2, player1, player2, "Submarine", 2, delayTime);

  // beginning the fight
  printf("\nLet the fight begin! \n\nHere are some instructions:\nYour move format should be:\n- move coordinate: Fire B3\n\n");

  int shipHits1 = 0;
  int shipHits2 = 0;

  int sweeps1 = 3;
  int sweeps2 = 3;

  int smoke1 = 0;
  int smoke2 = 0;

  int lastTurn1 = 0;
  int lastTurn2 = 0;

  char *move = (char *)malloc(sizeof(char) * 10);
  char *coord = (char *)malloc(sizeof(char) * 5);

  // turns start here
  while (shipHits1 < 4 && shipHits2 < 4)
  {
    // player 1's turn
    printf("%s, it is your turn to fight!\nThis is your opponents grid:\n", name1);
    printGrid(player2, SIZE);
    availableMoves(shipHits1, sweeps1, smoke1, lastTurn1);
    printf("From the above moves, choose your next one with its coordinate: ");
    scanf("%s %s", move, coord);
    int c = toupper(coord[0]);
    int r = coord[1];

    invert(&c, &r);
    fighting(c, r, &shipHits1, &lastTurn1, &sweeps1, &smoke1, move[0], difficulty[0], player1, player2, coord[0], delayTime);

    if (shipHits1 == 4)
      break;

    // player 2's turn
    printf("\n%s, it is your turn to fight!\nThis is your opponents grid:\n", name2);
    printGrid(player1, SIZE);
    availableMoves(shipHits2, sweeps2, smoke2, lastTurn2);
    printf("From the above moves, choose your next one with its coordinate: ");
    scanf("%s %s", move, coord);
    c = toupper(coord[0]);
    r = coord[1];
    invert(&c, &r);
    fighting(c, r, &shipHits2, &lastTurn2, &sweeps2, &smoke2, move[0], difficulty[0], player2, player1, coord[0], delayTime);
    printf("\n");
  }

  // announcing the winner
  if (shipHits1 == 4)
    printf("%s is the winner!\n", name1);
  else
    printf("%s is the winner!\n", name2);

  // freeing
  free(difficulty);
  free(name1);
  free(player1);
  free(player2);
  free(move);
  free(coord);
}
