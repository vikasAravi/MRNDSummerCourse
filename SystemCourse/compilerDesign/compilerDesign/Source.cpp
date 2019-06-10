#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stack>
using namespace std;
int getVariableVal(char *op,int syTabIdx, struct SymbolTable *symbolTable) {
	int addr = 0;
	for (int i = 0; i < syTabIdx; i++) {
		if (symbolTable[i].symbol == op[0]) {
			addr = symbolTable[i].address;
			break;
		}
	}
	if (op[1] != '\0' && op[1] == '[')
		addr += (op[2] - 48);
	return addr;
}


int getOpCode(char *opCodes[], char *operation) {
	for (int i = 0; i < 14; i++) {
		//printf("%s\n", opCodes[i]);
		if (!strcmp(operation, opCodes[i]))
			return i+1;
	}
	return 0;
}

void strreplace(char *op) {
	int i = 0;
	while (op[i] != '\0') {
		if (op[i] == ',')
			op[i] = ' ';
		i++;
	}
}


void tokenInstruction(char *line, int idx, struct IntermediateTable *interTable,char *opCodes[],char *regCodes[],int syTabIdx,struct SymbolTable *symbolTable) {
	char operation[10];
	sscanf(line, "%s %*s", operation);
	int len = strlen(operation);
	if (operation[len - 1] != ':') {
		interTable[idx].InNo = idx + 1;
		char operands[20];
		sscanf(line, "%*s %[^\n]%", operands);
		int len = strlen(operands);
		strreplace(operands);
		int i = 0;
		char *op = strtok(operands, " ");
		if (!strcmp(operation, "MOV")) {
			if (op[1] != '\0')
				interTable[idx].opCode = getOpCode(opCodes, "MOVMR");
			else
				interTable[idx].opCode = getOpCode(opCodes, "MOVRM");
		}
		else
			interTable[idx].opCode = getOpCode(opCodes, operation);
		while (op != NULL) {
			//printf("%s\n", op);
			if (op[1] != '\0' && op[1] == 'X') {
				interTable[idx].a[i] = op[0] - 65;
				//printf("%d\t%d\n", i, op[0] - 65);
				i++;
			}
			else  {
				int val = getVariableVal(op, syTabIdx, symbolTable);
				interTable[idx].a[i] = val;
				//printf("%d\t%d\n", i, val);
				i++;
			}
			op = strtok(NULL, " ");
		}
		for (int j = i; j < 5; j++) {
			interTable[idx].a[j] = -2;
		}
	}
}




int tokenDeclaration(char *line,int address,int idx,struct SymbolTable *symbolTable) {
	char operation[10];
	sscanf(line, "%s %*s", operation);
	char variable[5];
	sscanf(line, "%*s %s", variable);
	if (strlen(variable) > 1)
		symbolTable[idx].size = variable[2] - 48;
	else if (!strcmp(operation, "CONST"))
		symbolTable[idx].size = 0;
	else
		symbolTable[idx].size = 1;

	symbolTable[idx].address = address;
	symbolTable[idx].symbol = variable[0];
	return address + symbolTable[idx].size;
}

void processLabel(char *label, struct LabelTable *labelTable, int idx,int address) {
	strcpy(labelTable[idx].labelName,label);
	labelTable[idx].blockAddress = address;
}

void processIF(char *line,struct IntermediateTable *interTable,int idx,char *opCodes[],struct SymbolTable *symbolTable,int syTabIdx){
	interTable[idx].InNo = idx + 1;
	//printf("%s\n", line);
	interTable[idx].opCode = getOpCode(opCodes, "IF");
	char s[3][10];
	char operand1[3];
	sscanf(line, "%*s %s %*s %*s %*s", operand1);
	strcpy(s[0], operand1);
	char operand2[3];
	sscanf(line, "%*s %*s %*s %s %*s", operand2);
	strcpy(s[1], operand2);
	char operatr[10];
	sscanf(line, "%*s %*s %s %*s %*s", operatr);
	strcpy(s[2], operatr);
	int i = 0;
	while (i < 3) {
		if (s[i][1] != '\0' && s[i][1] == 'X') {
			interTable[idx].a[i] = s[i][0] - 65;
		}
		else  {
			int val = getVariableVal(s[i], syTabIdx, symbolTable);
			interTable[idx].a[i] = val;
		}
		i++;
	}
	interTable[idx].a[i] = -1;
	for (int j = i+1; j < 5; j++)
		interTable[idx].a[j] = -2;
}

void processELSE(struct IntermediateTable *interTable,int inTabIdx) {
	interTable[inTabIdx].InNo = inTabIdx + 1;
	interTable[inTabIdx].opCode = 6;
	interTable[inTabIdx].a[0] = -1;
	for (int j = 1; j < 5; j++)
		interTable[inTabIdx].a[j] = -2;
}

void processENDIF(stack<int> s, struct IntermediateTable *interTable, int nextIns) {
	int idx = s.top();
	s.pop();
	if (interTable[idx].opCode == 6) {
		int elsenxtins = idx + 1;
		interTable[idx].a[0] = nextIns + 1;
		idx = s.top();
		//printf("%d\n", idx);
		s.pop();
		for (int j = 0; j < 5; j++) {
			if (interTable[idx].a[j] == -1) {
				interTable[idx].a[j] = elsenxtins;
				break;
			}
		}
	}
	else {
		interTable[idx].a[0] = nextIns + 1;
	}
}

void inputModule(FILE *src,char *opCodes[],char *regCodes[]) {
	char line[20];
	int start = 0;
	int address = 0;
	stack<int> stack;
	struct SymbolTable *symbolTable = (struct SymbolTable *)malloc(sizeof(struct SymbolTable) * 10);
	struct IntermediateTable *interTable = (struct IntermediateTable *)malloc(sizeof(struct IntermediateTable) * 20);
	struct LabelTable *labelTable = (struct LabelTable *)malloc(sizeof(struct LabelTable) * 20);
	int syTabIdx = 0;
	int inTabIdx = 0;
	int labTabIdx = 0;
	while (fscanf(src, "%[^\n]%", line) == 1) {
		char operation[10];
		sscanf(line, "%s %*s", operation);
		int len = strlen(operation);
		if (!strcmp(operation, "START:")) {
			continue;
		}
		else if ( !strcmp(operation,"DATA") || !strcmp(operation,"CONST")) {
			address = tokenDeclaration(line, address, syTabIdx, symbolTable);
			syTabIdx++;
		}
		else if (operation[len - 1] == ':') {
			processLabel(operation, labelTable, labTabIdx,inTabIdx+1);
			labTabIdx++;
		}
		else if (!strcmp(operation, "IF")){
			processIF(line,interTable,inTabIdx,opCodes,symbolTable,syTabIdx);
			stack.push(inTabIdx);
			inTabIdx++;
		}
		else if (!strcmp(operation, "ELSE")) {
			processELSE(interTable, inTabIdx);
			stack.push(inTabIdx);
			inTabIdx++;
			processLabel(operation, labelTable, labTabIdx, inTabIdx + 1);
			labTabIdx++;
		}
		else if ( !strcmp(operation,"ENDIF") ) {
			processENDIF(stack, interTable, inTabIdx + 1);
			processLabel(operation, labelTable, labTabIdx, inTabIdx + 1);
			labTabIdx++;
		}
		else {
			//printf("%s\n", operation);
			tokenInstruction(line, inTabIdx, interTable,opCodes,regCodes,syTabIdx,symbolTable);
			inTabIdx++;
		}
		
	}
	printf("Symbol Table\n");
	for (int i = 0; i < syTabIdx; i++) {
		printf("%c\t%d\t%d\n", symbolTable[i].symbol, symbolTable[i].address, symbolTable[i].size);
	}
	printf("Intermediate Table\n");
	for (int i = 0; i < inTabIdx; i++) {
		printf("%d\t%d\t", interTable[i].InNo, interTable[i].opCode);
		for (int j = 0; j < 5; j++) {
			if (interTable[i].a[j] != -2)
				printf("%d\t", interTable[i].a[j]);
		}
		printf("\n");
	}
	printf("LableTable\n");
	for (int i = 0; i < labTabIdx; i++) {
		printf("%s\t%d\n", labelTable[i].labelName, labelTable[i].blockAddress);
	}
	FILE *fp = fopen("output.txt", "w");
	fwrite("InNo", 1, 4, fp);
	fwrite("\t", 1, sizeof("\t"), fp);
	fwrite("OpCode", 1, 6, fp);
	fwrite("\t", 1, sizeof("\t"), fp);
	fwrite("Parameters", 1, 10, fp);
	fwrite("\n", 1, sizeof("\n"), fp);
	for (int i = 0; i < inTabIdx; i++) {
		fprintf(fp,"%d",interTable[i].InNo);
		fprintf(fp, "%s", "\t");
		fprintf(fp, "%d", interTable[i].opCode);
		fprintf(fp, "%s", "\t");
		for (int j = 0; j < 5; j++) {
			if (interTable[i].a[j] != -2) {
				fprintf(fp, "%d", interTable[i].a[j]);
				fprintf(fp, "%s", "\t");
			}
		}
		fprintf(fp,"%s","\n");
	}
	fclose(fp);
}


int main() {
	FILE *src = fopen("input.txt", "r");
	char *opCodes[] = { "MOVRM" , "MOVMR" ,"ADD", "SUB" ,"MUL" ,"JMP","IF","EQ", "LT","GT","LTEQ","GTEQ","PRINT","READ" };
	char *regCodes[] = { "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX" };
	inputModule(src,opCodes,regCodes);
	fclose(src);
	return 0;
}