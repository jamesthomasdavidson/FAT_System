/* JAMES T. DAVIDSON | V00812527 | jamesdav@uvic.ca */

#include "Constants.h"

#define IMAGEFILE  "img1/disk1.img" 

/* Superblock, FAT, Current Directory, Root */

SuperBlock *SB;	
FileAllocationTable *FAT;
DirectoryTree* TREE;
char *CD;

char* displayShell()
{
    char t[MAX_INPUT_SIZE];
    sprintf(t, "shell: > ");
    return readline(t);    
}

void formatArgs(char** args, char* buffer)
{
    char* buffer_pointer;
    int i = 0;

    for(buffer_pointer = strtok(buffer, " \n"); buffer_pointer; buffer_pointer = strtok(NULL, " \n"))
    {

        if(!strcmp(buffer_pointer, "copytolocal")){
            args[0] = malloc(sizeof(char)*15+1);
            strcpy(args[0], "copytolocal");
            i = 1;
        }else if(!strcmp(buffer_pointer, "copytoimg")){
            args[0] = malloc(sizeof("copytoimg*")*13+1);
            strcpy(args[0], "copytoimg");
            i = 1;
        }else{
            args[i] = malloc(sizeof(char)*strlen(buffer_pointer)+1);
            strcpy(args[i++], buffer_pointer);
        }
    }
    args[i] = NULL;
}

void freeArgs(char** args)
{
    int i = 0;
    while(args[i])
    {
        free(args[i++]);
    }
}

bool runCommands(char** args)
{
    char* fileToCopy = args[1];
    char* destinationDirectory = args[2];

    if(!strcmp(args[0], "list")){
        printDirectoryInfo(SB, TREE->currentDirectory);
    }else if(!strcmp(args[0], "info")){
        printSuperBlockInfo(SB);
        printFileAllocationTableInfo(FAT);
    }else if(!strcmp(args[0], "copytolocal")){
        if(!fileToCopy || !destinationDirectory){ printf("Invalid arguements\n"); return TRUE;}
        copyFileToLocal(SB, TREE, fileToCopy, destinationDirectory /* On IMG file */);
    }else if(!strcmp(args[0], "copytoimg")){
        if(!fileToCopy || !destinationDirectory){ printf("Invalid arguements\n"); return TRUE; }
        copyFileToIMG(SB, FAT, TREE, fileToCopy, "" /* On local machine */);
    }else if(!strcmp(args[0], "quit")){
        freeArgs(args);
        return FALSE;
    }else{
        printf("Command not recognized\n");
    }
    freeArgs(args);
    return TRUE;
}

int main(int argc, char *argv[])
{

	int fileDescriptor;
	struct stat st;
	char* memMap;
    char* textFile;

	/* Open the .img file for reading */
	if((fileDescriptor = open(((argc == 1) ? IMAGEFILE : argv[1]), O_RDWR)) == -1)
	{
		if(argc >= 2){ printf("file \"%s\" not found, exiting program\n", argv[1]); }
		else{ printf("no file specified, tried opening %s with failure\n", IMAGEFILE); }
		return EXIT_FAILURE;
	}

    if(fstat(fileDescriptor, &st)) 
    { 
        printf("fstat error: [%s]\n",strerror(errno)); 
        close(fileDescriptor); 
        return EXIT_FAILURE; 
    } 

    memMap = mmap(NULL,st.st_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fileDescriptor, 0);
    SB = initializeSuperBlock(memMap);
    FAT = initializeFileAllocationTable(SB);
    TREE = initializeDirectoryTree(SB);

    bool run = TRUE;
    char **args;

    while(run){
        char *buffer = displayShell();
        if(buffer[0] == '\0'){
            free(buffer);
            continue;
        }else{
            args = malloc(sizeof(char*)*MAX_INPUT_PARAMETERS);
            formatArgs(args, buffer);
            run = runCommands(args);
        }
        free(buffer);
    }
    
    free(args);
    freeDirectoryListEntries(TREE->root);
    freeDirectoryListStruct(TREE->root);
    freeSuperBlock(SB);
    freeFileAllocationTable(FAT);

	return 0;
}