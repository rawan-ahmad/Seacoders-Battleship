// Welcome to our code
// developers: Rawan Ahmad, Tala Chehayeb, Cynthia Khalil, Rafic Rachidi
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Battleship.h"

int main()
{
  // getting the probability
  FILE *myFile;
  int played;
  int frequency[10][10];
  myFile = fopen("probability.txt", "r");
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      fscanf(myFile, "%d", &frequency[i][j]);
    }
  }
  fscanf(myFile, "%d", &played);
  fclose(myFile);

  // introduction
  printf("\nWelcome to battleship game! \nCoded by seacoders!\n\n");
  printf("The goal of the game is to sink all 4 of the opponent's ships.\nThe first player to sink all of the opponent's ships wins.\n\n");

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

  // ask for the name
  char *name = (char *)malloc(sizeof(char) * 100);
  printf("\nPlease enter your name: ");
  scanf("%s", name);
  char *name2 = "The bot";

  system("clear");

  // adding the ships
  const int SIZE = 10;
  char **player = (char **)malloc(sizeof(char *) * SIZE);
  for (int i = 0; i < SIZE; i++)
  {
    player[i] = (char *)malloc(sizeof(char) * SIZE);
  }
  createArray(player, SIZE);

  char **bot = (char **)malloc(sizeof(char *) * SIZE);
  for (int i = 0; i < SIZE; i++)
  {
    bot[i] = (char *)malloc(sizeof(char) * SIZE);
  }
  createArray(bot, SIZE);

  // the player adds his own ships
  insert(player, "Carrier", 5, delayTime, name);
  insert(player, "Battleship", 4, delayTime, name);
  insert(player, "Destroyer", 3, delayTime, name);
  insert(player, "Submarine", 2, delayTime, name);

  char **temp = player;

  // let the bot add his ships
  addMovesBot(bot);
  // printArray(bot, 10);

  // randomly choosing a player
  int randomPlayer = rand() % (2) + 1;
  printf("\nA random player is chosen to play first!\n\n ");

  // beginning the fight
  printf("\nLet the fight begin! \n\nHere are some instructions:\nYour move format should be:\n- move coordinate: Fire B3\n\n");

  int shipHits = 0;
  int shipHitsB = 0;

  int sweeps = 3;
  int sweepsB = 3;

  int smoke = 0;
  int smokeB = 0;

  int lastTurn = 0;
  int lastTurnB = 0;

  char *move = (char *)malloc(sizeof(char) * 10);
  char *coord = (char *)malloc(sizeof(char) * 5);

  char *aim = (char *)malloc(sizeof(char) * 5);
  aim[0] = '0';

  int total = 0;
  int hitButNotSunk = 0;
  int dir = 0;
  // turns start here
  while (shipHits < 4 && shipHitsB < 4)
  {
    // player's turn
    if (randomPlayer == 1)
    {
      printf("%s, it is your turn to fight!\nThis is your opponents grid:\n", name);
      printGrid(bot, "");
      availableMoves(shipHits, sweeps, smoke, lastTurn, 0);
      printf("From the above moves, choose your next one with its coordinate: ");
      scanf("%s %s", move, coord);
      int c = toupper(coord[0]);
      int r = coord[1];
      invert(&c, &r);
      playerMove(c, r, &shipHits, &lastTurn, &sweeps, &smoke, move[0], difficulty[0], player, bot, toupper(coord[0]), delayTime, 0, aim);

      if (shipHits == 4)
        break;
      randomPlayer = 0;
    }

    // bot's turn
    else
    {
      printf("\nIt's the bot's turn\n");
      char *moves = availableMoves(shipHitsB, sweepsB, smokeB, lastTurnB, 1);
      total++;
      botMove(frequency, played, moves, player, difficulty[0], &lastTurnB, &shipHitsB, aim, total, &hitButNotSunk, &dir, bot, shipHits, &smokeB);
      delay(delayTime);
      printf("\n");
      randomPlayer = 1;
    }
  }

  // announcing the winner
  if (shipHits == 4)
    printf("%s is the winner!\n", name);
  else
    printf("The bot is the winner!\n");

  // updating probability for future games
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (temp[i][j] != '~')
      {
        frequency[i][j]++;
      }
    }
  }

  myFile = fopen("probability.txt", "w");
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      fprintf(myFile, "%d ", frequency[i][j]);
    }
    fprintf(myFile, "\n ");
  }
  fprintf(myFile, "%d", played + 1);
  fclose(myFile);

  // freeing
  free(difficulty);
  free(name);
  free(player);
  free(bot);
  free(move);
  free(coord);
  free(aim);
}
