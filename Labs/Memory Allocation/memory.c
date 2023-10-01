#include "oslabs.h"

struct MEMORY_BLOCK build_mblock(int start_address,int end_address, int segment_size, int process_id)
{
    struct MEMORY_BLOCK block;
    block.start_address = start_address;
    block.end_address = end_address;
    block.segment_size = segment_size;
    block.process_id = process_id;
    return block;
}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int best_idx = -1;
    int best_space = 999999;

    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size >= request_size && memory_map[i].segment_size < best_space && memory_map[i].process_id == 0){
            best_space = memory_map[i].segment_size;
            best_idx = i;
        }
    }
    if(best_idx == -1){
        return build_mblock(0,0,0,0);
    }
    if(memory_map[best_idx].segment_size > request_size){
        memory_map[*map_cnt].start_address = memory_map[best_idx].start_address + request_size;
        memory_map[*map_cnt].end_address = memory_map[best_idx].end_address;
        memory_map[*map_cnt].process_id = 0;
        memory_map[*map_cnt].segment_size = memory_map[best_idx].segment_size - request_size;
        memory_map[best_idx].segment_size = request_size;
        memory_map[best_idx].end_address = memory_map[best_idx].start_address+request_size-1;
        (*map_cnt)++;
    }
    memory_map[best_idx].process_id = process_id;
    return memory_map[best_idx];
}  

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int best_idx = -1;
    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0){
            best_idx = i;
            break;
        }
    }

    if(best_idx == -1){
        return build_mblock(0,0,0,0);
    }
    if(memory_map[best_idx].segment_size > request_size){
        memory_map[*map_cnt].start_address = memory_map[best_idx].start_address + request_size;
        memory_map[*map_cnt].end_address = memory_map[best_idx].end_address;
        memory_map[*map_cnt].process_id = 0;
        memory_map[*map_cnt].segment_size = memory_map[best_idx].segment_size - request_size;
        memory_map[best_idx].segment_size = request_size;
        memory_map[best_idx].end_address = memory_map[best_idx].start_address+request_size-1;
        (*map_cnt)++;
    }
    memory_map[best_idx].process_id = process_id;
    return memory_map[best_idx];

}  

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int best_idx = -1;
    int best_space = -1;

    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size >= request_size && memory_map[i].segment_size > best_space && memory_map[i].process_id == 0){
            best_space = memory_map[i].segment_size;
            best_idx = i;
        }
    }
    if(best_idx == -1){
        return build_mblock(0,0,0,0);
    }
    if(memory_map[best_idx].segment_size > request_size){
        memory_map[*map_cnt].start_address = memory_map[best_idx].start_address + request_size;
        memory_map[*map_cnt].end_address = memory_map[best_idx].end_address;
        memory_map[*map_cnt].process_id = 0;
        memory_map[*map_cnt].segment_size = memory_map[best_idx].segment_size - request_size;
        memory_map[best_idx].segment_size = request_size;
        memory_map[best_idx].end_address = memory_map[best_idx].start_address+request_size-1;
        (*map_cnt)++;
    }
    memory_map[best_idx].process_id = process_id;
    return memory_map[best_idx];
}  
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address)
{
    int last_addr_idx = -1;
    for(int i = 0;i < *map_cnt; i++){
        if(memory_map[i].start_address == last_address){
            last_addr_idx = i;
            break;
        }
    }
    int best_idx = -1;
    for(int i=last_addr_idx;i<*map_cnt;i++){
        if(memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0){
            best_idx = i;
            break;
        }
    }
    if(best_idx == -1){
        for(int i=0;i<last_addr_idx;i++){
        if(memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0){
            best_idx = i;
            break;
        }
    }
    }
    if(best_idx == -1){
        return build_mblock(0,0,0,0);
    }
    if(memory_map[best_idx].segment_size > request_size){
        memory_map[*map_cnt].start_address = memory_map[best_idx].start_address + request_size;
        memory_map[*map_cnt].end_address = memory_map[best_idx].end_address;
        memory_map[*map_cnt].process_id = 0;
        memory_map[*map_cnt].segment_size = memory_map[best_idx].segment_size - request_size;
        memory_map[best_idx].segment_size = request_size;
        memory_map[best_idx].end_address = memory_map[best_idx].start_address+request_size-1;
        (*map_cnt)++;
    }
    memory_map[best_idx].process_id = process_id;
    return memory_map[best_idx];

} 

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt)
{
    int idx_to_free = -1;
    int temp = -1;
    for(int i = 0;i<*map_cnt;i++){
        if(memory_map[i].start_address == freed_block.start_address && memory_map[i].end_address == freed_block.end_address){
            idx_to_free = i;
            break;
        }
    }
    temp = idx_to_free;
    int removeCount = 1;
    memory_map[idx_to_free].process_id = 0;

    int newStart = memory_map[idx_to_free].start_address;
    int newEnd = memory_map[idx_to_free].end_address;
    int newSegmentSize = memory_map[idx_to_free].segment_size;

    if(memory_map[idx_to_free-1].process_id == 0){
        newStart = memory_map[idx_to_free-1].start_address;
        newSegmentSize+= memory_map[idx_to_free-1].segment_size;
        temp = idx_to_free - 1;
        removeCount+=1;
    }
    if(memory_map[idx_to_free+1].process_id == 0){
        newEnd = memory_map[idx_to_free+1].end_address;
        newSegmentSize+= memory_map[idx_to_free+1].segment_size;
        removeCount+=1;
    }

    struct MEMORY_BLOCK newm = build_mblock(newStart,newEnd,newSegmentSize,0);
    
    memory_map[temp] = newm;
    temp+=1;
    int shift = temp+removeCount-1;

    while(shift < *map_cnt){
        memory_map[temp] = memory_map[shift];
        temp+=1;
        shift+=1;
    }

    (*map_cnt) -= (removeCount-1);
}