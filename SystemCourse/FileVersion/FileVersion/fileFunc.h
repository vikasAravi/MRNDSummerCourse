#include "utilities.h"


struct FileData{
	char fileName[20];
	unsigned int offset;
	unsigned int blockCount;
	unsigned int length;
};

struct metaData{
	int magicNumber;
	struct FileData files[32];
	unsigned int fileCount;
	int avilableFreeSpace;
	char bitVector[6400];
};

void listFiles();
void copyToDisk(char *, char *);
void copyFromDisk(char *, char *);
void format();
void deleteFile(char *);
void readBlock(int, void *);
void writeBlock(int, void *);
int isFreeBlocksAvailable(int, int, struct metaData);
char *truncate(char **);

