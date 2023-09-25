#include "oslabs.h"
#include<stdio.h>

void setup_pcb(struct PCB *inpcb, int process_id, int arrival_timestamp, int total_bursttime,
        int execution_starttime, int execution_endtime, int remaining_bursttime, int process_priority)
{
    (*inpcb).process_id = process_id;
    (*inpcb).arrival_timestamp = arrival_timestamp;
    (*inpcb).total_bursttime = total_bursttime;
    (*inpcb).execution_starttime = execution_starttime;
    (*inpcb).execution_endtime = execution_endtime;
    (*inpcb).remaining_bursttime = remaining_bursttime;
    (*inpcb).process_priority = process_priority;
}

int main(int argc, char** argv)
{
    struct PCB p1;
    struct PCB p2;
    struct PCB p3;
    struct PCB returned_pcb;
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 3;
    setup_pcb(&p1,1,1,4,0,0,4,23);
    setup_pcb(&p2,2,1,4,0,0,4,22);
    setup_pcb(&p3,3,1,4,0,0,4,24);

    ready_queue[0] = p1;
    ready_queue[1] = p2;
    ready_queue[2] = p3;

    returned_pcb = handle_process_completion_pp(ready_queue, &queue_cnt, 2);
    printf("hhh %d and %d and id is %d\n",returned_pcb.execution_starttime,returned_pcb.execution_endtime,returned_pcb.process_id);
    return 0;
}