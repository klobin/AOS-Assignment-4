#include "paging.h"

int main(int argc, char* argv[]) {

    int clock_curr = 0;
    int n = 0;
    
    int *processSizePerPage = malloc(sizeof(int)*4);
    processSizePerPage[0] = 5;
    processSizePerPage[1] = 11;
    processSizePerPage[2] = 17;
    processSizePerPage[3] = 31;

    for(int i=0;i<5;i++) displayState[i] = malloc(3);
    displayState[0] = "RU";
    displayState[1] = "WA";
    displayState[2] = "FI";
    displayState[3] = "RE";
    displayState[4] = "PE";

    page *curr_page;

    void (*run_func)(page_list*);
    if (argc != 2){
        printf("Select page replacement algorithm: fifo, lru, lfu, mfu or r.\n");
        return -1;
    }
    if(strcmp(argv[1], "fifo") == 0){
        run_func = run_FCFS;
    }else if(strcmp(argv[1], "lru") == 0){
        run_func = run_LRU;
    }else if(strcmp(argv[1], "lfu") == 0){
        run_func = run_LFU;
    }else if(strcmp(argv[1], "mfu") == 0){
        run_func = run_MFU;
    }else if(strcmp(argv[1], "r") == 0){
        run_func = run_R;
    }else {
        printf("valid options are: fifo, lru, lfu, mfu or r.\n");
        return -1;
    }

    int swapped_process = 0;
    float avg_hitmiss_ratio = 0;
    int hit_count = 0;
    int miss_count = 0;
    srand(0);
    for(int i = 0; i < 5; i++) {
        printf("===================== Run %d =================\n", i+1);
        hit_count = 0;
        miss_count = 0;
        page_list list;
        create_page_list(&list);
        process Q[NUMBER_OF_TASKS];

        for(int i=0;i<NUMBER_OF_TASKS;i++) {
            Q[i].pid = i;
            Q[i].page_num = processSizePerPage[rand()%4]; 
            Q[i].arrival_time = rand()%60;
            Q[i].service_time = rand() % 7;
            Q[i].curr_page = 0; 
            Q[i].state = READY;
        }

        qsort(Q,NUMBER_OF_TASKS,sizeof(process), sort_by_arrival_times);

        int curr_q_ptr = 0; 
        for(clock_curr = 0; clock_curr < 60; clock_curr++) {
            printf("\ntime: %d:\n", clock_curr);

            while(curr_q_ptr < NUMBER_OF_TASKS 
                        && Q[curr_q_ptr].arrival_time <= clock_curr 
                        && Q[curr_q_ptr].state == READY) {

                // Check if there are 4 free pages that are available
                if(get_k_free_pages(&list,4)) { 
                    // bring process into page table
                    page* p = find_free_page(&list);
                    p->pid = Q[curr_q_ptr].pid;
                    p->page_id = Q[curr_q_ptr].curr_page;
                    p->state = LOADING;
                    swapped_process++;
                    miss_count++; // first page of process, hence a miss
                    Q[curr_q_ptr].state = WAITING;
                    Q[curr_q_ptr].io_count = 2 + rand() % 3;
                    Q[curr_q_ptr].service_time += Q[curr_q_ptr].io_count;
                    curr_q_ptr++;
                } else{
                    printf("Out of Memory: Process id %d pid %d not allocated \n", curr_q_ptr, Q[curr_q_ptr].pid);
                    break;  // no memory
                }

            }


            // Sending request every 100 ms/10 seconds
            for(int i=0;i<10;i++) { 
                for(int j=0;j<curr_q_ptr;j++) if(Q[j].service_time > 0) {
                    Q[j].curr_page = find_next_page_id(Q[j].curr_page,Q[j].page_num); // update current page no.
                    if(is_page_in_memory(&list,Q[j].pid,Q[j].curr_page)) { 
                        curr_page = retrieve_page_by_id(&list,Q[j].pid,Q[j].curr_page);
                        if (curr_page == NULL){
                            printf("Possible error: page is NULL: pid %d page %d\n", Q[j].pid, Q[j].curr_page);
                            return -1;
                        }

                        if (curr_page->state == IN_MEM){
                            curr_page->count++;
                            curr_page->last_used = clock_curr;
                            hit_count++;
                        }else if (curr_page->state == LOADING) {
                            Q[j].io_count--;
                            printf("pid: %03d, page_count %03d, arrival_time %02d, service_time %.2f, curr_page %03d, state %s, io_count %d\n",
                            Q[j].pid, Q[j].page_num, Q[j].arrival_time, Q[j].service_time, Q[j].curr_page, displayState[Q[j].state], Q[j].io_count);
                            if (Q[j].io_count <= 0){
                                curr_page->first_access = clock_curr+(0.1*i);
                                curr_page->state = IN_MEM;
                                curr_page->last_used = clock_curr+(0.1*i);
                                Q[j].state = RUNNING;
                                curr_page->page_id = Q[j].curr_page;
                            }
                        }
                        continue;
                    }

                    // When a page that is referred is not in memory, it is loaded onto the memory
                    page* pg = find_free_page(&list);
                    if(!pg) { 
                        // remove page as there are no free pages 
                        printf("OUT OF MEMORY, Page Table Map:\n");
                        display_memory_map(&list);
                        run_func(&list);
                        miss_count++;
                        Q[j].io_count = 2 + rand() % 3; 
                        display_memory_map(&list);
                        pg = find_free_page(&list); 

                    }

                    if (pg->state != NOT_IN_MEM){
                        printf("BUG: free page with following state found: %d\n",pg->state);
                    }

                    Q[j].state = WAITING;
                    Q[j].io_count = 2 + rand() % 3;
                    Q[j].service_time += Q[j].io_count;
                    pg->state = LOADING;
                    pg->pid = Q[j].pid;
                    pg->page_id = Q[j].curr_page;
                    pg->count = 0;
                }
            } // 100ms complete

            // don't run processes with RUNNING state as time is over
            for(int j=0;j<curr_q_ptr;j++) if(Q[j].service_time > 0 && Q[j].state == RUNNING) {
                Q[j].service_time -= 1.0;
                if(Q[j].service_time <= 0) { 
                    // make free space in memory
                    n = make_free_space(&list,Q[j].pid);
                    Q[j].state = FINISHED;
                    printf("Process %03d done. %d pages freed\n",Q[j].pid, n);
                }
            }

            usleep(800);
            display_process_queue(Q);
        }
        printf("Run %d: Hit(%d)/Miss(%d) Ratio\n\n", i+1, hit_count, miss_count);
        avg_hitmiss_ratio += hit_count/(1.0*miss_count);
    }
    printf("Avg. number of successfully swapped in processes %d\n", (swapped_process / 5));
    printf("Avg. hit/miss ratio: %.2f\n",(avg_hitmiss_ratio/5));
}

