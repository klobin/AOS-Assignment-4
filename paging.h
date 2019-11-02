#ifndef PAGING_H
#define PAGING_H

#define DEBUG 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_TASKS 150
#define PAGE_LIST_SIZE 100

extern int sim_clock;
extern int *page_count_opts;


enum state{
    RUNNING = 0,
    WAITING,
    FINISHED,
    READY,
};

char * display_state[5];


enum page_state{
    LOADING = 0,
    IN_MEM,
    NOT_IN_MEM,
};


typedef struct {
    int pid;
    float service_time;
    enum state state;

    int page_num;
    int arrival_time;
    int curr_page;
    int io_count;
} process;



// Information stored in a single node
typedef struct page {
    int pid; // indicator of the page holding the process running right now
    int page_id; // id of the page 
    struct page* next; // reference to next node on linked list
    int count;              // this is a count of used for LFU

    float first_access; 
    float last_used;          // this is a timestamp of the last time this page was used.
    enum page_state state;
} page;

// Linked List structure
typedef struct {
    page* head;
} page_list;


void create_page_list(page_list*);
void display_memory_map(page_list*);
int get_k_free_pages(page_list*,int); // test if atleast {count} no. of pages are free
int is_page_in_memory(page_list*,int,int);
page* find_free_page(page_list* list);
int make_free_space(page_list*,int);
int find_next_page_id(int,int);
int sort_by_arrival_times(const void* ,const void*);
page* retrieve_page_by_id(page_list* list,int pid,int page_no);
void display_process_queue(process* queue);

void run_FCFS(page_list* list);
void run_LRU(page_list* list);
void run_LFU(page_list* list);
void run_MFU(page_list* list);
void run_R(page_list* list);



#endif
