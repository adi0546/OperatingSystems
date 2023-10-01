#include "oslabs.h"

#include<stdio.h>

int main(int argc, char** argv)
{
    struct MEMORY_BLOCK memory_map[MAPMAX];

    struct MEMORY_BLOCK m1 = build_mblock(0,1023,1024,0);
    memory_map[0] = m1;
    int map_cnt = 1;
    int request_size = 10;
    int pid = 32;

    best_fit_allocate(request_size,memory_map,&map_cnt,pid);

    printf("start is %d, end is %d, pid = %d\n",memory_map[0].start_address,memory_map[0].end_address,memory_map[0].process_id);
    printf("start is %d, end is %d, pid = %d\n",memory_map[1].start_address,memory_map[1].end_address,memory_map[1].process_id);

    //printf("hhh %d and %d and id is %d\n",returned_pcb.execution_starttime,returned_pcb.execution_endtime,returned_pcb.process_id);
    return 0;
}