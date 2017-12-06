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
struct CacheInfo
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
    char* block;
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

/* exponent function
*/
int MyPow(int a,int b)
{
      if(b<0)      
        return (1.0/a)*MyPow(a,abs(b)-1);
      else if(b==0)
        return 1;
      else if(b==1)
        return a;
      else
        return a*MyPow(a,b-1);
}

/* Initializer for the cache
*/
Cache initCache (int numLines, int numSets, int blockSize)
{
	// Necessary local variables, edit cache and return it
	Cache cache;
	Set set;
	Line line;
	line.valid = 0;
	line.tag = 0;
	line.age = 0;
    line.block = malloc(blockSize); // dynamically allocate memory to the block
	
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
	
	for(int i = 0; i<numSets; i++)
	{
		for(int j=0; j<numLines; j++)
		{
			printf("valid: %d, age: %d\n", cache.sets[i].lines[j].valid, cache.sets[i].lines[j].age);
		}
	} */
	return cache;
}

/* detects if the line is empty
*/
static int detectEmptyLine(Cache cache, struct CacheInfo info)
{
    return 0;
} 

/* detect if an eviction is necessary
*/
static int detectEvictLine(Cache cache, struct CacheInfo info)
{
    return 0;
}   

/* accesData will perform all actions on the cache
*/
static void accessData(Cache cache, char instruction, address mem, struct CacheInfo parts)
{
    /* Get the tag and setIndex from the memory address */
	int tagSize = 64-parts.s-parts.b;
	address inputTag = mem >> (parts.s + parts.b);
    unsigned long long temp = mem << (tagSize);
    unsigned long long setIndex = temp >> (tagSize + parts.b);

    /* go through each line in the set*/
    for (int i =0; i<parts.E; i++)
    {
        /* if the tags are equal and the valid tag is not zero then it is a hit */
        if( (cache.sets[setIndex].lines[i].tag == inputTag) && cache.sets[setIndex].lines[i].valid != 0)
        {
            /* increment the age for eviction function */
            cache.sets[setIndex].lines[i].age ++;
            parts.hits ++;
        }
    }
    
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

    struct CacheInfo parts;
    char* trace;

    if(argc > 0)
    {
    	while( (c = getopt(argc, argv, "s:E:b:t:") != -1) )
    	{
    		switch (c)
    		{
    			case 's':
    				parts.s = atoi(optarg);
                    parts.S = MyPow(2, parts.s);
    			case 'E':
    				parts.E = atoi(optarg);
    			case 'b':
    				parts.b = atoi(optarg);
                    parts.B = MyPow(2, parts.b);
    			case 't':
    				trace = optarg;
    		}
    	}
    }

    Cache myCache = initCache(parts.E, parts.S, parts.B);

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


