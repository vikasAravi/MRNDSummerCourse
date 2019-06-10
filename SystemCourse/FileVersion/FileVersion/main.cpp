#include "utilities.h"
#include "fileFunc.h"



int main(void){
	char *givenInput = (char *)malloc(100 * sizeof(char));
	while (1){
		printf(">");
		gets(givenInput);
		char *command = (char *)malloc(100 * sizeof(char));
		strcpy(command, truncate(&givenInput));
		if (strcmp(command, "LISTOFFILES") == 0){
			listFiles();
		}
		else if (strcmp(command, "COPYTODISK") == 0){
			char *sourceFile = (char*)malloc(100 * sizeof(char));
			strcpy(sourceFile, truncate(&givenInput));
			char *destFile = (char *)malloc(100 * sizeof(char));
			strcpy(destFile, truncate(&givenInput));
			copyToDisk(sourceFile, destFile);
		}
		else if (strcmp(command, "COPYFROMDISK") == 0){
			char *sourceFile = (char*)malloc(100 * sizeof(char));
			strcpy(sourceFile, truncate(&givenInput));
			char *destFile = (char *)malloc(100 * sizeof(char));
			strcpy(destFile, truncate(&givenInput));
			copyFromDisk(sourceFile, destFile);
		}
		else if (strcmp(command, "FORMAT") == 0){
			format();
		}
		else if (strcmp(command, "DELETEFILE") == 0){
			char *sourceFile = (char*)malloc(100 * sizeof(char));
			strcpy(sourceFile, truncate(&givenInput));
			deleteFile(sourceFile);
		}
		else {
			printf("Invalid Command\n");
		}
		printf("\n");
	}
}