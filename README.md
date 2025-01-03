# Seacoders-Battleship
CMPS 270
Battleship game 
Rawan Ahmad, Tala Chehayeb, Cynthia Khalil, Rafic Rachidi

The Battleship game is a two-player game where each player should target the enemy’s ships by different types of attacking and trying to sink all the ships first to win. There are 4 ships for each player placed on a 10x10 grid, and we have implemented a version, where which allows a player to play against a bot. We created two grids for each player, one grid to insert their ships, using the insert method, and to record their hits and misses against their opponent for each. 

Phase 1: 
We used the same grid for firing and placing ships.
Phase 2: 
For placing ships: we used the random strategy where the bot randomly chooses the coordinates and orientation of its ships.
For firing: we used the random firing and hunt and destroy strategies.
•	The random firing: randomly choosing coordinates to fire from the frequency grid (updated after each game) that contains the most frequent coordinates where ships may be placed.
•	The hunt and destroy: once the bot hit a spot, it fires nearby the hit to know the orientation and sink it in future moves.
•	Choosing the suitable moves: choose the most powerful move available and ignore moves that cut the bots time (radar sweep), in case of sinking 2 or more of the bots’ ship: smokescreen will be used when available. 

Description of the implemented functions:

1. delay: function for better smooth transition while playing
2. createArray: creates a grid 10x10 and fills it with ~
3. printArray: prints the arrays to show where the ships are located
4. printGrid: prints the grid to show the opponent the hits and misses
5. invert: converts characters to indices
6. checkShip: checks if we can insert the ship, and returns 1 if true or 0 if false (checks if ship is out of bounds or if there are ships overlaping)
7. addShip: add ships to the grid
8. insert: allows each player to insert their ships by asking for the coordinates and returns if it was successfully inserted
9. addMovesBot: bot adds ships randomly
10. availableMove: prints the available moves to use in each turn
11. printMessage: prints which boat was sunk
12. sunkShip: checks if the ship sunk and sets last turn to 1 if it has
13. fire: fires a shot at specific coordinates, determines if it was a hit or miss, and updates the grid accordingly
14. RadarSweep: scans a 2x2 area in the grid to detect whether there are enemy ships while also making boundary checks,and prints the result 
15. SmokeScreen: simulates creating a smokescreen on a 2x2 area on the grid by converting the ships and misses to lowercase, and prints if it was successfully applied
16. Artillery: targets a 2x2 area in the grid, marks the hits and misses, handles invalid coordinates, and checks if it has sunk a ship to print the result
17. Torpedo: targets an entire row or column depending on the input, keeping track of the hits and misses to update the grid, and printing if it sunk a ship
18. playerMove: handles the player's actions based on their input, prints if their move and coordinates are valid, and makes sure the turns are managed correctly
19. previouslyHit: checks if a cell was previously hit
20. randomHit: allows bot to randomly select coordinates on the grid to hit and to choose a move, and makes sure the frequency of hitting in that area is not too high
21. bestMove: returns the first valid move in the list of available moves, and it prioritizes the latest move
22. botMove: allows bot to make strategic decisions based on its position, previous hits, and available moves
