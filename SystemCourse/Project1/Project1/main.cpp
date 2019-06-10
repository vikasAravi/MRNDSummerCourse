#include "utilities.h"
#include "commandOperations.h"

int main(void){
	int rowId = 10;
	int colId = 10;
	Cell **excelMatrix = (Cell **)calloc(rowId, sizeof(Cell *));
	for (int i = 0; i < rowId; i++){
		excelMatrix[i] = (Cell *)calloc(colId, sizeof(Cell));
	}
	char *givenInput = (char *)malloc(100 * sizeof(char));
	int flag = 0;
	char *fileName = (char *)malloc(100 * sizeof(char));
	while (1){
		printf(">");
		fgets(givenInput, 100, stdin);
		_strlwr(givenInput);
		char *command = (char *)malloc(100 * sizeof(char));
		command = truncate(&givenInput);
		if (strcmp(command, "set") == 0){
			setCommand(&givenInput, excelMatrix);
		}
		else if (strcmp(command, "get") == 0){
			getCommand(&givenInput, excelMatrix);
		}
		else if (strcmp(command, "print") == 0){
			printCommand(excelMatrix, rowId, colId);
		}
		else if (strcmp(command, "export") == 0){
			exportCommand(givenInput, excelMatrix);
		}
		else if (strcmp(command, "import") == 0){
			flag = 1;
			strcpy(fileName, givenInput);
			importCommand(givenInput, excelMatrix);
		}
		else if (strcmp(command, "save") == 0){
			if (flag != 1) printf("Invalid\n");
			else exportCommand(fileName, excelMatrix);
		}
	}
}