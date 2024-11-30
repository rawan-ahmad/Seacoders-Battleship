#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
// do a probability density function
// do if the user entered an entered spot for the easy mode
// random hits
// method to check if it is entered
// method to check rules and choose the best move

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
int checkShip(int col, int row, int shipSize, char **grid, char direction, const int SIZE, int bot)
{
    // to check the bounds
    if (col >= SIZE || row >= SIZE || col < 0 || row < 0 || col + shipSize > SIZE && tolower(direction) == 'h' || row + shipSize > SIZE && tolower(direction) == 'v')
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
// we can add more smart strategies
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
        // printf("%d %d\n", x, y);
        if (checkShip(x, y, i, grid, d, 10, 1) == 1)
        {
            addShip(x, y, i, grid, d);
            i++;
        }
    }
    system("clear");
}

// added an array to store available moves for the bot
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

// CHANGE (the bot should know the missing and their sizes)
// we should store the last hit move, last sink move and the last nothing move
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
int sunkShip(char **grid, int *lastTurn, int *shipHits, char ship, int bot, char *aim, char c, char r)
{

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (toupper(grid[i][j]) == toupper(ship))
            {
                *lastTurn = 0;
                if (bot == 1)
                {
                    aim[1] = c;
                    aim[2] = r;
                }
                return 0;
            }
        }
    }
    *lastTurn = 1;
    *shipHits = *shipHits + 1;
    if (bot == 1)
    {
        aim[0] = '1';
        aim[1] = c;
        aim[2] = r;
    }
    return 1;
}

// 13. function to fire
void fire(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim)
{
    if (tolower(difficulty) == 'e' && grid[row][col] == '~')
    {
        grid[row][col] = 'o';
        *lastTurn = 0;
        if (bot == 1)
            aim[0] = '0';
        printf("Miss!\n");
    }
    else if (grid[row][col] != '~')
    {
        char test = grid[row][col];
        grid[row][col] = '*';
        printf("Hit!\n");
        if (bot == 1)
            aim[0] = '1';
        if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0') == 1)
            printMessage(toupper(test));
    }
    else
    {
        printf("Miss!\n");
        if (bot == 1)
            aim[0] = '0';
        *lastTurn = 0;
    }
    printGrid(grid, 10);
}
// bot chooses which move to make
// we're going to add probability function for bot to choose based on it fa its gonna change alot
void botHit(char **grid, char difficulty, int *shipHits, int sweeps, int smoke, int *lastTurn, char *aim)
{
    // this is only for the first move the bot should fire randomly
    int row = rand() % (10);
    int col = rand() % (10);
    fire(col, row, grid, difficulty, lastTurn, shipHits, 1, aim);
    char *moves = availableMoves(shipHits, sweeps, smoke, lastTurn, 1);
    int elements = strlen(moves);
    int randomIndex = rand() % elements;
    char bestMove=chooseBestMove(grid, moves, probabilityGrid[10][10], difficulty);
    switch (moves[randomIndex])
    {
    case 'f':
        fire(col, row, grid, difficulty, lastTurn, shipHits, 1, aim);
        break;
    case 'r':
        RadarSweep(col, row, grid);
        break;
    case 's':
        SmokeScreen(col, row, grid);
        break;
    case 'a':
        Artillery(col, row, grid, difficulty, lastTurn, shipHits, 1, aim);
        break;
    case 't':
    {
        int choice = rand() % 2;
        char hit = '\0';
        if (choice == 0)
        {
            hit = 'A' + (rand() % 10);
        }
        else
        {
            hit = '0' + (rand() % 10);
        }
        Torpedo(hit, grid, difficulty, lastTurn, shipHits, 1, aim);
        break;
    }
    }
}
char chooseBestMove(char **grid, char *moves, int probabilityGrid[10][10], char difficulty)
{
    int maxProb=0;
    int row;
    int col;
    char bestMove='\0';
    int fireProb, artilleryProb, torpedoProb;
    for(int i=0; i<strlen(moves);i++){
        char move=moves[i];
        if(move=='f'){
        maxCellProbability(probabilityGrid, &row, &col);
        fireProb=calculateFireProbability(grid,row,col);
        if(maxProb<fireProb){
            maxProb=fireProb;
            bestMove='f';
        }
        }
        else if(moves[i]=='a'){
         artilleryProb=calculateRegionProbability(probabilityGrid);
        if(maxProb<artilleryProb){
            maxProb=artilleryProb;
            bestMove='a';
        }
        }
      else if(moves[i]=='t'){
       torpedoProb=calculateTorpedoProbability(probabilityGrid, row, col);
       if(maxProb<torpedoProb){
        maxProb=torpedoProb;
        bestMove='t';
       }
      }
    }
}
// 14. function to radarsweep
void RadarSweep(int col, int row, char **grid)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (grid[row + i][col + j] <= 10)
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
    printf("Smokescreen is successfully applied.\n");
}

// 16. function to artillery
void Artillery(int col, int row, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim)
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
            }
            else if (grid[row + i][col + j] != '~')
            {
                char test = grid[row + i][col + j];
                grid[row + i][col + j] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0') == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    if (bot == 1)
                        aim[0] = '1';
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
            aim[0] = '1';
    }
    else if (found == 0)
    {
        printf("Miss!\n");
        if (bot == 1)
            aim[0] = '0';
        *lastTurn = 0;
    }
    printGrid(grid, 10);
}

// 17. function to torpedo
void Torpedo(char hit, char **grid, char difficulty, int *lastTurn, int *shipHits, int bot, char *aim)
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
            }
            else if (grid[row][col] != '~')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0') == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    if (bot == 1)
                        aim[0] = '1';
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
            }
            else if (grid[row][col] != '~')
            {
                char test = grid[row][col];
                grid[row][col] = '*';
                if (sunkShip(grid, lastTurn, shipHits, test, bot, aim, col + 'A', row + '0') == 1)
                {
                    sunk++;
                    printf("Hit!\n");
                    if (bot == 1)
                        aim[0] = '1';
                    printMessage(toupper(test));
                }
                hits = 1;
            }
        }
    }

    if (hits == 1 && sunk == 0)
    {
        printf("Hit!\n");
        if (bot == 1)
            aim[0] = '1';
    }
    else if (hits == 0)
    {
        printf("Miss!\n");
        if (bot == 1)
            aim[0] = '0';
        *lastTurn = 0;
    }
    printGrid(grid, 10);
}

// 18. function for a fighting turn per player (mechanism)
void fighting(int c, int r, int *shipHits, int *lastTurn, int *sweeps, int *smoke, char move, char difficulty, char **player1, char **player2, char torp, int delayTime, int bot, char *aim)
{
    if (tolower(move) == 't' && *lastTurn == 1 && *shipHits == 3)
    {
        if (torp - 'A' < 10 && torp - 'A' >= 0 || torp - '0' >= 0 && torp - '0' < 10)
            Torpedo(torp, player2, difficulty, lastTurn, shipHits, bot, aim);
    }
    else if (c >= 10 || c < 0 || r >= 10 || r < 0)
    {
        printf("Your coordinates are invalid!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
    else if (tolower(move) == 'f')
    {
        fire(c, r, player2, difficulty, lastTurn, shipHits, bot, aim);
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
        Artillery(c, r, player2, difficulty, lastTurn, shipHits, bot, aim);
    }
    else
    {
        printf("Your move is unavailable for you!\nYou lost your turn.\n");
        *lastTurn = 0;
    }
    // probability to use fire
    int calculateFireProbability(char **grid, int r, int c)
    {
       
        if (r > 0 && grid[r - 1][c] == '*')
        {
            probabilityGrid[r][c];
        }
        if (r < 9 && grid[r + 1][c] == '*')
        {
              probabilityGrid[r][c];
        }
        if (c > 0 && grid[r][c - 1] == '*')
        {
              probabilityGrid[r][c];
        }
        if (c < 9 && grid[r][c + 1] == '*')
        {
              probabilityGrid[r][c]++;
        }
      return probabilityGrid[r][c];
    }
    void maxCellProbability(int probabilityGrid[10][10], int *row, int *col){
     *row=0; 
     *col=0;
     int maxProb=0;
     for(int r=0; r<=9; r++){
        for(int c=0; c<=9; c++){
            if(probabilityGrid[r][c]>maxProb){
                maxProb=probabilityGrid[r][c];
                *row=r;
                *col=c;
            }
        }
     }
    }
    
    // probability to use Artillery hit
    int calculateRegionProbability(int probabilityGrid[10][10])
    {
        int score = 0;
        int maxScore = -1;
        for (int r = 0; r <= 9; r++)
        {
            for
                int(c = 0; c <= 9; c++)
                {
                    score = probabilityGrid[r][c] + probabilityGrid[r][c + 1] + probabilityGrid[r + 1][c] + probabilityGrid[r + 1][c + 1];
                    if (score > maxScore)
                    {
                        maxScore = score;
                    }
                }
        }
        return maxScore;
    }
    // probability for row torpedo
    int rowProbability(int probabilityGrid[10][10], int row)
    {
        int sum = 0;
        for (int i = 0; i <= 9; i++)
        {
            sum += probabilityGrid[row][i];
        }
        return sum;
    }
    // probability for col torpedo
    int colProbability(int probabilityGrid[10][10], int col)
    {
        int sum = 0;
        for (int i = 0; i < -9; i++)
        {
            sum += probabilityGrid[i][col];
        }
        return sum;
    }
    int calculateTorpedoProbability(int probabilityGrid[10][10], int row, int col){
     int rowProb=rowProbability(probabilityGrid,row);
      int colProb=colProbability(probabilityGrid,col);
      if(rowProb>colProb){
        return rowProb;
      }
      else return colProb;
    }
    // probability for best torpedo target
    char calculateTorpedoTarget(int probabilityGrid[10][10])
    {
        int maxRow = 0; int maxCol = 0;
        for (int r = 0; r <= 9; r++)
        {
            int rowProb = rowProbability(probabilityGrid, r);
            if (maxRow < rowProb)
            {
                maxRow = rowProb;
            }
        }
        for (int c = 0; c <= 9; c++)
        {
            int colProb = colProbability(probabilityGrid, c);
            if (maxCol < colprob)
            {
                maxCol = colProb;
            }
        }
        if (maxRow >= maxCol)
        {
            return 'R';
        }
        else
            return 'C';
    }
}