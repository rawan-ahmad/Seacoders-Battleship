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
  // check input

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
  createArray(player2, SIZE);