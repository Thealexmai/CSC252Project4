#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

extern int write_xactions;
extern int read_xactions;

void printHelp(const char * prog);
void initArray(int **array, uint32_t sets, uint32_t ways);
uint32_t getIndexValue(uint32_t effectiveAddr, uint32_t bitsTag, uint32_t bitsIndex, uint32_t bitsOffset);
