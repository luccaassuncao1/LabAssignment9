#include <stdio.h>

// RecordType
struct RecordType
{
	int	id;
	char name;
	int	order; 
    struct RecordType *next;
};

// Fill out this structure
struct HashType
{
    int size;
    struct RecordType **data;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
    int index = hash(record->id, tableSize);
    // if the RecordType at that index is NULL
    if (hashTable->data[index] == NULL)
    {
        // set 'record' equal to the HashType pointer in the table at index
        hashTable->data[index] = record;
        // allocate memory for the 'next' pointer
        record->next = NULL;
    }
    else // there's already a RecordType at that index
    {
        // traverse to the end of the linked list and add 'record' to the end of it
        struct RecordType *current = hashTable->data[index];
        while (current->next != NULL && current != NULL)
        {
            current = current->next;
        }
        current->next = record;
        record->next = NULL;
    }
}


// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i, j;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray->data[i] != NULL)
        {
            printf("Index %d ->", i);

            struct RecordType *current = pHashArray->data[i];
            while (current != NULL)
            {
                printf(" %d, %c, %d ->", current->id, current->name, current->order);
                current = current->next;
            }
            printf(" NULL");
            printf("\n");
        }
        else
            printf("Index %d -> NULL\n", i);
    }
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    int hashTableSize = 10;
    struct HashType *hashTable = (struct HashType*) calloc(1, sizeof(struct HashType));
    hashTable->size = hashTableSize;
    hashTable->data = (struct RecordType**) calloc(hashTableSize, sizeof(struct RecordType*));
    for (int i = 0; i < recordSz; ++i)
    {
        struct RecordType *record = (struct RecordType*) malloc(sizeof(struct RecordType));
        record->id = pRecords[i].id;
        record->name = pRecords[i].name;
        record->order = pRecords[i].order;
        record->next = NULL;
        insertRecord(hashTable, record, hashTableSize);
    }
    displayRecordsInHash(hashTable, hashTableSize);

    // free all the allocated memory
    for (int i = 0; i < hashTableSize; ++i)
    {
        struct RecordType* current = hashTable->data[i];
        while (current != NULL)
        {
            struct RecordType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->data);
    free(hashTable);
    free(pRecords);

    return 0;
}
