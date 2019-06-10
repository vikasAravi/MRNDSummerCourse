#include "utilities.h"
#include "commandOperations.h"

char *truncate(char **givenInput){
	if (givenInput == NULL) return NULL;
	while (**givenInput == ' '){
		(*givenInput)++;
	}
	char *result = (char *)malloc(100 * sizeof(char));
	int index = 0;
	while (**givenInput != ' ' && **givenInput != '\n'){
		if (**givenInput == '='){
			(*givenInput)++;
			result[index] = '\0';
			return result;
		}
		result[index++] = **givenInput;
		(*givenInput)++;
	}
	result[index] = '\0'; 
	return result;
}

int isChar(char res){
	if (res >= 'a' && res <= 'z') return 1;
	return 0;
}

int precedence(char op){
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == '/')
		return 2;
	return 0;
}


int isValue(char *passedArg){
	int startIndex = 0;
	while (passedArg[startIndex] != '\0' && passedArg[startIndex] != '\n'){
		if (passedArg[startIndex] < '0' || passedArg[startIndex] > '9') return 0;
		startIndex += 1;
	}
	return 1;
}

char *getString(char *givenString){
	int count = 0;
	char *result = (char *)malloc(100 * sizeof(char));
	for (int i = 0; givenString[i] != '\0' && givenString[i] != '\n'; i++){
		if (givenString[i] != ' ' &&  givenString[i] != '='){
			result[count++] = givenString[i];
		}
	}
	result[count] = '\0';
	return result;
}

int applyOp(int a, int b, char op){
	switch (op){
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	}
	return 0;
}

int calculate(Cell **sheetMatrix, char *formulae){
		int i;

		std::stack <int> values; 
		std::stack <char> ops;

		for (i = 0; i < formulae[i] != '\0' && formulae[i] != '\n'; i++){
 
			if (formulae[i] == '('){
				ops.push(formulae[i]);
			}

			else if (isdigit(formulae[i])){
				int val = 0;

				while (formulae[i] != '\0' && isdigit(formulae[i]))
				{
					val = (val * 10) + (formulae[i] - '0');
					i++;
				}
				i--;
				values.push(val);
			}

			else if (isChar(formulae[i]) == 1){
				int pos = 0;
				char *operand = (char *)malloc(100 * sizeof(char));
				operand[pos] = formulae[i];
				i += 1;
				pos += 1;
				while (isChar(formulae[i]) || isdigit(formulae[i])){
					operand[pos++] = formulae[i];
					i += 1;
				}
				operand[pos] = '\0';
				i -= 1;
				int colId = operand[0] - 'a';
				int rowId = operand[1] - '0' - 1;
				values.push(evaluate(sheetMatrix, rowId, colId));
			}
			else if (formulae[i] == ')')
			{
				while (!ops.empty() && ops.top() != '(')
				{
					int val2 = values.top();
					values.pop();

					int val1 = values.top();
					values.pop();

					char op = ops.top();
					ops.pop();

					values.push(applyOp(val1, val2, op));
				}
				ops.pop();
			}

			else
			{
				while (!ops.empty() && precedence(ops.top()) >= precedence(formulae[i])){
					int val2 = values.top();
					values.pop();

					int val1 = values.top();
					values.pop();

					char op = ops.top();
					ops.pop();

					values.push(applyOp(val1, val2, op));
				}

				ops.push(formulae[i]);
			}
		}

		while (!ops.empty()){
			int val2 = values.top();
			values.pop();

			int val1 = values.top();
			values.pop();

			char op = ops.top();
			ops.pop();

			values.push(applyOp(val1, val2, op));
		}
		return values.top();
}



int evaluate(Cell ** cellMatrix, int rowId, int colId){
	if (cellMatrix[rowId][colId].value != -1){
		return cellMatrix[rowId][colId].value;
	}
	return calculate(cellMatrix, cellMatrix[rowId][colId].formulae);

}


void setCommand(char **givenInput, Cell** excelMatrix){
	char *index = (char *)malloc(100 * sizeof(char));
	index = truncate(givenInput);
	int colId = index[0] - 'a';
	int rowId = (index[1] - '0') - 1;
	char *value = (char *)malloc(100 * sizeof(char));
	value = getString(*givenInput);
	printf("%s\n", value);
	if (isValue(value) == 1){
		printf("Hello\n");
		excelMatrix[rowId][colId].value = std::atoi(value);
		excelMatrix[rowId][colId].formulae = "";
	}
	else{
		excelMatrix[rowId][colId].value = -1;
		excelMatrix[rowId][colId].formulae = value;
	}

}

void getCommand(char **givenInput, Cell** excelMatrix){

	//Todo cycle Detection
	char *index = (char *)malloc(100 * sizeof(char));
	index = truncate(givenInput);
	printf("%s\n", index);
	int rowId = (index[1] - '0') - 1;
	int colId = index[0] - 'a';
	printf("%d %d\n", rowId, colId);
	printf("%d\n", evaluate(excelMatrix, rowId, colId));
}


void printCommand(Cell** excelMatrix, int rows, int cols){

	//Todo CycleDetection in calculate Func
	int startRow, startCol;
	for (startRow = 0; startRow < rows; startRow++){
		for (startCol = 0; startCol < cols; startCol++){

			if (excelMatrix[startRow][startCol].value != -1){
				printf("%5d ", excelMatrix[startRow][startCol].value);
			}
			else{
				printf("%5d ", evaluate(excelMatrix, startRow, startCol));
			}
		}
		printf("\n");
	}
	printf("\n");
}


void exportCommand(char *givenInput, Cell** givenSheet){

	givenInput[strlen(givenInput) - 1] = '\0';
	givenInput = getString(givenInput);
	strcat(givenInput, ".csv");
	printf("%s\n", givenInput);
	FILE *fp = fopen(givenInput, "w+");
	if (fp == NULL) printf("Access Denied\n");
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			int val = givenSheet[i][j].value;
			if (val != -1){
				fprintf(fp, "%d,", givenSheet[i][j].value);
			}
			else{
				fprintf(fp, "%s,", givenSheet[i][j].formulae);
			}
		}
		fprintf(fp , "\n");
	}
	fclose(fp);
}

void importCommand(char *givenInput, Cell** givenSheet){
	givenInput[strlen(givenInput) - 1] = '\0';
	givenInput = getString(givenInput);
	strcat(givenInput, ".csv");
	printf("%s\n", givenInput);
	FILE * fp = fopen(givenInput, "r+");
	if (fp == NULL) printf("Access Denied");
	if (fp == NULL) return;
	int c;
	int rowId = 0;
	int colId = 0;
	int pos = 0;
	char* word = (char *)malloc(100 * sizeof(word));
	for (c = getc(fp); c != EOF; c = getc(fp)){
		if (c == ','){
			word[pos] = '\0';
			printf("%s ", word);
			if (isValue(word) == 1){
				givenSheet[rowId][colId].value = std::atoi(word);
				givenSheet[rowId][colId].formulae = "";
				word = (char *)malloc(100 * sizeof(word));
			}
			else{
				givenSheet[rowId][colId].value = -1;
				givenSheet[rowId][colId].formulae = word;
				word = (char *)malloc(100 * sizeof(word));
			}
			colId += 1;
			pos = 0;
		}
		else if (c == '\n'){
			rowId += 1;
			colId = 0;
			word = (char *)malloc(100 * sizeof(word));
		}
		else{
			word[pos] = c;
			pos += 1;
		}
	}
	printf("\n");
	fclose(fp); 
}


