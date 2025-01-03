#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// NOTES: created an array called aim:
// at coord 0: 0 if miss, 1 if hit without sink, 2 if sink
// at coord 1 and 2 are the last move coordinates

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
void printGrid(char **grid, char *space, char diff)
{
    printf("\n%s   ", space);
    for (int i = 0; i < 10; i++)
    {
        printf(" %c ", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        printf("%s %d ", space, i);
        for (int j = 0; j < 10; j++)
        {
            if (grid[i][j] == 'o' && tolower(diff) == 'e' || grid[i][j] == '*')
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
int checkShip(int col, int row, int shipSize, char **grid, char direction, const int SIZE, int bot)
{
    // to check the bounds
    if (col >= SIZE || row >= SIZE || col < 0 || row < 0 || col + shipSize > SIZE && tolower(direction) == 'h' || row + shipSize > SIZE && tolower(direction) == 'v' || tolower(direction) != 'h' && tolower(direction) != 'v')
    {
        if (bot == 0)
            printf("\nYour coordinates are invalid; you can't place it here.\nPlease choose another coordinate.\n");
        return 0;
    }

    // to check overlapping
    for (int i = 0; i < shipSize; i++)
    {
        if (tolower(direction) == 'h' && grid[row][col + i] != '~' || tolower(direction) == 'v' && grid[row + i][col] != '~')
        {
            if (bot == 0)
                printf("\nThe place is taken. Please choose another coordinate.\n");
            return 0;
        }
    }

    return 1;
}

// 7. function to add the ship to the grid
void addShip(int col, int row, int shipSize, char **grid, char direction)
{
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

    // if vertical we add i to the row
    if (tolower(direction) == 'v')
    {
        for (int i = 0; i < shipSize; i++)
        {
            grid[row + i][col] = ship;
        }
    }
    // if horizontal we add i to the column
    else if (tolower(direction) == 'h')
    {
        for (int i = 0; i < shipSize; i++)
        {
            grid[row][col + i] = ship;
        }
    }
}

// 8. function to insert the ships by the player
void insert(char **grid, char ship[10], int size, int delayTime, char *name)
{
    printf("%s, it is the time to insert your %s ship.\n", name, ship);
    printArray(grid, 10);
    char *coordinates = (char *)malloc(sizeof(char) * 2);
    char *direction = (char *)malloc(sizeof(char) * 10);
    printf("Please enter the coordinate to place your %s: ", ship);
    scanf("%s", coordinates);
    printf("Please enter the direction of your %s: ", ship);
    scanf("%s", direction);

    int c = toupper(coordinates[0]);
    int r = coordinates[1];
    invert(&c, &r);

    if (checkShip(c, r, size, grid, direction[0], 10, 0) == 1)
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
        insert(grid, ship, size, delayTime, name);
    }

    delay(delayTime);
    system("clear");
}

// 9. adding bot ships randomly
void addMovesBot(char **grid)
{
    printf("Loading ...\nThe bot is placing his ships ...\n");
    int i = 2;
    int x, y;
    while (i <= 5)
    {
        srand(time(NULL));
        x = rand() % (10);
        y = rand() % (10);
        int dir = rand() % (2) + 1;
        char d;
        if (dir == 1)
        {
            d = 'h';
        }
        else
            d = 'v';
        if (checkShip(x, y, i, grid, d, 10, 1) == 1)
        {
            addShip(x, y, i, grid, d);
            i++;
        }
    }
    system("clear");
}

// 10. function to print available moves
char *availableMoves(int shipHits, int sweeps, int smoke, int lastTurn, int bot)
{
    char *moves = (char *)malloc(sizeof(char) * 5);
    int i = 0;
    if (bot == 1)
        moves[i++] = 'f';
    printf("The available moves are: Fire ");
    if (sweeps > 0)
    {
        if (bot == 1)
            moves[i++] = 'r';
        printf("RadarSweeps ");
    }
    if (shipHits > smoke)
    {
        if (bot == 1)
            moves[i++] = 's';
        printf("SmokeScreen ");
    }
    if (lastTurn == 1)
    {
        if (bot == 1)
            moves[i++] = 'a';
        printf("Artillery ");
    }
    if (lastTurn == 1 && shipHits == 3)
    {
        if (bot == 1)
            moves[i++] = 't';
        printf("Torpedo ");
    }
    printf("\n");
    if (bot == 1)
    {
        return moves;
    }
    else
        return 0;
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
int sunkShip(char **grid, int *lastTurn, int *shipHits, char ship, int bot, char *aim, char c, char r, int *hitButNotSunk)
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
    if (bot == 1)
    {
        *hitButNotSunk = 0;
        aim[0] = '2';
        aim[1] = c;
        aim[2] = r;
    }
    return 1;
}

// 13. function to fire
void fire(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim, int *hitButNotSunk, int prob)
{
    if (col >= 10 || col < 0 || row >= 10 || row < 0)
    {
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
        return;
    }
    if (tolower(difficulty) == 'e' && grid[row][col] == '~' || grid[row][col] == 'o' || tolower(difficulty) == 'h' && grid[row][col] == '~' && prob == 1)
    {
        grid[row][col] = 'o';
        *lastTurn = 0;
        printf("Miss!\n");
    }
    else if (grid[row][col] != '~' && grid[row][col] != 'o')
    {
        char test = grid[row][col];
        grid[row][col] = '*';
        printf("Hit!\n");
        if (bot == 1)
        {
            aim[0] = '1';
            aim[1] = 'A' + col;
            aim[2] = '0' + row;
            *hitButNotSunk = 1;
        }
        if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0', hitButNotSunk) == 1)
            printMessage(toupper(test));
    }
    else
    {
        printf("Miss!\n");
        *lastTurn = 0;
    }
    char *space;
    if (bot == 1)
    {
        space = "                                         ";
    }
    else
        space = "";
    printGrid(grid, space, difficulty);
}

// 14. function to radarsweep
void RadarSweep(int col, int row, char **grid)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (row + i < 10 && col + j < 10)
            {
                if (grid[row + i][col + j] == 'C' || grid[row + i][col + j] == 'D' || grid[row + i][col + j] == 'S' || grid[row + i][col + j] == 'B')
                {
                    printf("Enemy ships found!\n");
                    return;
                }
            }
            else
            {
                printf("Cannot do RadarSweep because index out of bounds!");
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
            if (row + i < 10 && col + j < 10)
            {
                if (array[row + i][col + j] != '~' || array[row + i][col + j] != 'o')
                    array[row + i][col + j] = tolower(array[row + i][col + j]);
            }
        }
    }
    printf("The move is successfully applied.\n");
}

// 16. function to artillery
void Artillery(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim, int *hitButNotSunk, int prob)
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
            if (grid[row + i][col + j] == '~' && tolower(difficulty) == 'e' || grid[row + i][col + j] == 'o' || tolower(difficulty) == 'h' && grid[row + i][col + j] == '~' && prob == 1)
            {
                grid[row + i][col + j] = 'o';
            }
            else if (grid[row + i][col + j] != '~' && grid[row + i][col + j] != 'o')
            {
                char test = grid[row + i][col + j];
                grid[row + i][col + j] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0', hitButNotSunk) == 1)
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
    {
        printf("Hit!\n");
        if (bot == 1)
        {
            aim[0] = '1';
            aim[1] = 'A' + col;
            aim[2] = '0' + row;
            *hitButNotSunk = 1;
        }
    }
    else if (found == 0)
    {
        printf("Miss!\n");
        *lastTurn = 0;
    }
    char *space;
    if (bot == 1)
    {
        space = "                                         ";
    }
    else
        space = "";
    printGrid(grid, space, difficulty);
}

// 17. function to torpedo
void Torpedo(char hit, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim, int *hitButNotSunk, int prob)
{
    int col = hit - 'A';
    int row = hit - '0';
    int hits = 0;
    int sunk = 0;

    if (col >= 0 && col < 10)
    {
        for (row = 0; row < 10; row++)
        {
            if (tolower(difficulty) == 'e' && grid[row][col] == '~' || grid[row][col] == 'o' || tolower(difficulty) == 'h' && grid[row][col] == '~' && prob == 1)
            {
                grid[row][col] = 'o';
            }
            else if (grid[row][col] != '~' && grid[row][col] != 'o')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0', hitButNotSunk) == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    printMessage(toupper(test));
                }
                hits = 1;
            }
        }
    }
    else if (row >= 0 && row < 10)
    {
        for (col = 0; col < 10; col++)
        {
            if (tolower(difficulty) == 'e' && grid[row][col] == '~' || grid[row][col] == 'o' || tolower(difficulty) == 'h' && grid[row][col] == '~' && prob == 1)
            {
                grid[row][col] = 'o';
            }
            else if (grid[row][col] != '~' && grid[row][col] != 'o')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0', hitButNotSunk) == 1)
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
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
        return;
    }

    if (hits == 1 && sunk == 0)
    {
        printf("Hit!\n");
        if (bot == 1)
        {
            aim[0] = '1';
            aim[1] = 'A' + col;
            aim[2] = '0' + row;
            *hitButNotSunk = 1;
        }
    }
    else if (hits == 0)
    {
        printf("Miss!\n");
        *lastTurn = 0;
    }
    char *space;
    if (bot == 1)
    {
        space = "                                         ";
    }
    else
        space = "";
    printGrid(grid, space, difficulty);
}

// 18. function for a fighting turn per player (mechanism)
void playerMove(int c, int r, int *shipHits, int *lastTurn, int *sweeps, int *smoke, char move, char difficulty, char **player, char **enemy, char torp, int delayTime, int bot, char *aim)
{
    if (tolower(move) == 't' && *lastTurn == 1 && *shipHits == 3)
    {
        if (torp - 'A' < 10 && torp - 'A' >= 0 || torp - '0' >= 0 && torp - '0' < 10)
            Torpedo(torp, enemy, difficulty, lastTurn, shipHits, bot, aim, 0, 0);
    }
    else if (c >= 10 || c < 0 || r >= 10 || r < 0)
    {
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
    else if (tolower(move) == 'f')
    {
        fire(c, r, enemy, difficulty, lastTurn, shipHits, bot, aim, 0, 0);
    }
    else if (tolower(move) == 'r' && *sweeps > 0)
    {
        RadarSweep(c, r, enemy);
        *sweeps = *sweeps - 1;
        *lastTurn = 0;
    }
    else if (tolower(move) == 's' && *shipHits > *smoke)
    {
        SmokeScreen(c, r, player);
        *smoke = *smoke + 1;
        *lastTurn = 0;
        delay(delayTime);
        system("clear");
    }
    else if (tolower(move) == 'a' && *lastTurn == 1)
    {
        Artillery(c, r, enemy, difficulty, lastTurn, shipHits, bot, aim, 0, 0);
    }
    else
    {
        printf("Your move is unavailable for you!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
}

int previouslyHit(int c, int r, char **grid)
{
    if (grid[r][c] == 'o' || grid[r][c] == '*')
        return 1;
    else
        return 0;
}

void randomHit(int frequency[10][10], int played, char move, char **grid, char difficulty, int *lastTurn, int *shipHits, char *aim, int total, int *hitButNotSunk, int *smoke, int prob)
{
    int x = rand() % 10;
    int y = rand() % 10;
    if (total > 10)
        played = played / 2;
    if (frequency[x][y] >= (played / 2) && previouslyHit(x, y, grid) == 0)
    {
        if (x >= 10 || x < 0 || y >= 10 || y < 0)
        {
            printf("Your coordinates are invalid!\nYou lost your turn.\n");
            *lastTurn = 0;
            return;
        }

        switch (move)
        {
        case 'f':
            printf("Fire %c%c\n", x + 'A', y + '0');
            fire(x, y, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
            break;
        case 'r':
            printf("RadarSweep %c%c\n", x + 'A', y + '0');
            RadarSweep(x, y, grid);
            break;
        case 's':
            SmokeScreen(x, y, grid);
            *smoke = *smoke + 1;
            *lastTurn = 0;
            system("clear");
            break;
        case 'a':
            if (x + 2 >= 10)
                x -= 2;
            if (y + 2 >= 10)
                y -= 2;
            printf("Artillery %c%c\n", x + 'A', y + '0');
            Artillery(x, y, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
            break;
        case 't':
        {
            int choice = rand() % 2;
            char hit;
            if (choice == 1)
            {
                hit = 'A' + (x);
            }
            else
            {
                hit = '0' + (y);
            }
            printf("Torpedo %c\n", hit);
            Torpedo(hit, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
            break;
        }
        }
    }
    else
        randomHit(frequency, played, move, grid, difficulty, lastTurn, shipHits, aim, total, hitButNotSunk, smoke, prob);
}

char bestMove(char *moves, int shipHits)
{
    if (moves[4] != '\0')
    {
        return moves[4];
    }
    if (moves[3] != '\0')
    {
        return moves[3];
    }
    if (shipHits >= 2 && moves[2] != '\0')
    {
        return moves[2];
    }
    if (moves[0] != '\0')
    {
        return moves[0];
    }
    return 'n';
}

void botMove(int frequency[10][10], int played, char *moves, char **grid, char difficulty, int *lastTurn, int *shipHits, char *aim, int total, int *hitButNotSunk, int *dir, char **bot, int enemyHits, int *smoke, int prob)
{

    if (aim[0] == '0' && *hitButNotSunk == 0 || aim[0] == '2')
    {
        char move = bestMove(moves, enemyHits);
        randomHit(frequency, played, move, grid, difficulty, lastTurn, shipHits, aim, total, hitButNotSunk, smoke, prob);
    }
    else if (aim[0] == '1' || *hitButNotSunk == 1)
    {
        int x = aim[1] - 'A';
        int y = aim[2] - '0';
        int c = x;
        int r = y;

        int count = 0;
        do
        {
            if (grid[r + 1][c] == 'o' && grid[r - 1][c] == 'o')
            {
                *dir = 3;
            }
            if (*dir == 1 && r - 1 >= 0)
            {
                r--;
                if (grid[r][c] == 'o')
                {
                    while (r + 1 <= 9 && grid[r + 1][c] == '*' && grid[r + 1][c] != 'o')
                    {
                        r++;
                        *dir = 2;
                    }
                }
            }
            else if (*dir == 2 && r + 1 < 10)
            {
                r++;
                if (grid[r][c] == 'o')
                {
                    while (r - 1 >= 0 && grid[r - 1][c] == '*' && grid[r - 1][c] != 'o')
                    {
                        r--;
                        *dir = 1;
                    }
                }
            }
            else if (*dir == 3 && c - 1 >= 0)
            {
                c--;
                if (grid[r][c] == 'o')
                {
                    while (c + 1 <= 9 && grid[r][c + 1] == '*' && grid[r][c + 1] != 'o')
                    {
                        c++;
                        *dir = 4;
                    }
                }
            }
            else if (*dir == 4 && c + 1 < 10)
            {
                c++;
                if (grid[r][c] == 'o')
                {
                    while (c - 1 >= 0 && grid[r][c - 1] == '*' && grid[r][c - 1] != 'o')
                    {
                        c--;
                        *dir = 3;
                    }
                }
            }
            else
            {
                if (r - 1 >= 0 && grid[r - 1][c] != 'o' && grid[r - 1][c] != '*')
                { // up
                    while (r >= 0 && grid[r][c] == '*' && grid[r][c] != 'o')
                    {
                        r--;
                        *dir = 1;
                    }
                }
                else if (r == y && r + 1 <= 9 && grid[r + 1][c] != 'o' && grid[r + 1][c] != '*')
                { // down
                    while (r <= 9 && grid[r][c] == '*' && grid[r][c] != 'o')
                    {
                        r++;
                        *dir = 2;
                    }
                }

                else
                {
                    r = y;
                    if (c - 1 >= 0 && grid[r][c - 1] != 'o' && grid[r][c - 1] != '*')
                    { // left
                        while (c >= 0 && grid[r][c] == '*' && grid[r][c] != 'o')
                        {
                            c--;
                            *dir = 3;
                        }
                    }
                    else if (c == x && c + 1 <= 9 && grid[r][c + 1] != 'o' && grid[r][c + 1] != '*')
                    { // right
                        while (c <= 9 && grid[r][c] == '*' && grid[r][c] != 'o')
                        {
                            c++;
                            *dir = 4;
                        }
                    }
                }
            }
            count++;
            if (count > 50)
            {
                break;
            }
        } while (previouslyHit(c, r, grid) == 1);

        char move = bestMove(moves, enemyHits);

        if (count > 50)
        {
            aim[0] = '0';
            *hitButNotSunk = 0;
            randomHit(frequency, played, move, grid, difficulty, lastTurn, shipHits, aim, total, hitButNotSunk, smoke, prob);
        }
        else
        {
            if (c >= 10 || c < 0 || r >= 10 || r < 0)
            {
                printf("Your coordinates are invalid!\nYou lost your turn.\n");
                *lastTurn = 0;
                return;
            }
            switch (move)
            {
            case 'f':
                printf("Fire %c%c\n", c + 'A', r + '0');
                fire(c, r, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
                break;
            case 'r':
                printf("RadarSweep %c%c\n", c + 'A', r + '0');
                RadarSweep(c, r, grid);
                break;
            case 's':
                for (int i = 0; i < 10; i++)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        if (bot[i][j] != '*' && bot[i][j] != '~' && bot[i][j] != 'o')
                        {
                            c = i;
                            r = j;
                            break;
                        }
                    }
                }
                SmokeScreen(c, r, grid);
                *smoke = *smoke + 1;
                *lastTurn = 0;
                system("clear");
                break;
            case 'a':
                if (c + 2 >= 10)
                    c -= 2;
                if (r + 2 >= 10)
                    r -= 2;
                printf("Artillery %c%c\n", c + 'A', r + '0');
                Artillery(c, r, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
                break;
            case 't':
            {
                int choice = rand() % 2;
                char hit;
                if (choice == 1)
                {
                    hit = 'A' + (c);
                }
                else
                {
                    hit = '0' + (r);
                }
                printf("Torpedo %c\n", hit);
                Torpedo(hit, grid, difficulty, lastTurn, shipHits, 1, aim, hitButNotSunk, prob);
                break;
            }
            }
        }
        if (aim[0] == '2')
            *dir = 0;
    }
}
