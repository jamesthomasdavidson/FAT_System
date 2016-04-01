
/* Constants.h*/

#ifndef __CONSTANTS__H
#define __CONSTANTS__H

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <fcntl.h> 
#include <time.h> 
#include <sys/mman.h>
#include <readline/readline.h>
#include <readline/history.h>

#define bool uint32_t
#define TRUE 1
#define FALSE 0

#define COPY_SUCCESS   0x01
#define COPY_FINISHED  0x02
#define COPY_FAILURE   0x04

#define FS_IDENTIFIER					"CSC360FS"
#define DEFAULT_BLOCK_SIZE 				512

#define DIRECTORY_ENTRY_SIZE			64
#define DIRECTORY_ENTRY_PER_BLOCK		(DEFAULT_BLOCK_SIZE/DIRECTORY_ENTRY_SIZE)

#define DIRECTORY_ENTRY_FREE			0x00
#define DIRECTORY_ENTRY_USED			0x01
#define DIRECTORY_ENTRY_FILE			0x02
#define DIRECTORY_ENTRY_DIRECTORY		0x04

#define DIRECTORY_STATUS_OFFSET			0
#define DIRECTORY_START_BLOCK_OFFSET	1
#define DIRECTORY_BLOCK_COUNT_OFFSET	5
#define DIRECTORY_FILE_SIZE_OFFSET		9
#define DIRECTORY_CREATE_OFFSET			13
#define DIRECTORY_MODIFY_OFFSET			20
#define DIRECTORY_FILENAME_OFFSET		27

#define DIRECTORY_STATUS_SIZE			1
#define DIRECTORY_START_BLOCK_SIZE		4
#define DIRECTORY_BLOCK_COUNT_SIZE		4
#define DIRECTORY_FILE_SIZE_SIZE		4
#define DIRECTORY_CREATE_SIZE			7
#define DIRECTORY_MODIFY_SIZE			7
#define DIRECTORY_FILENAME_SIZE			31

#define MAX_INPUT_SIZE 					256
#define MAX_INPUT_PARAMETERS 			5
#define DIRECTORY_MAX_NAME_LENGTH		30

#define FAT_ENTRY_SIZE			4
#define FAT_ENTRY_PER_BLOCK		(DEFAULT_BLOCK_SIZE/FAT_ENTRY_SIZE)
#define	FAT_FREE				0x00000000
#define FAT_RESERVED			0x00000001
#define FAT_EOF					0xFFFFFFFF

#define IDENT_OFFSET			0
#define BLOCKSIZE_OFFSET		8
#define BLOCKCOUNT_OFFSET		10
#define FATSTART_OFFSET			14
#define FATBLOCKS_OFFSET		18
#define ROOTDIRSTART_OFFSET		22
#define ROOTDIRBLOCKS_OFFSET	26

#define IDENT_VAR_SIZE			8
#define BLOCKSIZE_VAR_SIZE		2
#define BLOCKCOUNT_VAR_SIZE		4
#define FATSTART_VAR_SIZE		4
#define FATBLOCKS_VAR_SIZE		4
#define ROOTDIRSTART_VAR_SIZE	4
#define ROOTDIRBLOCKS_VAR_SIZE	4


typedef struct FileAllocationTable{
    int available;
    int reserved;
    int allocated;
}FileAllocationTable;

typedef struct SuperBlock{
    char *fileSystemIdentifier;
    uint16_t blockSize;	
    uint32_t fileSystemSize; /* In blocks */
    uint32_t FATStartBlock;
    uint32_t numBlocksInFAT;
    uint32_t rootStartBlock;
    uint32_t numBlocksInRoot;
    char* memMap;
}SuperBlock;

typedef struct Time{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}Time;

typedef struct DirectoryEntry{
	struct DirectoryEntry *next;
	struct DirectoryList* DirectoryList;
	bool isUsed;
	bool isFile;
	bool isDirectory;
    uint8_t status;
    uint32_t startBlock;
    uint32_t numBlocks;
    uint32_t entrySize;
    Time *createTime; 
    Time *modTime;
    char* entryAddress;
    char* entryName;
    char* directoryAddress;
}DirectoryEntry;

typedef struct DirectoryList{
	struct DirectoryList *next;
	struct DirectoryList *parentDirectory;
	DirectoryEntry* head;
	DirectoryEntry* tail;
	int size;
	int numEntries;
	char* name;
	char* directoryAddress;
}DirectoryList;

typedef struct DirectoryListQueue{
	DirectoryList *head;
	DirectoryList *tail;
	int size;
}DirectoryListQueue;

typedef struct DirectoryTree{
	DirectoryList* root;
	DirectoryList* currentDirectory;
	char* directoryPath;
}DirectoryTree;




/* COMMON FILE FUNCTIONALITY FUNCTIONS */
/* SECTION 1___________________________*/ 

/* 1.1 */
char* getFSI(char* memMap);
/* 1.2 */
char* getRootDirectory(SuperBlock *SB);
/* 1.3 */
char* getDirectoryAddress(SuperBlock *SB, DirectoryEntry* entry);
/* 1.4 */
char* getFileName(char* CD);
/* 1.5 */
Time* getTimeModInfo(DirectoryEntry* entry);
/* 1.6 */
Time* getTimeCreateInfo(DirectoryEntry* entry);
/* 1.7 */
uint32_t getDirectorySize(SuperBlock *SB, char* CD);
/* 1.8 */
uint32_t getRootDirectorySize(SuperBlock *SB, char* CD);
/* 1.9 */
uint32_t getFileInfo(char * memMap, int offset, int length);
/* 1.10 */
void writeIntToFile(char* memMap, int offset, uint32_t data);
/* 1.11 */
void writeByteToFile(char* memMap, char byte);
/* 1.12 */
void writeDirectoryEntryToIMG(DirectoryEntry* entry);
/* 1.13 */
Time* writeTimeToFile(char* CD, int offset);
/* 1.14 */
void writeFileNameToFile(char *CD, int offset, char* name);
/* 1.15 */
void writeTwoBytesToFile(char* memMap, int offset, uint16_t data);


/* FILE COPY FUNCTIONS */
/*_SECTION 2___________*/

/* 2.1 */
bool copyBlockToBuffer(SuperBlock *SB, char* CD, FILE* writeFile, char* buffer, uint32_t* returnBlock, uint32_t* copySize);
/* 2.2 */
void copyBufferToBlock(SuperBlock *SB, char* writeAddress, char* buffer, uint32_t bytesRead);
/* 2.3 */
void copyFileToLocal(SuperBlock *SB, DirectoryTree* tree, char* fileToCopy, char* destinationDirectory);
/* 2.4 */
void copyFileToIMG(SuperBlock *SB, FileAllocationTable* FAT, DirectoryTree* tree, char* fileToCopy, char* destinationDirectory);



/* SUPERBLOCK & FAT FUNCTIONS */
/*_SECTION 3__________________*/


/* 3.1 */
uint32_t getSuperBlockInfo(char *memMap, int offset, int length);
/* 3.2 */
uint32_t getFreeBlock(SuperBlock* SB);
/* 3.3 */
uint32_t getFirstBlock(SuperBlock *SB, char* CD);
/* 3.4 */
uint32_t getNextBlock(SuperBlock *SB, char* CD);
/* 3.5 */
SuperBlock* initializeSuperBlock(char* memMap);
/* 3.6 */
FileAllocationTable* initializeFileAllocationTable(SuperBlock *SB);



/* PRINTING FUNCTIONS */
/*_SECTION 4__________*/

/* 4.1 */
void printSuperBlockInfo(SuperBlock *SB);
/* 4.2 */
void printFileAllocationTableInfo(FileAllocationTable *FAT);
/* 4.3 */
Time* getTimeModInfo(DirectoryEntry* entry);
/* 4.4 */
Time* getTimeCreateInfo(DirectoryEntry* entry);
/* 4.5 */
char status(DirectoryEntry* entry);
/* 4.6 */
void printDirectoryInfo(SuperBlock *SB, DirectoryList* list);
/* 4.7 */
void printDirectoryEntryInfo(DirectoryEntry* entry);



/* DIRECTORY FUNCTIONS */
/*_SECTION 5___________*/

/* 5.1 */
DirectoryEntry* cycleDirectoryList(DirectoryList* list);
/* 5.2 */
DirectoryEntry* getDirectoryEntryByName(DirectoryList* list, char* entryName);
/* 5.3 */
DirectoryEntry* insertDirectoryEntry(DirectoryList *list, DirectoryEntry* dir);
/* 5.4 */
DirectoryList* buildDirectoryList(SuperBlock* SB, DirectoryList* parentDirectory, char* CD, char* directoryName);
/* 5.5 */
DirectoryList* buildRootDirectoryList(SuperBlock* SB);
/* 5.6 */
void insertQueue(DirectoryListQueue* queue, DirectoryList* list);
/* 5.7 */
DirectoryList* removeQueue(DirectoryListQueue* queue);
/* 5.8 */
DirectoryTree* initalizeDirectoryTree(SuperBlock* SB);
/* 5.9 */
DirectoryEntry* returnFreeDirectoryEntry(DirectoryList *list);
/* 5.10 */
DirectoryTree* changeDirectories(DirectoryTree *tree, char* destinationDirectory);
/* 5.11 */
DirectoryEntry* getDirectoryEntry(char* CD);
/* 5.12 */
DirectoryList* initializeDirectoryList(SuperBlock* SB, char* CD, DirectoryList* parentDirectory, char* directoryName);



/* RESOURCE DEALLOCATION FUNCTIONS */
/*_SECTION 6_______________________*/

/* 6.1 */
void freeSuperBlock(SuperBlock *SB);
/* 6.2 */
void freeFileAllocationTable(FileAllocationTable *FAT);
/* 6.3 */
void freeDirectoryEntryMembers(DirectoryEntry* entry);
/* 6.4 */
void freeDirectoryEntryStruct(DirectoryEntry* entry);
/* 6.5 */
void freeDirectoryListEntries(DirectoryList* list);
/* 6.6 */
void freeDirectoryListStruct(DirectoryList* list);





/*------------------------------------------------------------------------------------------------*/





/* COMMON FILE FUNCTIONALITY FUNCTIONS */
/* SECTION 1___________________________*/ 

/* 1.1 */
char* getFSI(char* memMap)
{
	int i;
	char * FSI = malloc(sizeof(char)*8);
	for(i = 0; i < 8; i++){
		*(FSI+i) = *(memMap + i);
	}
	return FSI;
}
/* 1.2 */
char* getRootDirectory(SuperBlock *SB)
{
	char* root = SB->memMap + (SB->rootStartBlock)*(SB->blockSize);
	return root;
}
/* 1.3 */
char* getDirectoryAddress(SuperBlock *SB, DirectoryEntry* entry)
{
	char* directoryAddress = SB->memMap + (entry->startBlock)*(SB->blockSize);
	return directoryAddress;
}
/* 1.4 */
char* getFileName(char* CD)
{
	char* name = malloc(sizeof(char)*DIRECTORY_MAX_NAME_LENGTH);
	char* namePointer = (char*)(CD + DIRECTORY_FILENAME_OFFSET);
	int i;

	for(i = 0; namePointer[i] != 0 && i < DIRECTORY_MAX_NAME_LENGTH; i++)
	{
		name[i] = namePointer[i];
	}
	name[i] = 0;
	return name;
}
/* 1.5 */
Time* getTimeModInfo(DirectoryEntry* entry)
{
	Time *modTime   = malloc(sizeof(Time));
	modTime->year    = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET, 2);
	modTime->month   = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET + 2, 1);
	modTime->day     = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET + 3, 1);
	modTime->hour    = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET + 4, 1);
	modTime->minute  = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET + 5, 1);
	modTime->second  = getFileInfo(entry->entryAddress, DIRECTORY_MODIFY_OFFSET + 6, 1);
	return modTime;
}
/* 1.6 */
Time* getTimeCreateInfo(DirectoryEntry* entry)
{
	Time *createTime   = malloc(sizeof(Time));
	createTime->year    = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET, 2);
	createTime->month   = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET + 2, 1);
	createTime->day     = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET + 3, 1);
	createTime->hour    = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET + 4, 1);
	createTime->minute  = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET + 5, 1);
	createTime->second  = getFileInfo(entry->entryAddress, DIRECTORY_CREATE_OFFSET + 6, 1);
	return createTime;
}
/* 1.7 */
uint32_t getDirectorySize(SuperBlock *SB, char* CD)
{
	//
	return (getFileInfo(CD, DIRECTORY_BLOCK_COUNT_OFFSET, DIRECTORY_BLOCK_COUNT_SIZE))*(SB->blockSize);
}
/* 1.8 */
uint32_t getRootDirectorySize(SuperBlock *SB, char* CD)
{
	//
	return (SB->numBlocksInRoot)*(SB->blockSize);
}
/* 1.9 */
uint32_t getFileInfo(char * memMap, int offset, int length)
{
    int i; uint32_t retVal = 0;

    char *tmp = malloc(sizeof(char) * length);
    memset(tmp, 0, length);
    memcpy(tmp, memMap+offset, length);

    for(i=0; i < length; i++)
    {
        retVal += (((int)tmp[i]&0xff) << (8*(length - i - 1)));
    }
    free(tmp);
    return retVal;
}
/* 1.10 */
void writeIntToFile(char* memMap, int offset, uint32_t data)
{
    int i;
    int length = 4;
    char byte;
    for(i=0; i < length; i++)
    {
    	byte = (data) >> (8*(length - i - 1));
        writeByteToFile((memMap + offset + i), byte);
        memcpy((memMap + offset + i), &byte, sizeof(char));
    }
}
/* 1.11 */
void writeByteToFile(char* memMap, char byte)
{
	//
	memcpy(memMap, &byte, sizeof(char));
}
/* 1.12 */
void writeDirectoryEntryToIMG(DirectoryEntry* entry)
{
	writeByteToFile(entry->entryAddress + DIRECTORY_STATUS_OFFSET, entry->status);
	writeIntToFile(entry->entryAddress, DIRECTORY_START_BLOCK_OFFSET, entry->startBlock);
	writeIntToFile(entry->entryAddress, DIRECTORY_BLOCK_COUNT_OFFSET, entry->numBlocks);
	writeIntToFile(entry->entryAddress, DIRECTORY_FILE_SIZE_OFFSET,   entry->entrySize);
	entry->createTime = writeTimeToFile(entry->entryAddress, DIRECTORY_CREATE_OFFSET);
	entry->modTime = writeTimeToFile(entry->entryAddress, DIRECTORY_MODIFY_OFFSET);
	writeFileNameToFile(entry->entryAddress, DIRECTORY_FILENAME_OFFSET, entry->entryName);
}
/* 1.13 */
Time* writeTimeToFile(char* CD, int offset)
{
	Time* writeTime = malloc(sizeof(Time));
	if(!writeTime){
		printf("writeTime failed\n");
	}

	time_t t = time(0);
	struct tm * currentTime = localtime(&t);

	writeTime->year   = currentTime->tm_year + 1990;
	writeTime->month  = currentTime->tm_mon + 1;
	writeTime->day    = currentTime->tm_mday;
	writeTime->hour   = currentTime->tm_hour;
	writeTime->minute = currentTime->tm_min;
	writeTime->second = currentTime->tm_sec;

	printf("here1 %d\n", currentTime->tm_mday);

	writeTwoBytesToFile(CD, offset,  writeTime->year);
	writeByteToFile(CD + offset + 2, writeTime->month);
	writeByteToFile(CD + offset + 3, writeTime->day);
	writeByteToFile(CD + offset + 4, writeTime->hour);
	writeByteToFile(CD + offset + 5, writeTime->minute);
	writeByteToFile(CD + offset + 6, writeTime->second);
	printf("here2 %d\n", writeTime->minute);
	return writeTime;
}
/* 1.14 */
void writeFileNameToFile(char *CD, int offset, char* name)
{
	int length = strlen(name);
	int i;
	char byte;
	for(i = 0; i < length; i++)
	{
		byte = *(name + i);
		writeByteToFile(CD + offset + i, byte);
	}
	writeByteToFile(CD + offset + i, 0);
}
/* 1.15 */
void writeTwoBytesToFile(char* memMap, int offset, uint16_t data)
{
    int i;
    int length = 2;
    char byte;
    for(i=0; i < length; i++)
    {
    	byte = (data) >> (8*(length - i - 1));
        writeByteToFile((memMap + offset + i), byte);
        memcpy((memMap + offset + i), &byte, sizeof(char));
    }
}





/* FILE COPY FUNCTIONS */
/*_SECTION 2___________*/

/* 2.1 */
bool copyBlockToBuffer(SuperBlock *SB, char* CD, FILE* writeFile, char* buffer, uint32_t* returnBlock, uint32_t* copySize)
{
	memset(buffer, 0, SB->blockSize);
	int i;

	uint32_t nextBlock = getNextBlock(SB, CD);
	*returnBlock = nextBlock;

	if(nextBlock == FAT_EOF || *copySize == 0){
		return COPY_FINISHED;
	}else{
		for(i = 0; i < SB->blockSize && i < *copySize; i++)
		{
			*(buffer+i) = *((SB->memMap) + (SB->blockSize)*(nextBlock) + i);
		}
	}

	int n;
	*copySize = *copySize - i;

	if((n = fwrite(buffer, sizeof(char), i, writeFile)) == i)
	{
		return COPY_SUCCESS;
	}else{
		return COPY_FAILURE;
	}
}
/* 2.2 */
void copyBufferToBlock(SuperBlock *SB, char* writeAddress, char* buffer, uint32_t bytesRead)
{
	int i;
	for(i = 0; i < bytesRead; i++)
	{
		writeByteToFile(writeAddress + i, buffer[i]);
	}
}
/* 2.3 */
void copyFileToLocal(SuperBlock *SB, DirectoryTree* tree, char* fileToCopy, char* destinationDirectory)
{
	changeDirectories(tree, destinationDirectory);

	DirectoryEntry* entry = getDirectoryEntryByName(tree->currentDirectory, fileToCopy);

	if(entry)
	{
		FILE* file = fopen(entry->entryName, "w");
		if(!file)
		{
			printf("fopen failed: 2.2\n");
			return;
		}
		char* buffer = malloc(sizeof(char)*(SB->blockSize));

		uint32_t copySize = entry->entrySize;
		uint32_t failBlock;
		uint32_t COPY_STATUS;

		while(((COPY_STATUS = copyBlockToBuffer(SB, entry->entryAddress, file, buffer, &failBlock, &copySize)) & COPY_SUCCESS) == COPY_SUCCESS);

		if(COPY_STATUS == COPY_FAILURE){
			printf("Copying Block %08x FAILED", failBlock);
		}

		fclose(file);
		free(buffer);
	}else{
		printf("Copytolocal: File not found\n");
	}
}
/* 2.4 */
void copyFileToIMG(SuperBlock *SB, FileAllocationTable* FAT, DirectoryTree* tree, char* fileToCopy, char* destinationDirectory)
{
	
	changeDirectories(tree, destinationDirectory);

	DirectoryEntry* entry = returnFreeDirectoryEntry(tree->root);


	entry->status = (DIRECTORY_ENTRY_FILE | DIRECTORY_ENTRY_USED);
	entry->isFile = TRUE;
	entry->isUsed = TRUE;
	entry->entryName = malloc(sizeof(char)*(strlen(fileToCopy))+1);
	strcpy(entry->entryName, fileToCopy);
	entry->startBlock = (getFreeBlock(SB)-((SB->blockSize)*(SB->FATStartBlock)))/FAT_ENTRY_SIZE;
	entry->directoryAddress = getDirectoryAddress(SB, entry);

	FILE* file;
	if(!(file = fopen(fileToCopy, "r"))){
		printf("fopen failed \n");
	}

	char* buffer = malloc(sizeof(char)*(SB->blockSize));
	uint32_t bytesRead;

	entry->startBlock = getFreeBlock(SB);
	uint32_t FATEntry = entry->startBlock;

	uint32_t blockOffset;
	uint32_t previousBlockOffset = 0;

	FAT->allocated = FAT->allocated + 1;
	FAT->available = FAT->available - 1;

	while((bytesRead = fread(buffer, sizeof(char), SB->blockSize, file)) > 0)
	{
		if(previousBlockOffset){
			FATEntry = getFreeBlock(SB);
			FAT->reserved = FAT->reserved + 1;
			FAT->available = FAT->available - 1;
		}

		writeIntToFile(SB->memMap, FATEntry, FAT_RESERVED);

		blockOffset = (SB->blockSize)*(SB->FATStartBlock) + FATEntry*FAT_ENTRY_SIZE;

		copyBufferToBlock(SB, SB->memMap + (FATEntry)*(SB->blockSize), buffer, bytesRead);

		writeIntToFile(SB->memMap, blockOffset, FAT_EOF);

		if(previousBlockOffset){
			writeIntToFile(SB->memMap, previousBlockOffset, FATEntry);
		}

		previousBlockOffset = blockOffset;

		entry->entrySize = entry->entrySize + bytesRead;
		entry->numBlocks = entry->numBlocks + 1;

	}
	writeDirectoryEntryToIMG(entry);
}






/* SUPERBLOCK & FAT FUNCTIONS */
/*_SECTION 3__________________*/

/* 3.1 */
uint32_t getSuperBlockInfo(char *memMap, int offset, int length)
{
	uint32_t info = getFileInfo(memMap, offset, length);
    return info;
}
/* 3.2 */
uint32_t getFreeBlock(SuperBlock* SB)
{
	int FATStart = (SB->FATStartBlock)*(SB->blockSize);
	int FATEnd = FATStart + (SB->numBlocksInFAT)*(SB->blockSize);
	int FATEntry;
	uint32_t block;
	int offset;


	for(offset = FATStart; offset < FATEnd; offset = offset + FAT_ENTRY_SIZE)
	{
		block = getFileInfo(SB->memMap, offset, FAT_ENTRY_SIZE);
		if(block == FAT_FREE)
		{
			return offset;
		}
	}
	return 0;
}
/* 3.3 */
uint32_t getFirstBlock(SuperBlock *SB, char* CD)
{
	uint32_t firstBlock = getFileInfo(CD, DIRECTORY_START_BLOCK_OFFSET, FAT_ENTRY_SIZE);
	return firstBlock;
}
/* 3.4 */
uint32_t getNextBlock(SuperBlock *SB, char* CD)
{
	static uint32_t returnBlock = 0;
	if(!returnBlock)
	{
		returnBlock = getFirstBlock(SB, CD);
	}
	else
	{
		returnBlock = (getFileInfo(SB->memMap,SB->blockSize*SB->FATStartBlock + returnBlock*FAT_ENTRY_SIZE ,FAT_ENTRY_SIZE));
	}
	return returnBlock;	
}
/* 3.5 */
SuperBlock* initializeSuperBlock(char* memMap)
{
	SuperBlock* SB = malloc(sizeof(SuperBlock));
    SB->fileSystemIdentifier = getFSI(memMap);
    SB->blockSize 			 = getSuperBlockInfo(memMap,BLOCKSIZE_OFFSET,    BLOCKSIZE_VAR_SIZE);
    SB->fileSystemSize 		 = getSuperBlockInfo(memMap,BLOCKCOUNT_OFFSET,   BLOCKCOUNT_VAR_SIZE);
    SB->FATStartBlock 	     = getSuperBlockInfo(memMap,FATSTART_OFFSET,     FATSTART_VAR_SIZE);
    SB->numBlocksInFAT 		 = getSuperBlockInfo(memMap,FATBLOCKS_OFFSET,    FATBLOCKS_VAR_SIZE);
    SB->rootStartBlock 		 = getSuperBlockInfo(memMap,ROOTDIRSTART_OFFSET, ROOTDIRSTART_VAR_SIZE);
    SB->numBlocksInRoot 	 = getSuperBlockInfo(memMap,ROOTDIRBLOCKS_OFFSET,ROOTDIRSTART_VAR_SIZE);
    SB->memMap 				 = memMap;
    return SB;
}
/* 3.6 */
FileAllocationTable* initializeFileAllocationTable(SuperBlock *SB)
{

	FileAllocationTable* FAT = malloc(sizeof(FileAllocationTable));
	char* m = SB->memMap;

	int available = 0;
	int allocated = 0;
	int reserved = 0;
	
    int memAddr = ((SB->FATStartBlock)*(SB->blockSize));
    int memOffset = (SB->numBlocksInFAT)*(SB->blockSize);

	char *tmp = malloc(sizeof(char) * memOffset);
	memcpy(tmp, m + memAddr, memOffset);

	int i;

	for(i = 0; i < memOffset; i = i + FAT_ENTRY_SIZE)
	{
		int j;
		int val = 0;
		int byte = FAT_ENTRY_SIZE - 1;

        for(j=i; j < i + FAT_ENTRY_SIZE; j++)
        {
	        val += ((int)tmp[j]<<(8*byte));
			byte--;
        }

		if(val == 0){
			available++;
		}else if(val == 1){
			reserved++;
		}else{
			allocated++;
		}
	}
	FAT->available = available;
	FAT->allocated = allocated;
	FAT->reserved = reserved;
	free(tmp);
	return FAT;
}





/* PRINTING FUNCTIONS */
/*_SECTION 4__________*/

/* 4.1 */
void printSuperBlockInfo(SuperBlock *SB)
{
	printf("Super Block Information:\n");
	printf("Block Size: %d\n",SB->blockSize);
	printf("Block Count %d\n",SB->fileSystemSize);
	printf("FAT starts: %d\n",SB->FATStartBlock);
	printf("FAT blocks: %d\n",SB->numBlocksInFAT);
	printf("Root directory start: %d\n",SB->rootStartBlock);
	printf("Root directory blocks: %d\n\n",SB->numBlocksInRoot);
}
/* 4.2 */
void printFileAllocationTableInfo(FileAllocationTable *FAT)
{
	printf("FAT information:\n");
	printf("Free blocks: %d\n",FAT->available);
	printf("Reserved blocks: %d\n",FAT->reserved);
	printf("Allocated blocks: %d\n",FAT->allocated);
}
/* 4.3 */
char status(DirectoryEntry* entry)
{
	if((entry->status >> 1) == 1){
		return 'F';
	}else{
		return 'D';
	}
}
/* 4.4 */
void printDirectoryInfo(SuperBlock *SB, DirectoryList* list)
{
	int i = 0;
	DirectoryEntry *entry;
	while((entry = cycleDirectoryList(list)))
	{
		if((entry->isFile || entry->isDirectory) && (entry->isUsed))
		{
			printDirectoryEntryInfo(entry);
		}
    }
}
/* 4.5 */
void printDirectoryEntryInfo(DirectoryEntry* entry)
{
	printf("%c ",    status(entry));
	printf("%10d ",  entry->entrySize);
	printf("%30s ",  entry->entryName);
	if(entry->modTime){
		printf("%d/%d/%d %02d:%02d:%02d\n", entry->modTime->year, entry->modTime->month, entry->modTime->day, entry->modTime->hour, entry->modTime->minute, entry->modTime->second);
	}
}





/* DIRECTORY FUNCTIONS */
/*_SECTION 5___________*/

/* 5.1 */
DirectoryEntry* cycleDirectoryList(DirectoryList* list)
{
	static DirectoryEntry *current = NULL;
	if(current == NULL){
		current = list->head;
	}else if(current->next == NULL){
		current = NULL;
	}else{
		current = current->next;
	}
	return current;
}
/* 5.2 */
DirectoryEntry* insertDirectoryEntry(DirectoryList *list, DirectoryEntry* entry)
{

	/* List is empty */
	if(list->numEntries == 0){
		list->head = entry;
		list->tail = entry;
	}else{
		list->tail->next = entry;;
		list->tail = entry;
	}
	list->numEntries = list->numEntries + 1;
	list->tail->next = NULL;
	return entry;
}
/* 5.3 */
DirectoryEntry* getDirectoryEntryByName(DirectoryList* list, char* entryName)
{
	DirectoryEntry* entry;
	DirectoryEntry* returnEntry = NULL;
	int i = 0;
	while((entry = cycleDirectoryList(list)))
	{
		if(!strcmp(entry->entryName, entryName)){
			returnEntry = entry;
		}
	}
	return returnEntry;
}
/* 5.4 */
DirectoryEntry* returnFreeDirectoryEntry(DirectoryList *list)
{
	DirectoryEntry *returnEntry = NULL;
	DirectoryEntry* entry;

	while((entry = cycleDirectoryList(list)))
	{
		if(!entry->isUsed && !entry->isFile)
		{
			returnEntry = entry;
		}
	}
	return returnEntry;
}
/* 5.5 */
DirectoryEntry* getDirectoryEntry(char* CD)
{
	DirectoryEntry *dir = malloc(sizeof(DirectoryEntry));

	dir->entryAddress  = CD;
	dir->status        = getFileInfo(dir->entryAddress, DIRECTORY_STATUS_OFFSET,      DIRECTORY_STATUS_SIZE);

	dir->isUsed = ((dir->status & DIRECTORY_ENTRY_USED) == DIRECTORY_ENTRY_USED ? TRUE : FALSE);
	dir->isFile = ((dir->status & DIRECTORY_ENTRY_FILE) == DIRECTORY_ENTRY_FILE ? TRUE : FALSE);
	dir->isDirectory = ((dir->status & DIRECTORY_ENTRY_DIRECTORY) == DIRECTORY_ENTRY_DIRECTORY ? TRUE : FALSE);

	if(!dir->isUsed)
	{
		dir->numBlocks     = 0;
		dir->startBlock    = 0;
		dir->entrySize     = 0;
	}else{
		dir->numBlocks     = getFileInfo(dir->entryAddress, DIRECTORY_BLOCK_COUNT_OFFSET, DIRECTORY_BLOCK_COUNT_SIZE);
		dir->startBlock    = getFileInfo(dir->entryAddress, DIRECTORY_START_BLOCK_OFFSET, DIRECTORY_START_BLOCK_SIZE);
		dir->entrySize     = getFileInfo(dir->entryAddress, DIRECTORY_FILE_SIZE_OFFSET,   DIRECTORY_FILE_SIZE_SIZE);\
	}
	dir->createTime    = getTimeCreateInfo(dir);
	dir->modTime 	   = getTimeModInfo(dir);
	dir->entryName     = getFileName(dir->entryAddress);
	dir->directoryAddress = NULL;

	return dir;
}
/* 5.6 */
DirectoryList* initializeDirectoryList(SuperBlock* SB, char* CD, DirectoryList* parentDirectory, char* directoryName)
{
	DirectoryList *list = malloc(sizeof(DirectoryList));
	list->directoryAddress = CD;
	list->name = malloc(sizeof(directoryName) + 1);
	strcpy(list->name, directoryName);
	list->size = ((!parentDirectory) ? getRootDirectorySize(SB, CD) : getDirectorySize(SB, CD));
	list->numEntries = 0;
	list->parentDirectory = parentDirectory; 
	return list;
}
/* 5.7 */
DirectoryList* buildDirectoryList(SuperBlock* SB, DirectoryList* parentDirectory, char* CD, char* directoryName)
{
	DirectoryList *list = initializeDirectoryList(SB, CD, parentDirectory, directoryName);

	int i;

	for(i = 0; i < list->size; i = i + DIRECTORY_ENTRY_SIZE)
	{
		DirectoryEntry *dir = getDirectoryEntry(CD + i);
		insertDirectoryEntry(list, dir);
    }
    return list;
}
/* 5.8 */
DirectoryList* buildRootDirectoryList(SuperBlock* SB)
{
	char* RT = getRootDirectory(SB);
	DirectoryList* root = buildDirectoryList(SB, NULL, RT, "root");
	return root;
}
/* 5.9 */
void insertQueue(DirectoryListQueue* queue, DirectoryList* list)
{
	/* List is empty */
	if(queue->size == 0){
		queue->head = list;
		queue->tail = list;
	}else{
		queue->tail->next = list;
		queue->tail = list;
	}
	queue->size = queue->size + 1;
	queue->tail->next = NULL;
}
/* 5.10 */
DirectoryList* removeQueue(DirectoryListQueue* queue)
{
	DirectoryList* returnList;
	if(queue->size == 0){
		return NULL;
	}else if(queue->size == 1){
		returnList = queue->head;
		queue->head = NULL;
		queue->tail = NULL;
	}else{
		returnList = queue->head;
		queue->head = queue->head->next;;
		returnList->next = NULL;
	}
	queue->size = queue->size - 1;
	return returnList;
}
/* 5.11 */
DirectoryTree* changeDirectories(DirectoryTree *tree, char* destinationDirectory)
{
	//
	return tree;
}
/* 5.12 */
DirectoryTree* initializeDirectoryTree(SuperBlock* SB)
{
	DirectoryTree* tree = malloc(sizeof(tree));

	tree->root = buildRootDirectoryList(SB);
	tree->currentDirectory = tree->root;

	tree->directoryPath = malloc(sizeof(char)*DIRECTORY_MAX_NAME_LENGTH);
	strcpy(tree->directoryPath, tree->root->name);

	DirectoryList* list;
	DirectoryEntry* entry;
	DirectoryListQueue queue;
	queue.size = 0;

	int q = 0;
	insertQueue(&queue, tree->root);
 	while(queue.size != 0)
 	{
 		list = removeQueue(&queue);
 		while((entry = cycleDirectoryList(list))){
 			if(entry->isDirectory && entry->isUsed)
 			{
 				DirectoryList* insertList = buildDirectoryList(SB, list, entry->directoryAddress, entry->entryName);
 				insertQueue(&queue, insertList);

 			}
		}
 	}
 	return tree;
}





/* RESOURCE DEALLOCATION FUNCTIONS */
/*_SECTION 6_______________________*/

/* 6.1 */
void freeSuperBlock(SuperBlock *SB)
{
	free(SB->fileSystemIdentifier);
	free(SB);
}
/* 6.2 */
void freeFileAllocationTable(FileAllocationTable *FAT)
{
	//
	free(FAT);
}
/* 6.3 */
void freeDirectoryEntryMembers(DirectoryEntry* entry)
{
	free(entry->createTime);
	free(entry->modTime);
	free(entry->entryName);
}
/* 6.4 */
void freeDirectoryEntryStruct(DirectoryEntry* entry)
{
	//
	free(entry);
}
/* 6.5 */
void freeDirectoryListEntries(DirectoryList* list)
{
	DirectoryEntry* entry;
	DirectoryEntry* previous = NULL;
	while((entry = cycleDirectoryList(list)))
	{
		if(!previous){
			freeDirectoryEntryStruct(previous);
		}
		freeDirectoryEntryMembers(entry);
		previous = entry;
	}
	freeDirectoryEntryStruct(previous);
}
/* 6.6 */
void freeDirectoryListStruct(DirectoryList* list)
{
	free(list->name);
	free(list);
}

#endif