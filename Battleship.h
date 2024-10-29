#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
// game functions will be here

// 1. to delay time before clearing for smoother transition
void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

// 2. function to create the grid
void createArray(char **grid, const int SIZE)
{
    // filling the array
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            grid[i][j] = '~'; // to fill the wave
        }
    }
}

// 3. function to print the array with the ships for locating
void printArray(char **grid, const int SIZE)
{
    printf("\n   ");
    for (int i = 0; i < SIZE; i++)
    {
        printf(" %c ", 'A' + i); // to print the letter coords
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf(" %d ", i); // to print the number coords
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %c ", grid[i][j]); // to print the wave or ship
        }
        printf("\n");
    }
    printf("\n");
}

// 4. function to print the array with the hits and misses
void printGrid(char **grid, const int SIZE)
{
    printf("\n   ");
    for (int i = 0; i < SIZE; i++)
    {
        printf(" %c ", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf(" %d ", i);
        for (int j = 0; j < SIZE; j++)
        {
            if (grid[i][j] == 'o' || grid[i][j] == '*')
                printf(" %c ", grid[i][j]);
            else
                printf(" ~ ");
        }
        printf("\n");
    }
    printf("\n");
}

// 5. function to invert from char (ascii value of char) to indices 
void invert(int *c, int *r)
{
    *c = *c - 'A';
    *r = *r - '0';
}
// 6. function to check if we can insert the ship: 1 if true - 0 if false
int checkShip(char x, char y, int shipSize, char **array, char direction, const int SIZE)
{
    // convert from char to an int
    int xCoordinate = x - 'A';
    int yCoordinate = y - '0';

    // to check the bounds
    if (xCoordinate >= SIZE || yCoordinate >= SIZE || xCoordinate < 0 || yCoordinate < 0 || xCoordinate + shipSize > SIZE && tolower(direction) == 'h' || yCoordinate + shipSize > SIZE && tolower(direction) == 'v')
    {
        printf("\nThe ship is out of bounds; you can't place it here.\nPlease choose another coordinate.\n");
        return 0;
    }

    // to check overlapping

    for (int i = 0; i < shipSize; i++)
    {
        if (tolower(direction) == 'h' && array[yCoordinate][xCoordinate + i] != '~')
        {
            printf("\nThe place is taken. Please choose another coordinate.\n");
            return 0;
        }
        else if (tolower(direction) == 'v' && array[yCoordinate + i][xCoordinate] != '~')
        {
            printf("\nThe place is taken. Please choose another coordinate.\n");
            return 0;
        }
    }

    return 1;
}

// 7. function to add the ship
void addShip(char x, char y, int shipSize, char **array, char direction)
{
    int xCoordinate = x - 'A';
    int yCoordinate = y - '0';

    // identifying the ship
    char ship;
    if (shipSize == 5)
        ship = 'C';
    else if (shipSize == 4)
        ship = 'B';
    else if (shipSize == 3)
        ship = 'D';
    else
        ship = 'S';

    // if vertical we add i to the x coordinate
    if (tolower(direction) == 'v')
    {
        for (int i = 0; i < shipSize; i++)
        {
            array[yCoordinate + i][xCoordinate] = ship;
        }
    }
    // if horizontal we add i to the y coordinate
    else if (tolower(direction) == 'h')
    {
        for (int i = 0; i < shipSize; i++)
        {
            array[yCoordinate][xCoordinate + i] = ship;
        }
    }
}
