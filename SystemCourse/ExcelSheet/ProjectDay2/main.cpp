#include "utilities.h"

#include "ratInMaze.h"

int main(void){
	int rows, cols;
	printf("Enter the rows and cols");
	scanf_s("%d %d", &rows, &cols);
	int **maze = (int **)calloc(rows, sizeof(int *));
	for (int i = 0; i < rows; i++){
		maze[i] = (int *)calloc(cols, sizeof(int));
	}
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			scanf_s("%d", &maze[i][j]);
		}
	}
	int startRow, startCol, endRow, endCol;
	printf("Enter the startRow and startCol");
	scanf_s("%d %d", &startRow, &startCol);
	printf("Enter the terminatePath");
	scanf_s("%d %d", &endRow, &endCol);
	isPathExists(maze, startRow, startCol, endRow, endCol,rows, cols);
	getchar();
}