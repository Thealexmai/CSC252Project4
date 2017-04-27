#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef enum {
    FALSE, TRUE
}boolean;

typedef enum {
    HIT, COMPULSORY, CONFLICT, CAPACITY
}fullyMiss_t;

extern int write_xactions;
extern int read_xactions;

void printHelp(const char * prog);
void initArray(int **array, uint32_t sets, uint32_t ways);
uint32_t getIndexValue(uint32_t effectiveAddr, uint32_t bitsTag, uint32_t bitsIndex, uint32_t bitsOffset);
uint32_t getTagValue(uint32_t effectiveAddr, uint32_t bitsTag);
boolean duplicateinFully(int currentLine, uint32_t effectiveAddr, uint32_t fullyTagValue, uint32_t * traceAddress, uint32_t bitsOffset);
void printArray(int ** array, uint32_t sets, uint32_t ways);
void simRealWorld(char storeLoad, uint32_t effectiveAddr, uint32_t indexValue, uint32_t tagValue, uint32_t ways, int ** validArray, int ** tagArray, int ** dirtyArray, boolean fullyDuplicate, fullyMiss_t toReal, int *totalHits, int *totalMisses);
