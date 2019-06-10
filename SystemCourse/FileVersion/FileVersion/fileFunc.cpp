#include "utilities.h"
#include "fileFunc.h"


char *truncate(char **givenInput){
	if (givenInput == NULL) return NULL;
	while (**givenInput == ' '){
		(*givenInput)++;
	}
	char *result = (char *)malloc(100 * sizeof(char));
	int index = 0;
	while (**givenInput != ' ' && **givenInput != '\n' && **givenInput != '\0'){
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


int isFreeBlocksAvailable(int start, int end, struct metaData meta){
	for (int i = start; i < end; i++){
		if (meta.bitVector[i] != 1) return 0;
	}
	return 1;
}

 


void readBlock(int blockId, void *sourceAdd){
	FILE *fp = NULL;
	fp = fopen("hardDisk.hdd", "r+");
	if (fp == NULL){
		printf("No hdd file\n");
		return;
	}
	fseek(fp, blockId * 16384, SEEK_SET);
	fread(sourceAdd, 16384, 1, fp);
	fclose(fp);
}



void writeBlock(int blockId, void *sourceAdd){
	FILE* fp;
	fp = fopen("hardDisk.hdd", "r+");
	fseek(fp, blockId * 16384, SEEK_SET);
	fwrite(sourceAdd, 16384, 1, fp);
	fclose(fp);
}




void listFiles(){
	char data[16384];
	readBlock(0, data);
	struct metaData meta;
	memcpy(&meta, data, sizeof(meta));
	for (int i = 0; i < meta.fileCount; i++){
		printf("%s\n", meta.files[i].fileName);
	}
}



void copyToDisk(char *sourceFile, char *destFile){
	char data[16384];
	FILE *fp;
	fp = fopen(sourceFile, "r+");
	if (fp == NULL){
		printf("No such filename Exists\n");
		return;
	}
	struct metaData meta;
	readBlock(0, data);
	memcpy(&meta, data, sizeof(metaData));
	int availableSpace = (meta.avilableFreeSpace) * 16384;
	printf("%d\n", availableSpace);
	fseek(fp, 0, SEEK_END);
	int sizeOfFile = ftell(fp);
	if (sizeOfFile > availableSpace){
		printf("No Space Availabe\n");
		return;
	}
	printf("Debug Point1\n");
	int noOfBlocks = ceil(sizeOfFile / 16384.0);
	printf("%d\n", noOfBlocks);
	for (int i = 400; i < 6400; i++){
		if (meta.bitVector[i] == 1){
			int num = isFreeBlocksAvailable(i, noOfBlocks, meta);
			if (num == 1){
				strcpy(meta.files[meta.fileCount].fileName, destFile);
				meta.files[meta.fileCount].offset = i;
				meta.files[meta.fileCount].length = sizeOfFile;
				meta.files[meta.fileCount].blockCount = noOfBlocks;
				meta.fileCount += 1;
				meta.avilableFreeSpace -= noOfBlocks;
				for (int j = i; j < i + noOfBlocks; i++){
					meta.bitVector[j] = 0; 
				}
				memcpy(data, &meta, sizeof(meta));
				writeBlock(0, data);
				for (int k  = 0; k < (noOfBlocks - 1); k++){
					fread(data, 16384, 1, fp);
					writeBlock(i, data);
					i += 1;
				}
				int remainingBytes = sizeOfFile - ((noOfBlocks - 1) * 16384);
				fread(data, remainingBytes, 1, fp);
				writeBlock(i, data);
				fclose(fp);
				return;
			}
		}
	}
}




void copyFromDisk(char *sourceFile, char *destFile){
	FILE *fp;
	fp = fopen(destFile, "w+");
	if (fp == NULL){
		printf("No such filename Exists\n");
		return;
	}
	const int BUFFERSIZE = 16384;
	char BUFFER[BUFFERSIZE];
	struct metaData meta;
	readBlock(0, BUFFER);
	memcpy(&meta, BUFFER, sizeof(metaData));
	for (int i = 0; i < meta.fileCount; i++){
		if (strcmp(meta.files[i].fileName, sourceFile) == 0){
			int offset = meta.files[i].offset;
			int length = meta.files[i].length;
			int j = 0;
			for (j = offset; j < (offset + length - 1); j++){
				readBlock(j, BUFFER);
				fwrite(BUFFER, sizeof(BUFFER), 1, fp);
			}
			readBlock(j, BUFFER);
			int remainingBytes = length - (((meta.files[i].blockCount) - 1) * BUFFERSIZE);
			fwrite(BUFFER, remainingBytes, 1, fp);
			fclose(fp);
			return;
		}
	}
	printf("Source Files not Found\n");
}




void deleteFile(char *sourceFile){
	struct metaData meta;
	char data[16384];
	readBlock(0, data);
	memcpy(&meta, data, sizeof(metaData));
	for (int i = 0; i < (meta.fileCount); i++){
		if (strcmp(meta.files[i].fileName, sourceFile) == 0){
			int value = meta.files[i].offset;
			for (int k = value; k < (value + meta.files[i].length); k++){
				meta.bitVector[k] = 1;
			}
			meta.avilableFreeSpace += meta.files[i].blockCount;
			strcpy(meta.files[i].fileName, meta.files[meta.fileCount - 1].fileName);
			meta.files[i].offset = meta.files[meta.fileCount - 1].offset;
			meta.files[i].length = meta.files[meta.fileCount - 1].length;
			meta.files[i].blockCount = meta.files[meta.fileCount - 1].blockCount;
			return;
		}
	}
	printf("No file to delete\n");
}



void format(){
	struct metaData meta;
	char data[16384];
	readBlock(0, data);
	memcpy(&meta, data, sizeof(meta));
	meta.fileCount = 0;
	meta.avilableFreeSpace = 6000;
	for (int i = 0; i < 6400; i++){
		meta.bitVector[i] = 1;
	}
	meta.magicNumber = 0x444e524d;
	memcpy(data, &meta, sizeof(metaData));
	writeBlock(0, data);
}