#include <stdio.h>

/* Initialize the specified game board. */
void initGameBoard(int gameBoard[][9], int puzzle[][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            gameBoard[row][col] = puzzle[row][col];
        }
    }
}

/* Display the specified game board on the screen.
   You are required to exactly follow the output format stated in the project specification.
   IMPORTANT: Using other output format will result in mark deduction. */
void printGameBoard(int gameBoard[][9]) {
    // Print column numbers
    printf("   1 2 3   4 5 6   7 8 9\n");
    printf(" +-------+-------+-------\n");

    for (int i = 0; i < 9; i++) {
        // Print row number
        printf("%d| ", i+1);

        for (int j = 0; j < 9; j++) {
            // Print cell content
            if(gameBoard[i][j]==0){
                printf("%c ",' ');
            }else if(gameBoard[i][j]=='X'){
                printf("%c ",'X');
            }else{
                printf("%d ",gameBoard[i][j]);
            }
            

            // Print column separator
            if ((j + 1) % 3 == 0 && j < 8) {
                printf("| ");
            }
        }

        // Print row separator
        printf("\n");
        if ((i + 1) % 3 == 0 && i < 8) {
            printf(" |-------|-------|-------\n");
        }
    }

    printf(" +-------+-------+-------\n");
}



/* inputBoard() reads a char '1' to '9', or 'H' from the player.
    You need to apply input validation, output error message, and go back to cell selection if its input is invalid.
    gameMode indicates if the game is run in Easy (0) or Hard (1) mode, so that you can determine if hint is allowed.
    *** The function returns 1 if 'H' is inputted (i.e., the player uses a hint), returns -1 if an invalid input is read, and 0 otherwise (valid input). ***
    IMPORTANT: Using other output format will result in mark deduction. */
int inputBoard(int gameBoard[][9], int x, int y, int sol[][9], int gameMode) {
    char input;

    // Prompt the user for input
    printf("Input a number [or H: hint]: ");
    scanf(" %c", &input);

    // Validate the input
    if ((input >= '1' && input <= '9') || input == 'H' || input == 'h') {
        if (input == 'H' || input == 'h') {
            if (gameMode == 1) {
                printf("No hint in Hard mode.\n");
                return -1; // No hint in Hard mode
            } else {
                // Implement hint logic here
                // You may need to use the 'sol' array to provide a hint to the player
                printf("Hint: The correct number is %d.\n", sol[x][y]);
                return 1; // Player used a hint
            }
        } else {
            // Update the game board with the entered number
            int number = input - '0';
            gameBoard[x][y] = number;
            return 0; // Valid input
        }
    } else {
        printf("Invalid Input.");
        return -1; // Invalid input
    }
}


// This function outputs 1 if the gameBoard is finished (and identical to the solution), and 0 otherwise.
int checkFinish(int gameBoard[][9], int sol[][9]){
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (gameBoard[row][col] != sol[row][col]) {
                return 0; // Game board is not finished
            }
        }
    }
    return 1; // Game board is finished and identical to the solution
}



/* ** Hard Mode **
    Check the (x, y) cell in the game board to see if it conflicts with other numbers in the same row/column/subgrid.
    You need to output messages to indicate incorrect answer.
    if (x,y) cell causes conflict in 2 or more categories, all such messages will be printed in order.
    The function returns 1 if (x, y) causes conflicts, and 0 otherwise.
    Hint: The return value can be used to update chances in main.
    IMPORTANT: Using other output format will result in mark deduction. */

int checkSolutionDetail(int gameBoard[9][9], int x, int y) {
    int num = gameBoard[x][y];

    // Check for collision in the row
    for (int i = 0; i < 9; i++) {
        if (i != y && gameBoard[x][i] == num) {
            //printf("Check again the numbers in row %d.\n", x + 1);
            return 1;  // Collision in the row
        }
    }

    // Check for collision in the column
    for (int i = 0; i < 9; i++) {
        if (i != x && gameBoard[i][y] == num) {
            //printf("Check again the numbers in column %d.\n", y + 1);
            return 2;  // Collision in the column
        }
    }

    // Check for collision in the subgrid
    int subgridStartRow = 3 * (x / 3);
    int subgridStartCol = 3 * (y / 3);
    for (int i = subgridStartRow; i < subgridStartRow + 3; i++) {
        for (int j = subgridStartCol; j < subgridStartCol + 3; j++) {
            if ((i != x || j != y) && gameBoard[i][j] == num) {
                //printf("Check again the numbers in the subgrid where (%d, %d) is at.\n", x + 1, y + 1);
                return 3;  // Collision in the subgrid
            }
        }
    }

    return 0;  // No collision
}

/* checkFillable() checks the row/column/subgrid that the cell (x, y) resides at given the current gameBoard.
    It determines if all the numbers in the same row + column + subgrid contain all numbers 1 - 9, i.e., the cell (x, y) cannot be filled (or is locked).
    It returns 0 if cell (x, y) is locked; and returns 1 if the cell (x, y) can be filled */
int checkFillable(int gameBoard[][9], int x, int y){
    int numPresence[9] = {0}; // Array to track the presence of numbers 1-9

    // Check the row
    for (int col = 0; col < 9; col++) {
        if (gameBoard[x][col] != 0) {
            numPresence[gameBoard[x][col] - 1] = 1;
        }
    }

    // Check the column
    for (int row = 0; row < 9; row++) {
        if (gameBoard[row][y] != 0) {
            numPresence[gameBoard[row][y] - 1] = 1;
        }
    }

    // Check the 3x3 subgrid
    int startRow = (x / 3) * 3;
    int startCol = (y / 3) * 3;

    for (int row = startRow; row < startRow + 3; row++) {
        for (int col = startCol; col < startCol + 3; col++) {
            if (gameBoard[row][col] != 0) {
                numPresence[gameBoard[row][col] - 1] = 1;
            }
        }
    }

    // Check if all numbers 1-9 are present in the row, column, and subgrid
    for (int i = 0; i < 9; i++) {
        if (numPresence[i] == 0) {
            return 1; // The cell can be filled
        }
    }

    return 0; // The cell is locked
}


/* isLockBoard() determines if the input gameBoard contains a locked cell.
    If there is a locked cell, it prints a locked cell and returns 1.
    It returns 0 if the game board is free of locked cell
    Hint: loop through all empty cell and use checkFillable to aid in checking the cell. */
int isLockBoard(int gameBoard[][9]){
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            // Check only empty cells
            if (gameBoard[row][col] == 0) {
                if (!checkFillable(gameBoard, row, col)) {
                    printf("Locked cell at (%d, %d).\n", row, col);
                    return 1; // Locked cell found
                }
            }
        }
    }

    return 0; // No locked cells found
}


/* the savePuzzle function saves the gameBoard and solution to the file "saveGame.txt"
   You may add parameters to this function, e.g., the file to save.
   The program returns 1 if it successfully saves the file, and 0 otherwise. */
int savePuzzle(int gameBoard[][9], int sol[][9]) {
    FILE *fp;

    // Open the file for writing
    fp = fopen("saveGame.txt", "w");

    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 0; // Return 0 if the file cannot be opened
    }

    // Write the content of gameBoard to the file
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            fprintf(fp, "%d ", gameBoard[row][col]);
        }
        fprintf(fp, "\n");
    }

    // Separate gameBoard and sol with a line of dashes
    fprintf(fp, "-----------------\n");

    // Write the content of sol to the file
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            fprintf(fp, "%d ", sol[row][col]);
        }
        fprintf(fp, "\n");
    }

    // Close the file
    fclose(fp);

    // Return 1 to indicate success
    return 1;
}

/* the loadPuzzle function loads the gameBoard and solution from the file "saveGame.txt"
   You may add parameters to this function, e.g., the file to load.
   The program returns 1 if it successfully loads the file, and 0 otherwise. */
int loadPuzzle(int gameBoard[][9], int sol[][9]) {
    FILE *fp;

    // Open the file for reading
    fp = fopen("saveGame.txt", "r");

    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 0; // Return 0 if the file cannot be opened
    }

    // Read the content of gameBoard from the file
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (fscanf(fp, "%d", &gameBoard[row][col]) != 1) {
                printf("Error: Unable to read the file.\n");
                fclose(fp);
                return 0; // Return 0 if unable to read the file
            }
        }
    }

    // Skip the line of dashes separating gameBoard and sol
    fscanf(fp, "-----------------\n");

    // Read the content of sol from the file
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (fscanf(fp, "%d", &sol[row][col]) != 1) {
                printf("Error: Unable to read the file.\n");
                fclose(fp);
                return 0; // Return 0 if unable to read the file
            }
        }
    }

    // Close the file
    fclose(fp);

    // Return 1 to indicate success
    return 1;
}

/* A helper function to craft a text file containing the inputs to fill in the puzzle according to the solution.
   Call it to generate a text file that prints the inputs leading to a finished puzzle.
   To Debug:
       In Powershell, run: Get-Content .\textInput.txt | & .\main.exe
       or simply run the program, copy and paste contents of the text file to console.
   You may modify the function as you wish */
void printSolution(int puzzle[][9], int solution[][9]) {
    FILE *fp;
    fp = fopen("textInput.txt", "w");
    if (fp == NULL) {
        printf("Error in writing file.");
        return;
    }

    fprintf(fp, "0\n"); // Change to 1 if running in Hard mode
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                fprintf(fp, "%d %d\n", i, j);
                fprintf(fp, "%d\n", solution[i][j]); // You may print a character to test Hint/Save-load function.
            }
        }
    }

    fclose(fp);
}

/* The main function */
int main()
{
    /* Local variables */

    // Assume both mode uses same set of puzzle
    // In Part 2, read the puzzle and solution from puzzle.txt
    // You may input your puzzle to debug, e.g., replace some entries of the solution by 0 
    int myPuzzle[9][9]={
        {5,0,0,0,6,3,4,0,0},
        {0,0,0,7,0,0,0,0,0},
        {1,0,0,0,5,0,8,3,0},
        {0,0,0,0,1,8,0,0,7},
        {0,0,6,9,0,0,0,0,0},
        {0,4,3,0,0,0,9,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,7,0,0,2,0},
        {3,2,0,6,4,0,5,0,0}
    };
    // The solution
    int mySolution[9][9] = {
        {5,9,8,1,6,3,4,7,2},
        {6,3,2,7,8,4,1,5,9},
        {1,7,4,2,5,9,8,3,6},
        {2,5,9,4,1,8,3,6,7},
        {8,1,6,9,3,7,2,4,5},
        {7,4,3,5,2,6,9,8,1},
        {4,6,5,8,9,2,7,1,3},
        {9,8,1,3,7,5,6,2,4},
        {3,2,7,6,4,1,5,9,8}
    };
    // Game Board
    int gameBoard[9][9];

    // gameMode: {0: Easy  1: Hard}
    // chances is the no. of chances in hard mode
    // hintCount stores the no. of hint used.
    int gameMode, chances = 3, hintCount = 0;

    // store x, y coordinate of a cell
    int x, y;

    // number will store the puzzle ID (player's puzzle selection)
    int number;

    // Create YOUR local variables
    /* Initialize the local variables */
    int win = 0, validIn = 0, inputValid = 0;
    int check = 0;
    
    // Read the Game mode to gameMode
    do {
        printf("Select game mode (0: Easy, 1: Hard): ");
        scanf("%d", &gameMode);
    } while (gameMode != 0 && gameMode != 1);

    // Call initGameBoard to read the puzzle to gameBoard
    // and call printGameBoard to print it
    initGameBoard(gameBoard, myPuzzle);
    printGameBoard(gameBoard);

    // The following line calls the helper function, which prints the user inputs leading to a finished puzzle
    printSolution(gameBoard, mySolution);

    // Your Easy/Hard mode Game starts
    while (!win) {
        // Let the player select a cell
        // Player selects a cell
        do {
            // Prompt the user for input
            printf("Select a row and column: ");
            scanf("%d %d", &x, &y);
            x--;  // Convert to zero-based index
            y--;  // Convert to zero-based index

            // Check if the selected cell is out of bounds
            if (x < 0 || x >= 9 || y < 0 || y >= 9) {
                printf("Out of bound. Input Again.\n");
                continue;
            }

            // Check if the selected cell is occupied
            if (gameBoard[x][y] != 0) {
                printf("Occupied. Input Again.\n");
            }

        } while (x < 0 || x >= 9 || y < 0 || y >= 9 || gameBoard[x][y] != 0);

        // Print Game Board After the player chose a valid cell (displayed as 'X')
        gameBoard[x][y] = 'X';
        // Print the game board
        printGameBoard(gameBoard);

        // Receive input from the player by calling inputBoard().
        // Use the return value from inputBoard() for program control flow.
        inputValid = inputBoard(gameBoard, x, y, mySolution, gameMode);

        if (inputValid == -1) {
            printf("Please try again.\n");
            // Reset the input value
            gameBoard[x][y] = 0;
            continue; // Go back to cell selection
        }

        if (inputValid == 1) {
            hintCount++;
            inputValid = inputBoard(gameBoard, x, y, mySolution, gameMode);
        }
        

        // Check the answer for different game modes
        if (gameMode == 0 && inputValid != 1) {
            // Easy mode
            if (gameBoard[x][y] != mySolution[x][y]) {
                printf("Incorrect number. Try again.\n");
                // Additional processing if the number is incorrect in Easy mode

                // Reset the input value
                gameBoard[x][y] = 0;

                continue; // Go back to cell selection
            }
            win = checkFinish(gameBoard, mySolution);
        } else {
            // Hard mode
           // Hard mode
            int collisionType = checkSolutionDetail(gameBoard, x, y);
            if (collisionType > 0) {
                // Decrease chances if there is a collision
                chances--;

                // Print specific message based on collision type
                if (collisionType == 1) {
                    printf("Check again the numbers in row %d. ", x + 1);
                } else if (collisionType == 2) {
                    printf("Check again the numbers in column %d. ", y + 1);
                } else if (collisionType == 3) {
                    printf("Check again the numbers in the subgrid where (%d, %d) is at. ", x + 1, y + 1);
                }

                printf("Chances remaining: %d\n", chances);

                if (chances == 0) {
                    printf("You lose!\n");
                    break;
                }

                // Reset the input value
                gameBoard[x][y] = 0;

                continue; // Go back to cell selection
            } else {
               win = checkFinish(gameBoard, mySolution);
                
            }
        }

        // Print the game board
        printGameBoard(gameBoard);

        // If the player wins, exit the while loop, else continue the game.
       if (win) {
            if (gameMode == 0) {
                    printf("Congratulations! You have finished a puzzle in easy mode and used %d hints.\n", hintCount);
            } else {
                    printf("Congratulations! You have finished a puzzle in hard mode with %d chances left.\n", chances);
            }
        }
    }

    return 0;
}