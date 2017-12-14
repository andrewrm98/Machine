// Peter christakos pechristakos
// Andrew Morrison armorrison
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <strings.h>
#include <math.h>
#include "cachelab.h"

typedef unsigned long long int address;

/* holds the info for the cache
 * keeps track of all important information like hits, misses, and evictions 
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
    int evicts;
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

/* typedef to make initializing easier */
typedef struct Line Line;
typedef struct Set Set;
typedef struct Cache Cache;
typedef struct CacheInfo CacheInfo;

/* initCache initializes the cache */
Cache initCache(int linesize, long long setsize) 
{   // function creates the cache out of amounts of lines, sets, blocks 

    Cache cache; //create the cache for the function   
    Set set; // create set for cache
    Line line; // create lines for set
    cache.sets = (struct Set*) malloc(sizeof(struct Set) * setsize); //allocate memory for the cache set
    for (int i = 0; i < setsize; i++) // loop through each set
    {     
        set.lines =  (struct Line *) malloc(sizeof(struct Line) * linesize); // allocate memory for each individual line    
        cache.sets[i] = set; 
        for (int j = 0; j < linesize; j++) // loop through the line in each set and clear it
        {
            line.age = 0;
            line.valid = 0;
            line.tag = 0; 
            set.lines[j] = line; 
        }  
    } 
    return cache;
}

/* findEmptyLine checks for an empty line and returns the index */
int findEmptyLine(Set set, CacheInfo info)
// see if line in cache is empty, -1 means no and proper location means yes
{
    int index = -1; // if it stays -1 through the function, no empty lines
    for (int i = 0; i<info.E; i++)
    {
        if (set.lines[i].valid == 0)
        {
            index = i;
            break;
        }
    }
    return index; // return either location or -1
} 

/* findEvictionLine searches for the line to evict and returns the index */
int findEvictionLine(Set set, CacheInfo parts, int * lines_occupied)
{
    int index = 0;
    int least = set.lines[0].age; 
    int most = set.lines[0].age;

    for (int i = 0; i < parts.E; i++)
    {
        if(set.lines[i].age < least)
        {
            least = set.lines[i].age;
            index = i;
        }
        if(set.lines[i].age > most)
        {
            most = set.lines[i].age;
        }
    }
    lines_occupied[0] = most;
    return index;
}   
 
/* accessData will perform all actions on the cache */
CacheInfo accessData(Cache cache, CacheInfo parts, address mem) {

        int cachefull = 1;     // assume that cache is full
        int previousHit = parts.hits;
        
        /* get the tag */
        address raw_tag = mem >> (parts.s + parts.b);
        int tag_size = 64 - (parts.s + parts.b);
        unsigned long long indexOfSet = (mem << tag_size) >> (tag_size + parts.b);
        Set set = cache.sets[indexOfSet];
        int *lines_occupied = (int*) malloc(sizeof(int) * 2);
        int eviction_index = findEvictionLine(set, parts, lines_occupied);  

        for (int i = 0; i < parts.E; i++)  
        {
            if ((set.lines[i].valid != 0) && (set.lines[i].tag == raw_tag)) 
            {
                parts.hits++;  
                set.lines[i].age ++; 
            }
            else if ((cachefull) && !(set.lines[i].valid)) {
                // valid tag = 0, fullcache != 0 
                cachefull = 0;     // reset this to 0  because there is empty space left.
            }
            // 
        }
        if (previousHit == parts.hits) {   // if after the above loop nothing hit -> we miss
            parts.misses++;    // raise miss
        } 
        else 
        {
            return parts;
        }
        if (cachefull)     // if cache is full (checkFullCache!=0) do eviction
        {
            parts.evicts++;
            set.lines[eviction_index].tag = raw_tag;
            set.lines[eviction_index].age = lines_occupied[0] + 1;
        }

        else        
            {
            int empty_line = findEmptyLine(set, parts);
            set.lines[empty_line].tag = raw_tag;
            set.lines[empty_line].valid = 1;
            set.lines[empty_line].age = lines_occupied[0] + 1;
        }                       
    return parts;
}


int main(int argc, char **argv)
{
    /* Create necessary variables */
    CacheInfo parts;
    FILE *trace;

    /* Get all the input info */
    char x; // use x for switch
    while( (x=getopt(argc,argv,"s:E:b:t:vh")) != -1)
    {
        switch(x)
        {
        case 's':
            parts.s = atoi(optarg);
            break;
        case 'E':
            parts.E = atoi(optarg);
            break;
        case 'b':
            parts.b = atoi(optarg);
            break;
        case 't':
            trace = fopen(optarg, "r");
            break;
        }
    }

    /* Set up parts to default values and correct sizes of lines and sets*/
    parts.hits = 0;
    parts.misses = 0;
    parts.evicts = 0;
    parts.S = pow(2, parts.s);
    parts.B = pow(2, parts.b);

    /* initialize the cache */
    Cache cache = initCache (parts.E, parts.S);

    /* use ins, mem, and size to go through trace */
    char ins;
    address mem;
    int size;

    /* perform accesses on cache based on trace */
    if (trace != NULL) {
        while (fscanf(trace, " %c %llx,%d", &ins, &mem, &size) == 3) {
            switch(ins) {
                case 'I':
                    break;
                case 'L':
                    parts = accessData(cache, parts, mem);
                    break;
                case 'S':
                    parts = accessData(cache, parts, mem);
                    break;
                case 'M': // A modify is a store and a load so access cache twice
                    parts = accessData(cache, parts, mem);
                    parts = accessData(cache, parts, mem);  
                    break;
                default:
                    break;
            }
        }
    }
    printSummary(parts.hits, parts.misses, parts.evicts);
    fclose(trace);
    return 0;
}
