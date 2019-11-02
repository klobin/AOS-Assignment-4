#include "paging.h"

void run_LRU(page_list* list) {
    page* curr = list->head;
    page* page_to_remove = list->head;
    int lru = curr->last_used;
    while(curr) {
        if(curr->last_used < lru){
            page_to_remove = curr;
            lru = curr->last_used;
        }
        curr = curr->next;
    }
    if (DEBUG==1) printf("REMOVED: p[%03d] c:%02d l:%02f\n", page_to_remove->pid, page_to_remove->count, page_to_remove->last_used);
    page_to_remove->pid = -1;
    page_to_remove->page_id = -1;
    page_to_remove->state = NOT_IN_MEM;
}
