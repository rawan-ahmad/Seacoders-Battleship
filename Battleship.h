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
// 8. function to insert the ships by the player
void insert(char ship[10], int size, char **grid)
{
    char *coordinates = (char *)malloc(sizeof(char) * 2);
    char *direction = (char *)malloc(sizeof(char) * 10);
    printf("Please enter the coordinate to place your %s: ", ship);
    scanf("%s", coordinates);
    printf("Please enter the direction of your %s: ", ship);
    scanf("%s", direction);

    int c = toupper(coordinates[0]);
    int r = coordinates[1];
    invert(&c, &r);

    if (checkShip(c, r, size, grid, direction[0], 10) == 1)
    {
        addShip(c, r, size, grid, direction[0]);
        printArray(grid, 10);
        printf("\nThe ship is succesfully inserted.\n\n");
    }
    else
    {
        printArray(grid, 10);
        free(coordinates);
        free(direction);
        insert(ship, size, grid);
    }
}

// 9. function responsible for adding ships in turns mechanism
void insertingInTurns(char *name1, char *name2, char **player1, char **player2, char *ship, int size, int delayTime)
{
    printf("%s, it is your turn to insert your %s.\n", name1, ship);
    printArray(player1, 10);
    insert(ship, size, player1);
    delay(delayTime);
    system("clear");
    printf("%s, it is your turn to insert your %s.\n", name2, ship);
    printArray(player2, 10);
    insert(ship, size, player2);
    delay(delayTime);
    system("clear");
}

// 10. function to print available moves
void availableMoves(int shipHits, int sweeps, int smoke, int lastTurn)
{
    printf("The available moves are: Fire ");
    if (sweeps > 0)
        printf("RadarSweeps ");
    if (shipHits > smoke)
        printf("SmokeScreen ");
    if (lastTurn == 1)
        printf("Artillery ");
    if (lastTurn == 1 && shipHits == 3)
        printf("Torpedo ");
    printf("\n");
}

// 11. function to print sunk message
void printMessage(char letter)
{
    printf("You sunk the ");
    if (letter == 'C')
        printf("carrier");
    else if (letter == 'D')
        printf("destroyer");
    else if (letter == 'B')
        printf("battleship");
    else
        printf("submarine");
    printf(" ship of your enemy!\n");
}

// 12. function to check if the boat sunk
int sunkShip(char **grid, int *lastTurn, int *shipHits, char ship)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (toupper(grid[i][j]) == toupper(ship))
            {
                *lastTurn = 0;
                return 0;
            }
        }
    }
    *lastTurn = 1;
    *shipHits = *shipHits + 1;
    return 1;
}

// 13. function to fire
void fire(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits)
{
    if (tolower(difficulty) == 'e' && grid[row][col] == '~')
    {
        grid[row][col] = 'o';
        *lastTurn = 0;
        printf("Miss!\n");
    }
    else if (grid[row][col] != '~')
    {
        char test = grid[row][col];
        grid[row][col] = '*';
        printf("Hit!\n");
        if (sunkShip(grid, lastTurn, shipHits, test) == 1)
            printMessage(toupper(test));
    }
    printGrid(grid, 10);
}

// 14. function to radarsweep
void RadarSweep(int col, int row, char **grid)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (grid[row + i][col + j] == 'C' || grid[row + i][col + j] == 'D' || grid[row + i][col + j] == 'S' || grid[row + i][col + j] == 'B')
            {
                printf("Enemy ships found!\n");
                return;
            }
        }
    }
    printf("No enemy ships found!\n");
}

// 15. function to smokescreen
void SmokeScreen(int col, int row, char **array)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (array[row + i][col + j] != '~' || array[row + i][col + j] != 'o')
                array[row + i][col + j] = tolower(array[row + i][col + j]);
        }
    }
}

// 16. function to artillery
void Artillery(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits)
{
    int found = 0;
    int sunk = 0;
    if (col + 2 >= 10 || row + 2 >= 10)
    {
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
        return;
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (grid[row + i][col + j] == '~' && tolower(difficulty) == 'e' || grid[row + i][col + j] == 'o')
            {
                grid[row + i][col + j] = 'o';
                *lastTurn = 0;
            }
            else
            {
                char test = grid[row + i][col + j];
                grid[row + i][col + j] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test) == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    printMessage(toupper(test));
                }
                found = 1;
            }
        }
    }
    if (found == 1 && sunk == 0)
        printf("Hit!\n");
    else if (found == 0)
        printf("Miss!\n");
    printGrid(grid, 10);
}
// 17. function to torpedo
void Torpedo(char hit, char **grid, char difficulty, int *lastTurn, int *shipHits)
{
    int col = hit - 'A';
    int row = hit - '0';
    int hits = 0;
    int sunk = 0;
    if (col >= 0 && col < 10)
    {
        for (row = 0; row < 10; row++)
        {
            if (tolower(difficulty) == 'e' && grid[row][col] == '~' || grid[row][col] == 'o')
            {
                grid[row][col] = 'o';
                *lastTurn = 0;
            }
            else if (grid[row][col] != '~')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test) == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    printMessage(toupper(test));
                }
                hits = 1;
            }
        }
    }
    else
    {
        for (col = 0; col < 10; col++)
        {
            if (tolower(difficulty) == 'e' && grid[row][col] == '~' || grid[row][col] == 'o')
            {
                grid[row][col] = 'o';
                *lastTurn = 0;
            }
            else if (grid[row][col] != '~')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test) == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    printMessage(toupper(test));
                }
                hits = 1;
            }
        }
    }

    if (hits == 1 && sunk == 0)
        printf("Hit!\n");
    else if (hits == 0)
        printf("Miss!\n");
    printGrid(grid, 10);
}

// 18. function for a fighting turn per player (mechanism)
void fighting(int c, int r, int *shipHits, int *lastTurn, int *sweeps, int *smoke, char move, char difficulty, char **player1, char **player2, char torp, int delayTime)
{
    if (tolower(move) == 't' && *lastTurn == 1 && *shipHits == 3)
    {
        if (torp - 'A' < 10 && torp - 'A' >= 0 || torp - '0' >= 0 && torp - '0' < 10)
            Torpedo(torp, player2, difficulty, lastTurn, shipHits);
    }
    else if (c >= 10 || c < 0 || r >= 10 || r < 0)
    {
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
    else if (tolower(move) == 'f')
    {
        fire(c, r, player2, difficulty, lastTurn, shipHits);
    }
    else if (tolower(move) == 'r' && *sweeps > 0)
    {
        RadarSweep(c, r, player2);
        *sweeps = *sweeps - 1;
        *lastTurn = 0;
    }
    else if (tolower(move) == 's' && *shipHits > *smoke)
    {
        SmokeScreen(c, r, player1);
        *smoke = *smoke + 1;
        *lastTurn = 0;
        delay(delayTime);
        system("clear");
    }
    else if (tolower(move) == 'a' && *lastTurn == 1)
    {
        Artillery(c, r, player2, difficulty, lastTurn, shipHits);
    }
    else
    {
        printf("Your move is unavailable for you!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
}