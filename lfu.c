#include "paging.h"

void run_LFU(page_list* list) {
    page* curr = list->head;
    page* page_to_remove = list->head;
    int min = curr->count;
    while(curr) {
        if(curr->count < min){
            page_to_remove = curr;
            min = curr->count;
        }
        curr = curr->next;
    }
    if (DEBUG==1) printf("EVICTED: p[%03d] c:%02d l:%02f\n", page_to_remove->pid, page_to_remove->count, page_to_remove->last_used);
    page_to_remove->pid = -1;
    page_to_remove->page_id = -1;
    page_to_remove->state = NOT_IN_MEM;
}
