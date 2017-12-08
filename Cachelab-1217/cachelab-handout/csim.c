/* Andrew Morrison
 *  armorrison
    Peter Christakos
    pechristakos
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

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

/* typedef to make defining easier */
typedef struct Line Line;
typedef struct Set Set;
typedef struct Cache Cache;
typedef struct CacheInfo CacheInfo;

/********************************************** END OF STRUCT CREATION *******************************************/

/* initCache initializes the cache
 * it starts by using malloc to allocate memory for the cache
 * it then uses malloc to create a set
 * then assigns the sets to the cache
 */
Cache initCache(int linesize, long long setsize, long long blocksize) 
{   

    Cache cache; //create the cache for the function   
    Set set; // create set for cache
    Line line; // create lines for set
    cache.sets = (struct Set*) malloc(sizeof(struct Set) * setsize); //allocate memory for the cache set
    for (int i = 0; i < setsize; i++) // loop through each set
    {     
        set.lines =  (struct Line *) malloc(sizeof(struct Line) * linesize); // allocate memory for each individual line    
        cache.sets[i] = set; 
        for (int j = 0; j < linesize; j++) // loop through the line in each set and set default values

        {
            line.age = 0;
            line.valid = 0;
            line.tag = 0; 
            set.lines[j] = line; 
        }  
    } 
    return cache; // return the cache with memory allocated
}

/* detect if an eviction is necessary
*/
int findEvictionLine(Set set, CacheInfo parts, int * usedLines)
{
    // Keep track of least and most to implement LRU cache without a linked list
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
    // Save most and return index
    usedLines[0] = most;
    return index;
}   

int findEmptyLine(Set set, CacheInfo info)
// see if line in cache is empty, -1 means no and proper location means yes
{
    int index = -1; // default value meaning no location
    for (int i = 0; i<info.E; i++)
    {
        if (set.lines[i].valid == 0)
        {
            index = i;
            return index;
        }
    }
    return index; // return -1 if no line available
} 



/* accesData will perform all accesses on the cache
 * First, check if hit
 * then, check if eviction
 * finally, check for miss
 */
CacheInfo accessData(Cache cache, CacheInfo parts, address mem) {

        int cachefull = 1;  // variable that sais cache is full, will change otherwise
        int previousHit = parts.hits; // measure previousHit before looping
      address raw_tag = mem >> (parts.s + parts.b);
        int tag_size = (64 - (parts.s + parts.b));
        unsigned long long temp = mem << (tag_size);
        unsigned long long indexOfSet = temp >> (tag_size + parts.b);
        Set set = cache.sets[indexOfSet];
        int *lines_occupied = (int*) malloc(sizeof(int) * 2); //malloc array of occupied lines
        int eviction_index = findEvictionLine(set, parts, lines_occupied); //set variable has index for function

        for (int i = 0; i < parts.E; i++)  // loop through checking each line to see if hit
        {
            if ((set.lines[i].valid != 0) && (set.lines[i].tag == raw_tag)) 
            {
                parts.hits++;  
                set.lines[i].age ++; // if hits, incrememnt hits and age, the older the less recently accessed
            }
            else if ((cachefull) && !(set.lines[i].valid)) { 
                cachefull = 0;  // set cachefull back to zero if there is room
            } 
        }
        if (previousHit == parts.hits) {   // if we didn't find the hit, then it counts as a miss
            parts.misses++;
        } 
        else 
        {
            return parts;
        }
        if (cachefull) // if the cache is full then an eviction is needed
        {
            parts.evictions++;
            set.lines[eviction_index].tag = raw_tag;
            set.lines[eviction_index].age = lines_occupied[0] + 1;
        }
        else        // run find empty line function
            {
            int empty_line = findEmptyLine(set, parts);
            set.lines[empty_line].tag = raw_tag;
            set.lines[empty_line].valid = 1;
            set.lines[empty_line].age = lines_occupied[0] + 1;
        }
    // return parts to keep track of S, B, E, misses, evictions, and hits                       
    return parts;
}

/* main method
*/
int main(int argc, char **argv)
{
    // create all variables
    Cache myCache;
    CacheInfo parts;
    bzero(&parts, sizeof(parts)); 
    long long numberOfSets;
    long long sizeOfBlock;  
    FILE *openTrace;
    char instructionInTraceFile;
    address mem;
    int size;
    char *trace_file;
    char x;s

    while( (x = getopt(argc, argv, "s:E:b:t:vh")) != -1)
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
            trace_file = optarg;
            break;
        case 'h':
            exit(0);
        default:
            exit(1);
        }
    }
   /* end of take in arguments from command line */ 

    numberOfSets = pow(2.0, parts.s);   // get Number of set by 2^s
    sizeOfBlock = pow(2.0, parts.b);  //  get sizeOfBlock by 2^b
    parts.hits = 0;
    parts.misses = 0;
    parts.evictions = 0;
    myCache = initCache (parts.E, numberOfSets, sizeOfBlock);

    /* this part read file. More on how do I do this-> read report file */
    openTrace  = fopen(trace_file, "r");
    if (openTrace != NULL) {
        while (fscanf(openTrace, " %c %llx,%d", &instructionInTraceFile, &mem, &size) == 3) {
            switch(instructionInTraceFile) {
                case 'I':
                    break;
                case 'L':
                    parts = accessData(myCache, parts, mem);
                    break;
                case 'S':
                    parts = accessData(myCache, parts, mem);
                    break;
                case 'M':
                    parts = accessData(myCache, parts, mem);
                    parts = accessData(myCache, parts, mem);  
                    break;
                default:
                    break;
            }
        }
    }
    printSummary(parts.hits, parts.misses, parts.evictions);
    fclose(openTrace);
    return 0;
}
