#include "paging.h"

void run_FCFS(page_list* list) {
    page* pageCurrent = list->head;
    page* pageMarkedToRemove = list->head;
    while(pageCurrent) {
        if(pageCurrent->first_access < pageMarkedToRemove->first_access) {
            pageMarkedToRemove = pageCurrent;
        }
        pageCurrent = pageCurrent->next;
    }
    if (DEBUG==1) printf("REMOVED: p[%03d] c:%02d l:%02f\n", pageMarkedToRemove->pid, pageMarkedToRemove->count, pageMarkedToRemove->last_used);
    pageMarkedToRemove->pid = -1;
    pageMarkedToRemove->page_id = -1;
    pageMarkedToRemove->state = NOT_IN_MEM;
}
