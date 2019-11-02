#include "paging.h"

void create_page_list(page_list* list) {
    list->head = malloc(sizeof(page));
    page* curr = list->head;
    for(int i=0;i<PAGE_LIST_SIZE;i++) {
        curr->pid = -1; curr->page_id = -1;
        curr->state = NOT_IN_MEM;
        curr->next = NULL;
        if(i < 99) {
            curr->next = malloc(sizeof(page));
            curr = curr->next;
        }
    }
}

void display_memory_map(page_list* list) {
    page* curr = list->head;
    int count = 0;
    while(curr) {
        printf(curr->pid >= 0 ? "| pid[%03d] count:%02d prev_access:%02.2f |" : "|                     |",curr->pid, curr->count, curr->last_used);
        count++;
        if((count % 5) == 0) printf("\n");
        curr = curr->next;
    }
    printf("\n");
}

int get_k_free_pages(page_list* list,int count) { 
    page* curr = list->head;
    while(curr) {
        if(curr->pid == -1) { 
            count--;
        }
        if(!count) return 1;
        curr = curr->next;
    }
    return 0;
}

int is_page_in_memory(page_list* list,int pid,int page_id) {
    page* curr = list->head;
    while(curr) {
        if(curr->pid == pid && curr->page_id == page_id) return 1;
        curr = curr->next;
    }
    return 0;
}

page* find_free_page(page_list* list) {
    page* curr = list->head;
    while(curr) {
        if(curr->pid == -1) return curr;
        curr = curr->next;
    }
    return NULL;
}

int make_free_space(page_list* list,int pid) {
    page* curr = list->head;
    int count = 0;
    while(curr) {
        if(curr->pid == pid) {
            curr->pid = -1;
            curr->page_id = -1;
            curr->state = NOT_IN_MEM;
            count++;
        }
        curr = curr->next;
    }
    return count;
}

int find_next_page_id(int curr_page_id,int largest_page_size) {
    int x = rand()%10;
    if(x < 7) {
        x = curr_page_id+(rand()%3)-1 < 0;
    } else {
        x = rand()%largest_page_size;
        while(abs(x-curr_page_id) <= 1) x = rand()%largest_page_size;
    }
    if(x < 0) x = 0;
    if(x >= 100) x = largest_page_size-1;
    return x;
}

page* retrieve_page_by_id(page_list* list,int pid,int page_id) {
    page* curr = list->head;
    while(curr) {
        if(curr->pid == pid && curr->page_id == page_id) return curr;
        curr = curr->next;
    }
    return NULL;
}

int sort_by_arrival_times(const void* a,const void* b) {
    return ((process*)a)->arrival_time - ((process*)b)->arrival_time;
}

void display_process_queue(process* queue){
    int i;


    for (i=0; i < NUMBER_OF_TASKS; i++){
        if (queue[i].state != READY){
        printf("i: %03d, pid: %03d, page_num %03d, arrival_time %02d, service_time %.2f, curr_pg %03d, state %s, io_count %d\n",
         i, queue[i].pid, queue[i].page_num, queue[i].arrival_time, queue[i].service_time, queue[i].curr_page, display_state[queue[i].state], queue[i].io_count);
         }
    }
}
