/* Andrew Morrison
 * armorrison
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

typedef unsigned long address;

/* Lines are part of sets
*/
struct Line
{
	int valid;
	address tag;
	int age;
};

/* Sets are parts of a Cache
*/
struct Set
{
	struct Line* lines;
};

/* The full Cache
*/
struct Cache
{
	struct Set* sets;
};

typedef struct Line Line;
typedef struct Set Set;
typedef struct Cache Cache;

/* Initializer for the cache
*/
Cache initCache (long numLines, long numSets)
{
	// Necessary local variables, edit cache and return it
	Cache cache;
	Set set;
	Line line;
	line.valid = 0;
	line.tag = 0;
	line.age = 0;
	
	cache.sets = (Set*) malloc (sizeof(Set) * numSets); // dynamically allocate memory to the cache

	/* Dynamically allocate memory for each Set
	*/
	for(int i=0; i<numSets; i++)
	{
		set.lines = (Line*) malloc (sizeof(Line) * numLines); // dynamically allocate memory to the Lines
		
		/* set all lines to default values
		*/
		for(int j=0; j<numLines; j++)
		{
			set.lines[j] = line; // fill the set with default lines
		}

		cache.sets[i] = set; // fill the cache with default sets
	}

	/* Prints out the cache for referencial purposes
	*/
	for(int i = 0; i<numSets; i++)
	{
		for(int j=0; j<numLines; j++)
		{
			printf("valid: %d, age: %d\n", cache.sets[i].lines[j].valid, cache.sets[i].lines[j].age);
		}
	}
	return cache;
}

/* accesData will perform all actions on the cache
*/
void accessData(Cache cache, char instruction, address mem, int s, int b, int E)
{
    /* Get the tag from the memory address */
	int tagSize = 64-s-b;
	

}

/* main method
*/
int main(int argc, int* argv)
{
	
	if (argc > 4)
		return 0;

    printSummary(0, 0, 0);
    return 0;
    char c;

    int s;
    int E;
    int b;
    char* trace;

    if(argc > 0)
    {
    	while(c = getopt(argc, argv, "s:E:b:t:") != -1)
    	{
    		switch (c)
    		{
    			case 's':
    				s = atoi(optarg);
    			case 'E':
    				E = atoi(optarg);
    			case 'b':
    				b = atoi(optarg);
    			case 't':
    				t = optarg;
    		}
    	}
    }

    Cache myCache = initCache(E, S);

    char next;
    address mem;
    int size;

    if(trace != NULL)
    {
    	while(fscanf(trace, "%c, %x, %d", next, mem, size) = 3)
    	{
    		next = trace[i];
    		switch (next)
    		{
    			case 'I':
    				break;
    			case 'L':
    				accesData(myCache, next, mem, s, b, E);
    				break;
    			case 'S':
    				accessData(myCache, next, mem, s, b, E);
    				break;
    			case 'M':
    				accessData(myCache, next, mem, s, b, E);
    				break;
    		}
    	}
    }
}


