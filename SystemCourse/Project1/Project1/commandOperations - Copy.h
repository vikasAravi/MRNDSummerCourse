#include "utilities.h"

struct Cell{
	int value;
	char *formulae;
};


char *truncate(char **);
void setCommand(char **, Cell **);
void getCommand(char **, Cell **);
void printCommand(Cell **, int , int);
void exportCommand(char *, Cell **);
void importCommand(char *, Cell **);
int isValue(char *);
int isChar(char);
int precedence(char);
int applyOp(int, int, char);
int calculate(Cell**, char *);
int evaluate(Cell **, int, int);
char *getString(char *);
