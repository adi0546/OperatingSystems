 #include "oslabs.h"

int test_null_pcb(struct PCB inpcb){
    if(inpcb.process_id == 0 &&
        inpcb.arrival_timestamp == 0 &&
        inpcb.total_bursttime == 0 &&
        inpcb.execution_starttime == 0 &&
        inpcb.execution_endtime == 0 &&
        inpcb.remaining_bursttime == 0 &&
        inpcb.process_priority == 0)
        return 1;
    else
        return 0;
}


void create_null_pcb(struct PCB *nullpcb)
{
    (*nullpcb).process_id = 0;
    (*nullpcb).arrival_timestamp = 0;
    (*nullpcb).total_bursttime = 0;
    (*nullpcb).execution_starttime = 0;
    (*nullpcb).execution_endtime = 0;
    (*nullpcb).remaining_bursttime = 0;
    (*nullpcb).process_priority = 0;
}

void remove_process_from_queue(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int remove_ix)
{
    for(int i = 0; i <  *queue_cnt - 1; i++){
        if(i>=remove_ix){
            ready_queue[i] = ready_queue[i+1];
        }
    }
    *queue_cnt-=1;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    if(test_null_pcb(current_process)){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else if(new_process.process_priority >= current_process.process_priority){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
    else{
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= timestamp - current_process.execution_starttime;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
 }

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){

    if(*queue_cnt == 0){
        struct PCB nullpcb;
        create_null_pcb(&nullpcb);
        return nullpcb;
    }
    int minIdx = 0;
    int minPriority = ready_queue[minIdx].process_priority;
    for(int i = 0;i<*queue_cnt;i++){
        if( ready_queue[i].process_priority < minPriority){
            minPriority = ready_queue[i].process_priority;
            minIdx = i;
        }
    }
    struct PCB removedPCB = ready_queue[minIdx];
    removedPCB.execution_starttime = timestamp;
    removedPCB.execution_endtime = timestamp + removedPCB.remaining_bursttime;
    remove_process_from_queue(ready_queue, queue_cnt, minIdx);
    return removedPCB;
}
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
    if(test_null_pcb(current_process)){
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    else if(current_process.remaining_bursttime < new_process.total_bursttime){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
    else{
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= time_stamp - current_process.execution_starttime;
        current_process.execution_starttime = 0;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    
    if(*queue_cnt == 0){
        struct PCB nullpcb;
        create_null_pcb(&nullpcb);
        return nullpcb;
    }
    int minIdx = 0;
    int minBurstTime = ready_queue[minIdx].remaining_bursttime;
    for(int i = 0;i<*queue_cnt;i++){
        if( ready_queue[i].remaining_bursttime < minBurstTime){
            minBurstTime = ready_queue[i].remaining_bursttime;
            minIdx = i;
        }
    }
    struct PCB removedPCB = ready_queue[minIdx];
    removedPCB.execution_starttime = timestamp;
    removedPCB.execution_endtime = timestamp + removedPCB.remaining_bursttime;
    remove_process_from_queue(ready_queue, queue_cnt, minIdx);
    return removedPCB;
}
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    if(test_null_pcb(current_process)){
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + MIN(time_quantum,new_process.total_bursttime);
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    else{
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum){
    if(*queue_cnt == 0){
        struct PCB nullpcb;
        create_null_pcb(&nullpcb);
        return nullpcb;
    }
    int minIdx = 0;
    int minArrivalTime = ready_queue[minIdx].arrival_timestamp;
    for(int i = 0;i<*queue_cnt;i++){
        if( ready_queue[i].arrival_timestamp < minArrivalTime){
            minArrivalTime = ready_queue[i].arrival_timestamp;
            minIdx = i;
        }
    }
    struct PCB removedPCB = ready_queue[minIdx];
    removedPCB.execution_starttime = timestamp;
    removedPCB.execution_endtime = timestamp + MIN(time_quantum,removedPCB.remaining_bursttime);
    remove_process_from_queue(ready_queue, queue_cnt, minIdx);
    return removedPCB;
}
