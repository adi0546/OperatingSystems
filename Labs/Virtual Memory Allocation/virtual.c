#include<stdio.h>
#include<stdbool.h>
#include"oslabs.h"
	
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == 1) {
		if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
		else page_table[page_number].reference_count++;
		
		page_table[page_number].last_access_timestamp = current_timestamp;
		return page_table[page_number].frame_number;
	}
	else {
		if (*frame_cnt > 0) {
			page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
			frame_pool[*frame_cnt-1] = -1;
			*frame_cnt -= 1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].is_valid = 1;
			return page_table[page_number].frame_number;
		}
		else if (*frame_cnt == 0) {
			int earliest_arrival_timestamp = -1;
			int chosen_pte_idx = -1;
			for (int i =0; i < *table_cnt; i++) {
				if(page_table[i].is_valid == 1 && 
				  (earliest_arrival_timestamp == -1 || page_table[i].arrival_timestamp < earliest_arrival_timestamp))  {
					chosen_pte_idx = i;
					earliest_arrival_timestamp = page_table[i].arrival_timestamp;
				}
			}
			int freed_frame_number = page_table[chosen_pte_idx].frame_number;
			
			page_table[chosen_pte_idx].is_valid = 0;
			page_table[chosen_pte_idx].frame_number = -1;
			page_table[chosen_pte_idx].arrival_timestamp = -1;
			page_table[chosen_pte_idx].last_access_timestamp = -1;
			page_table[chosen_pte_idx].reference_count = -1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].frame_number = freed_frame_number;
			page_table[page_number].is_valid = 1;
			
			return page_table[page_number].frame_number;
		}
	}
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
	
	int latest_arrival_timestamp = -1;
	int latest_pte_idx = -1;
	for (int i =0; i < table_cnt; i++) {
		if(page_table[i].is_valid == 1 && 
		(latest_arrival_timestamp == -1 || page_table[i].arrival_timestamp > latest_arrival_timestamp))  {
			latest_pte_idx = i;
			latest_arrival_timestamp = page_table[i].arrival_timestamp;
		}
	}
	
	int current_timestamp = latest_arrival_timestamp + 1;
	int page_fault_cnt = 0;
	
	for (int ref = 0; ref < reference_cnt; ref++) {
		int page_number = refrence_string[ref];
		
		if (page_table[page_number].is_valid == 1) {
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			
			page_table[page_number].last_access_timestamp = current_timestamp;
		}
		else {
			page_fault_cnt++;
			if (frame_cnt > 0) {
				page_table[page_number].frame_number = frame_pool[frame_cnt-1];
				frame_pool[frame_cnt-1] = -1;
				frame_cnt -= 1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].is_valid = 1;
			}
			else if (frame_cnt == 0) {
				int earliest_arrival_timestamp = -1;
				int chosen_pte_idx = -1;
				for (int i =0; i < table_cnt; i++) {
					if(page_table[i].is_valid == 1 && 
					(earliest_arrival_timestamp == -1 || page_table[i].arrival_timestamp < earliest_arrival_timestamp))  {
						chosen_pte_idx = i;
						earliest_arrival_timestamp = page_table[i].arrival_timestamp;
					}
				}
				int freed_frame_number = page_table[chosen_pte_idx].frame_number;
				
				page_table[chosen_pte_idx].is_valid = 0;
				page_table[chosen_pte_idx].frame_number = -1;
				page_table[chosen_pte_idx].arrival_timestamp = -1;
				page_table[chosen_pte_idx].last_access_timestamp = -1;
				page_table[chosen_pte_idx].reference_count = -1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].frame_number = freed_frame_number;
				page_table[page_number].is_valid = 1;
			}
		}
		current_timestamp++;
	}
	return page_fault_cnt;
}
	
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == 1) {
		if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
		else page_table[page_number].reference_count++;
		
		page_table[page_number].last_access_timestamp = current_timestamp;
		
		return page_table[page_number].frame_number;
	}
	else {
		if (*frame_cnt > 0) {
			page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
			frame_pool[*frame_cnt-1] = -1;
			*frame_cnt -= 1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].is_valid = 1;
			
			return page_table[page_number].frame_number;
		}
		else if (*frame_cnt == 0) {
			int earliest_access_timestamp = -1;
			int chosen_pte_idx = -1;
			for (int i =0; i < *table_cnt; i++) {
				if(page_table[i].is_valid == 1 && 
				  (earliest_access_timestamp == -1 || page_table[i].last_access_timestamp < earliest_access_timestamp))  {
					chosen_pte_idx = i;
					earliest_access_timestamp = page_table[i].last_access_timestamp;
				}
			}
			int freed_frame_number = page_table[chosen_pte_idx].frame_number;
			
			page_table[chosen_pte_idx].is_valid = 0;
			page_table[chosen_pte_idx].frame_number = -1;
			page_table[chosen_pte_idx].arrival_timestamp = -1;
			page_table[chosen_pte_idx].last_access_timestamp = -1;
			page_table[chosen_pte_idx].reference_count = -1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].frame_number = freed_frame_number;
			page_table[page_number].is_valid = 1;
			
			return page_table[page_number].frame_number;
		}
	}
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
	
	int latest_access_timestamp = -1;
	int latest_pte_idx = -1;
	int valid_entries_cnt = 0;
	for (int i =0; i < table_cnt; i++) {
		if(page_table[i].is_valid == 1 && 
		(latest_access_timestamp == -1 || page_table[i].last_access_timestamp > latest_access_timestamp))  {
			latest_pte_idx = i;
			latest_access_timestamp = page_table[i].last_access_timestamp;
		}
	}
	
	int current_timestamp = latest_access_timestamp + 1;
	int page_fault_cnt = 0;
	
	for (int ref = 0; ref < reference_cnt; ref++) {
		int page_number = refrence_string[ref];
		
		if (page_table[page_number].is_valid == 1) {
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			
			page_table[page_number].last_access_timestamp = current_timestamp;
		}
		else {
			page_fault_cnt++;
			if (frame_cnt > 0) {
				page_table[page_number].frame_number = frame_pool[frame_cnt-1];
				frame_pool[frame_cnt-1] = -1;
				frame_cnt -= 1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].is_valid = 1;
			}
			else if (frame_cnt == 0) {
				int earliest_access_timestamp = -1;
				int chosen_pte_idx = -1;
				for (int i =0; i < table_cnt; i++) {
					if(page_table[i].is_valid == 1 && 
					(earliest_access_timestamp == -1 || page_table[i].last_access_timestamp < earliest_access_timestamp))  {
						chosen_pte_idx = i;
						earliest_access_timestamp = page_table[i].last_access_timestamp;
					}
				}
				int freed_frame_number = page_table[chosen_pte_idx].frame_number;
				page_table[chosen_pte_idx].is_valid = 0;
				page_table[chosen_pte_idx].frame_number = -1;
				page_table[chosen_pte_idx].arrival_timestamp = -1;
				page_table[chosen_pte_idx].last_access_timestamp = -1;
				page_table[chosen_pte_idx].reference_count = -1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].frame_number = freed_frame_number;
				page_table[page_number].is_valid = 1;
			}
		}
		current_timestamp++;
	}
	return page_fault_cnt;
}
	
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == 1) {
		if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
		else page_table[page_number].reference_count++;
		
		page_table[page_number].last_access_timestamp = current_timestamp;
		
		return page_table[page_number].frame_number;
	}
	else {
		if (*frame_cnt > 0) {
			page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
			frame_pool[*frame_cnt-1] = -1;
			*frame_cnt -= 1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].is_valid = 1;
			
			return page_table[page_number].frame_number;
		}
		else if (*frame_cnt == 0) {
			int least_ref_count = -1;
			int chosen_pte_idx = -1;
			for (int i =0; i < *table_cnt; i++) {
				if(page_table[i].is_valid == 1 && 
				  (least_ref_count == -1 || page_table[i].reference_count <= least_ref_count))  {
					chosen_pte_idx = i;
					least_ref_count = page_table[i].reference_count;
				}
			}
			
			int freed_frame_number = page_table[chosen_pte_idx].frame_number;
			
			page_table[chosen_pte_idx].is_valid = 0;
			page_table[chosen_pte_idx].frame_number = -1;
			page_table[chosen_pte_idx].arrival_timestamp = -1;
			page_table[chosen_pte_idx].last_access_timestamp = -1;
			page_table[chosen_pte_idx].reference_count = -1;
			
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			page_table[page_number].frame_number = freed_frame_number;
			page_table[page_number].is_valid = 1;
			
			return page_table[page_number].frame_number;
		}
	}
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
	int current_timestamp = 1, page_fault_cnt = 0;
	for (int ref = 0; ref < reference_cnt; ref++) {
		int page_number = refrence_string[ref];
		
		if (page_table[page_number].is_valid == 1) {
			if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
			else page_table[page_number].reference_count++;
			
			page_table[page_number].last_access_timestamp = current_timestamp;
		}
		else {
			page_fault_cnt++;
			if (frame_cnt > 0) {
				page_table[page_number].frame_number = frame_pool[frame_cnt-1];
				frame_pool[frame_cnt-1] = -1;
				frame_cnt -= 1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].is_valid = 1;
			}
			else if (frame_cnt == 0) {
				int least_ref_count = -1;
				int chosen_pte_idx = -1;
				for (int i =0; i < table_cnt; i++) {
					if(page_table[i].is_valid == 1 && 
					(least_ref_count == -1 || page_table[i].reference_count <= least_ref_count))  {
						chosen_pte_idx = i;
						least_ref_count = page_table[i].reference_count;
					}
				}
				
				int freed_frame_number = page_table[chosen_pte_idx].frame_number;
				
				page_table[chosen_pte_idx].is_valid = 0;
				page_table[chosen_pte_idx].frame_number = -1;
				page_table[chosen_pte_idx].arrival_timestamp = -1;
				page_table[chosen_pte_idx].last_access_timestamp = -1;
				page_table[chosen_pte_idx].reference_count = -1;
				
				page_table[page_number].arrival_timestamp = current_timestamp;
				page_table[page_number].last_access_timestamp = current_timestamp;
				if (page_table[page_number].reference_count == -1) page_table[page_number].reference_count = 1;
				else page_table[page_number].reference_count++;
				page_table[page_number].frame_number = freed_frame_number;
				page_table[page_number].is_valid = 1;
			}
		}
		current_timestamp++;
	}
	return page_fault_cnt;
}