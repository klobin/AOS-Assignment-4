#include "paging.h"

void run_LFU(page_list* list) {
    page* pageCurrent = list->head;
    page* pageMarkedToRemove = list->head;
    int min = pageCurrent->count;
    while(pageCurrent) {
        if(pageCurrent->count < min){ // logic to assess page delete condition
            pageMarkedToRemove = pageCurrent;
            min = pageCurrent->count;
        }
        pageCurrent = pageCurrent->next;
    }
    if (DEBUG==1) printf("EVICTED: p[%03d] c:%02d l:%02f\n", pageMarkedToRemove->pid, pageMarkedToRemove->count, pageMarkedToRemove->last_used);
    pageMarkedToRemove->pid = -1;
    pageMarkedToRemove->page_id = -1;
    pageMarkedToRemove->state = NOT_IN_MEM;
}
