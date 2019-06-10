struct SymbolTable {
	char symbol;
	unsigned int address;
	unsigned int size;
};

struct IntermediateTable {
	unsigned int InNo;
	unsigned int opCode;
	unsigned int a[5];
};

struct LabelTable {
	char labelName[20];
	int blockAddress;
};