#include "paging.h"

void run_R(page_list* list) {
    page* curr = list->head;
    page* page_to_remove = list->head;
    int count = 0;
    int r = rand() % PAGE_LIST_SIZE;
    while(curr) {
        if(curr->pid>=0 && count < r){
            page_to_remove = curr;
        }
        curr = curr->next;
        count++;
    }
    if (DEBUG==1) printf("EVICTED: p[%03d] c:%02d l:%02f\n", page_to_remove->pid, page_to_remove->count, page_to_remove->last_used);
    page_to_remove->pid = -1;
    page_to_remove->page_id = -1;
    page_to_remove->state = NOT_IN_MEM;
}
