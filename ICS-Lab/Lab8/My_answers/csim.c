//Name:Lilin	ID:5130379091

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
I:instruction load
L:data load
S:data store
M:data modify
*/

const int MAXLINE = 100;            //max lines in the cache

//each line in the cache
typedef struct
{
	int valid;                     //is valid?
	int tag;                       //tag
	int access_time;               //the access time for LRU
} Cache_line;                       

//each set in the cache 
typedef struct
{
	Cache_line *lines;             //each set has E lines
} Cache_set;

//the whole cache
typedef struct
{
	int set_num;                   //the number of sets
	Cache_set *sets;               //the sets
} Cache;

Cache *cache;
int s;
int E;
int b;
char* filename = NULL;

long clocktime = 0;

int hits = 0;
int misses = 0;
int evictions = 0;

void get_opt(int argc, char **argv);    //get the s,E,b and t from the order
void Init();                            //init the cache

void Handle_L(void *addr, int size);    //if the option is L
void Handle_M(void *addr, int size);    //if the option is M
void Handle_S(void *addr, int size);    //if the option is S

int  get_set(void *addr);               //get set from the input
long get_tag(void *addr);               //get tag from the input

void get_opt(int argc, char **argv)
{
    char c;
    while ((c = getopt(argc, argv, "s:E:b:t:")) != -1)
    {
        switch (c)
        {
            case 's':
                s = atoi(optarg);
                //printf("%d\n",s);
                break;
            case 'E':
                E = atoi(optarg);
                //printf("%d\n",E);
                break;
            case 'b':
                b = atoi(optarg);
                //printf("%d\n",b);
                break;
            case 't':
                filename = optarg;
                //printf("%s\n",filename);
                break;
            default:
                printf("illegal opt\n");
                exit(0);
        }
    }
}

void Init()
{
	cache = (Cache*)malloc(sizeof(Cache));     //init the cache ptr
	//printf("%d\n", s);
	cache->set_num = (1 << s);                 //S = 2 ^ s 
	//printf("%d\n", cache->set_num);
	cache->sets = (Cache_set*)malloc(cache->set_num * sizeof(Cache_set));
    //malloc place for sets and lines
	for (int i = 0; i < cache->set_num; ++i)
	{
		cache->sets[i].lines = (Cache_line*)malloc(E * sizeof(Cache_line));
		for (int j = 0; j < E; ++j)
		{
			cache->sets[i].lines[j].valid  = 0;
			cache->sets[i].lines[j].tag  = 0;
			cache->sets[i].lines[j].access_time  = 0;
		}
	}
}

// get the set from the address
int get_set(void *addr)
{
    return (int)(((long)addr >> b) & ((1 << s) - 1));
}

// get the tag from the address
long get_tag(void *addr)
{
    return (long)((long)addr >> (s + b));
}

//deal with the 'L'
void Handle_L(void *addr, int size)
{
    int set_num = get_set(addr);
    Cache_set *this_set = &cache->sets[set_num];

    int is_hit = 0;             //is hit?
    int is_full = 1;            //is this line full?
    int empty_item = 0;         // if not full, keep track of the empty item
    int last_item = 0;          // if full, keep track of the evict item
    int last_time = this_set->lines[0].access_time;

    for (int i = 0; i < E; i++)
    {
        //if hit, update the access time
        if (this_set->lines[i].valid == 1 && this_set->lines[i].tag == get_tag(addr))
        {
        	++clocktime;
            this_set->lines[i].access_time = clocktime;
            is_hit = 1;
            break;
        }
        //not valid, then this entry is considered empty which means cache is not full
        else if (this_set->lines[i].valid == 0)
        {
            is_full = 0;
            empty_item = i;
        }
        //valid but tag not equal
        else
        {
            // keep track of the LRU item, ready for eviction
            if (this_set->lines[i].access_time < last_time)
            {
                last_item = i;
                last_time = this_set->lines[i].access_time;
            }
        }
    }

    //update the hits,misses and evictions
    if (!is_hit) // miss
    {
        misses++;
        if (is_full)   //evict
        {
        	++clocktime;
            this_set->lines[last_item].tag = get_tag(addr);
            this_set->lines[last_item].access_time = clocktime;
            evictions++;
        }
        else
        {
        	++clocktime;
            this_set->lines[empty_item].valid = 1;
            this_set->lines[empty_item].tag = get_tag(addr);
            this_set->lines[empty_item].access_time = clocktime;
        }
    }
    else    // hit
    {
        hits++;
    }

}

//deal with the 'S'
//if hit,hit++;
//else Handle_L
void Handle_S(void *addr, int size)
{
    int set_num = get_set(addr);
    Cache_set *this_set = &cache->sets[set_num];

    int is_hit = 0;

    for (int i = 0; i < E; ++i)
    {
        if (this_set->lines[i].valid == 1 && this_set->lines[i].tag == get_tag(addr))
        {
            ++clocktime;
            this_set->lines[i].access_time = clocktime;
            is_hit = 1;
            break;
        }
    }

    if (!is_hit) // store miss
    {
        Handle_L(addr, size);   // if miss, then load
    }
    else            // store hit
    {
        hits++;
    }
}

//deal with the 'M'
//modify means load and then store
void Handle_M(void *addr, int size)
{
    Handle_L(addr, size);
    Handle_S(addr, size);
}

int main(int argc, char **argv)
{
    get_opt(argc,argv);
    Init();

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "open file error\n");
        exit(0);
    }

    char op[MAXLINE];
    void *addr;
    int size;
    char buf[MAXLINE];
    //read from the file
    while (fgets(buf, MAXLINE, fp) != NULL)
    {
    	//printf("\nline read: %s\n", buf);
        sscanf(buf, "%s %p,%d", op, &addr, &size);
        //printf("op = %s, addr = %p, size = %d\n", op, addr, size);
    
        if (*op == 'L')
        {
        	//printf("%c\n",*op);
            Handle_L(addr, size);
        }
        else if (*op == 'M')
        {
        	//printf("%c\n",*op);
            Handle_M(addr, size);
        }
        else if (*op == 'S')
        {
        	//printf("%c\n",*op);
            Handle_S(addr, size);
        }
    }

    printSummary(hits, misses, evictions);
    return 0;
}