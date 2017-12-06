/* Andrew Morrison
 * armorrison
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

typedef unsigned long long int address;

/* holds the info for the cache 
*/
struct cacheInfo
{
    int s;
    int b;
    int E;
    int S;
    int B;
    int hits;
    int misses;
    int evictions;
};

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
static void accessData(Cache cache, char instruction, address mem, struct cacheInfo parts)
{
    /* Get the tag from the memory address */
	int tagSize = 64-parts.s-parts.b;
	address inputTag = mem >> (parts.s + parts.b);
    unsigned long long temp = mem << (tagSize);
    unsigned long long indexOfSet = temp >> (tagSize + parts.b);

    
    
}

/* main method
*/
int main(int argc, char** argv)
{
	
	if (argc > 4)
		return 0;

    printSummary(0, 0, 0);
    return 0;
    char c;

    struct cacheInfo parts;
    char* trace;

    if(argc > 0)
    {
    	while( (c = getopt(argc, argv, "s:E:b:t:") != -1) )
    	{
    		switch (c)
    		{
    			case 's':
    				parts.s = atoi(optarg);
    			case 'E':
    				parts.E = atoi(optarg);
    			case 'b':
    				parts.b = atoi(optarg);
    			case 't':
    				trace = optarg;
    		}
    	}
    }

    Cache myCache = initCache(parts.E, parts.s);

    char next;
    address mem;
    int size;

    if(trace != NULL)
    {
    	while( (scanf(trace, "%c, %llx, %d", &next, &mem, &size) ) == 3)
    	{
    		switch (next)
    		{
    			case 'I':
    				break;
    			case 'L':
    				accessData(myCache, next, mem, parts);
    				break;
    			case 'S':
    				accessData(myCache, next, mem, parts);
    				break;
    			case 'M':
    				accessData(myCache, next, mem, parts);
    				break;
    		}
    	}
    }
}


