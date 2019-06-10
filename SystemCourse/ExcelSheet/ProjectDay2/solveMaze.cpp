
#include "utilities.h"
#include "ratInMaze.h"

int isSafe(int **maze, int rowId, int colId, int rows, int cols ){
	if (rowId >= 0 && colId >= 0 && rowId < rows && colId < cols && maze[rowId][colId] == 1)
		return 1;
	return 0;
}





int solvePuzzle(int **maze, int startRow, int startCol, int endRow, int endCol, int rows, int cols, int **sol){
	if (startRow == endRow && startCol == endCol){
		sol[startRow][startCol] = 1;
		return 1;
	}
	if (isSafe(maze, startRow, startCol, rows, cols) == 1){
		sol[startRow][startCol] = 1;
		if (isSafe(maze, startRow+1, startCol, rows, cols) && sol[startRow + 1][startCol] == 0 && solvePuzzle(maze, startRow + 1, startCol, endRow, endCol, rows, cols, sol) == 1) return 1;
		if (isSafe(maze, startRow, startCol+1, rows, cols) && sol[startRow][startCol + 1] == 0 && solvePuzzle(maze, startRow, startCol + 1, endRow, endCol, rows, cols, sol) == 1) return 1;
		if (isSafe(maze, startRow - 1, startCol, rows, cols) && sol[startRow - 1][startCol] == 0 && solvePuzzle(maze, startRow - 1, startCol, endRow, endCol, rows, cols, sol) == 1) return 1;
		if (isSafe(maze, startRow, startCol - 1, rows, cols) && sol[startRow][startCol - 1] == 0 && solvePuzzle(maze, startRow, startCol - 1, endRow, endCol, rows, cols, sol) == 1) return 1;
		sol[startRow][startCol] = 0;
		return 0;
	}
	return 0;
}

void print(int **sol, int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%d ", sol[i][j]);
		}
		printf("\n");
	}
}

void isPathExists(int **maze, int startRow, int startCol, int endRow, int endCol, int rows, int cols){
	int **sol = (int **)calloc(rows, sizeof(int*));
	for (int i = 0; i < rows; i++){
		sol[i] = (int*)calloc(cols, sizeof(int));
	}
	int res = solvePuzzle(maze, startRow, startCol, endRow, endCol, rows, cols, sol);
	if (res == 0){
		printf("Path Doesnt Exists");
	}
	else print(sol, rows, cols);
}