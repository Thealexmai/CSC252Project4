#include <math.h>
#include "cache.h"
#include "trace.h"


int write_xactions = 0;
int read_xactions = 0;

/*
  Print help message to user
*/
void printHelp(const char * prog) {
	printf("%s [-h] | [-s <size>] [-w <ways>] [-l <line>] [-t <trace>]\n", prog);
	printf("options:\n");
	printf("-h: print out help text\n");
	printf("-s <cache size>: set the total size of the cache in KB\n");
	printf("-w <ways>: set the number of ways in each set\n");
	printf("-l <line size>: set the size of each cache line in bytes\n");
	printf("-t <trace>: use <trace> as the input file for memory traces\n");
  /* EXTRA CREDIT
  printf("-lru: use LRU replacement policy instead of FIFO\n");
  */
}
/*
	Main function. Feed to options to set the cache
	
	Options:
	-h : print out help message
	-s : set L1 cache Size (KB)
	-w : set L1 cache ways
	-l : set L1 cache line size
*/
int main(int argc, char* argv[])
{
	int i,j;
	uint32_t size = 32; //total size of L1$ (KB)
	uint32_t ways = 1; //# of ways in L1. Default to direct-mapped
	uint32_t line = 32; //line size (B)
    uint32_t numLines = 0;
    uint32_t indexValue;
    uint32_t tagValue;

  //for FileInput Output formats
    FILE *fp, *wp;
    char c; //store character and read from file - for counting numLines
    char storeLoad;
    uint32_t effectiveAddr;
    char writeFileName[200];
    
  // hit and miss counts
  int totalHits = 0;
  int totalMisses = 0;

  char * filename;

	//strings to compare
	const char helpString[] = "-h";
	const char sizeString[] = "-s";
	const char waysString[] = "-w";
	const char lineString[] = "-l";
	const char traceString[] = "-t";
    const char lruString[] = "-lru";
	
  if (argc == 1) {
    // No arguments passed, show help
    printHelp(argv[0]);
    return 1;
  }
  
	//parse command line
	for(i = 1; i < argc; i++)
	{
        
		//check for help
		if(!strcmp(helpString, argv[i]))
		{
			//print out help text and terminate
            printHelp(argv[0]);
			return 1; //return 1 for help termination
		}
		//check for size
		else if(!strcmp(sizeString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				size = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of size value\n");
				return -1; //input failure
			}
		}
		//check for ways
		else if(!strcmp(waysString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				ways = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of ways value\n");
				return -1; //input failure
			}
		}
		//check for line size
		else if(!strcmp(lineString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				line = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of line size value\n");
				return -1; //input failure
			}
		}
        else if (!strcmp(traceString, argv[i])) {
            filename = argv[++i];
        }
        else if (!strcmp(lruString, argv[i])) {
            // Extra Credit: Implement Me!
			printf("Unrecognized argument. Exiting.\n");
			return -1;
        }
		//unrecognized input
		else{
			printf("Unrecognized argument. Exiting.\n");
			return -1;
		}
        
	} //end for
	
    
    
    //calculate variables
    uint32_t sets = (size * 1024) / (line * ways);
    uint32_t bitsOffset = 0;
    uint32_t bitsIndex = 0;
    
    uint32_t copyLine = line;
    uint32_t copySets = sets;
    while (copyLine >>= 1) {
        bitsOffset++;
    }
    while (copySets >>= 1) {
        bitsIndex++;
    }
    uint32_t bitsTag = 32 - (bitsIndex + bitsOffset);
    
    
  /* TODO: Probably should intitalize the cache */
    //init array stuff
    //calloc(n length, size)
    int **validArray = calloc(sets, sizeof(int *) * sets);
    int **tagArray = calloc(sets, sizeof(int *) * sets);
    int **dirtyArray = calloc(sets, sizeof(int *) * sets);
    
    initArray(validArray, sets, ways);
    initArray(tagArray, sets, ways);
    initArray(dirtyArray, sets, ways);

//Used to test if init worked
//    i=0;
//    int j=0;
//    int count = 0;
//    
//    for(i=0; i<sets; i++)
//        for(j=0;j<ways; j++)
//            validArray[i][j] = ++count;
//    
//    for(i=0; i< sets; i++)
//        for(j=0;j<ways; j++)
//            printf("array[%i][%i] = %d\n ", i, j, validArray[i][j]);
    
    printf("Ways: %u; Sets: %u; Line Size: %uB\n", ways, sets, line);
    printf("Tag: %d bits; Index: %d bits; Offset: %d bits\n", bitsTag, bitsIndex, bitsOffset);

    
/* TODO: Now we read the trace file line by line */

    //get writeFileName
    sprintf(writeFileName, "%s.simulated", filename);
    //printf("Value of writeFileName: %s\n", writeFileName);
    
    fp = fopen(filename, "r");
    wp = fopen(writeFileName, "w"); //file to write, write file

    if (fp == NULL) {
        printf("Couldn't open file %s\n", filename);
        return 0;
    }
    
    
    //Extract number of lines in file
    for(c = getc(fp); c != EOF; c = getc(fp)) {
        //keep track of the number of lines
        if(c == '\n') {
            numLines++;
        }
    }
    printf("File %s has %d lines\n", filename, numLines);

    
    //rewind to top
    rewind(fp);
    
    //Test values of tag[]
    //printArray(tagArray, sets, ways);

    
/* TODO: Now we simulate the cache */
    
    
    for(i=0; i<numLines; i++) {
        
        fscanf(fp, "%c ", &storeLoad);
        fscanf(fp, "%x\n", &effectiveAddr);
        //printf("%i. %c, %x\n", i+1, storeLoad, effectiveAddr);
        
        //get index and tag values
        indexValue = getIndexValue(effectiveAddr, bitsTag, bitsIndex, bitsOffset);
        tagValue = getTagValue(effectiveAddr, bitsTag);
//        printf("%i tagValue = %i\n", i+1, tagValue);
        
        //testing writing to file
        fprintf(wp, "%c 0x%.8x\n", storeLoad, effectiveAddr);
        
        
        
    } //end for
    
    //close the files writing
    fclose(fp);
    fclose(wp);

  /* Print results */
  printf("Miss Rate: %8lf%%\n", ((double) totalMisses) / ((double) totalMisses + (double) totalHits) * 100.0);
  printf("Read Transactions: %d\n", read_xactions);
  printf("Write Transactions: %d\n", write_xactions);

  /* TODO: Now we output the file */

  /* TODO: Cleanup */
}

void initArray(int **array, uint32_t sets, uint32_t ways) {
    int j;
    for(j=0; j<sets; j++) {
        array[j] = calloc(ways, sizeof(int) * ways);
    }
}

uint32_t getIndexValue(uint32_t effectiveAddr, uint32_t bitsTag, uint32_t bitsIndex, uint32_t bitsOffset) {
    
    if((bitsOffset + bitsTag) == 32) {
        return 0;
    } else {
        uint32_t value = effectiveAddr;
        value <<= bitsTag;
        value >>= (bitsTag + bitsOffset);
        
        return value;

    }
}

uint32_t getTagValue(uint32_t effectiveAddr, uint32_t bitsTag) {
    uint32_t shiftamount = 32-bitsTag;
    uint32_t value1 = effectiveAddr;
    value1 >>= (32-bitsTag);

    return value1;
}

void printArray(int ** array, uint32_t sets, uint32_t ways) {
    int i,j;
    for(i=0; i< sets; i++)
        for(j=0;j<ways; j++)
            printf("ArrayName[%i][%i] = %d\n ", i, j, array[i][j]);
}


