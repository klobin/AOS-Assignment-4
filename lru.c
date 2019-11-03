#include "paging.h"

void run_LRU(page_list* list) {
    page* pageCurrent = list->head;
    page* pageMarkedToRemove = list->head;
    int lastUsedPage = pageCurrent->last_used;
    while(pageCurrent) {
        if(pageCurrent->last_used < lastUsedPage){
            pageMarkedToRemove = pageCurrent;
            lastUsedPage = pageCurrent->last_used;
        }
        pageCurrent = pageCurrent->next;
    }
    if (DEBUG==1) printf("REMOVED: p[%03d] c:%02d l:%02f\n", pageMarkedToRemove->pid, pageMarkedToRemove->count, pageMarkedToRemove->last_used);
    pageMarkedToRemove->pid = -1;
    pageMarkedToRemove->page_id = -1;
    pageMarkedToRemove->state = NOT_IN_MEM;
}
