#include "oslabs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_null_RCB(struct RCB current_request) {
if((current_request.address == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) && (current_request.process_id == 0) && (current_request.request_id == 0))
return true;
else
return false;
}


struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
if (is_null_RCB(current_request)){
//struct RCB null_RCB = {0,0,0,0,0};
return new_request;
}
else {
if (*queue_cnt <= QUEUEMAX){
request_queue[*queue_cnt] = new_request;
*queue_cnt += 1;
return current_request;
}
else
return current_request;
}
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
if (*queue_cnt <= 0){
struct RCB null_RCB = {0,0,0,0,0};
return null_RCB;
}
else{
struct RCB next_RCB;
int temp_index = 0;
int temp_arrival = request_queue[0].arrival_timestamp;
for (int index = 1; index < *queue_cnt; index++){ //looking for the smallest time_arrival value
if (temp_arrival > request_queue[index].arrival_timestamp){
temp_arrival = request_queue[index].arrival_timestamp;
temp_index = index;
}
}
next_RCB = request_queue[temp_index];
for (int index = temp_index; index < *queue_cnt ; index++){
request_queue[index] = request_queue[index+1];
}
*queue_cnt -= 1;
return next_RCB;
}
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
if (is_null_RCB(current_request)){
return new_request;
}
else {
if (*queue_cnt <= QUEUEMAX){
request_queue[*queue_cnt] = new_request;
*queue_cnt += 1;
return current_request;
}
else
return current_request;
}
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
if (*queue_cnt <= 0){
struct RCB null_RCB = {0,0,0,0,0};
return null_RCB;
}
else{
struct RCB next_RCB;
int temp_index = 0, temp_cylinder = 0, temp_arrivel = 0;
temp_cylinder = abs(current_cylinder - request_queue[0].cylinder);
temp_arrivel = request_queue[0].arrival_timestamp;
for (int index = 1; index < *queue_cnt; index++){ //looking for the closest cylinder gap
if (temp_cylinder == abs(current_cylinder - request_queue[index].cylinder)){
if (temp_arrivel > request_queue[index].arrival_timestamp){
temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
temp_arrivel = request_queue[index].arrival_timestamp;
temp_index = index;
}
}
else if (temp_cylinder > abs(current_cylinder - request_queue[index].cylinder)){
temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
temp_arrivel = request_queue[index].arrival_timestamp;
temp_index = index;
}
}
next_RCB = request_queue[temp_index];
for (int index = temp_index; index < *queue_cnt ; index++){
request_queue[index] = request_queue[index+1];
}
*queue_cnt -= 1;
return next_RCB;
}
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
if (is_null_RCB(current_request)){
return new_request;
}
request_queue[*queue_cnt] = new_request;
*queue_cnt = *queue_cnt + 1;
return current_request;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction){
if (*queue_cnt <= 0){
struct RCB null_RCB = {0,0,0,0,0};
return null_RCB;
}
else{
struct RCB next_RCB;
int temp_index = 0, temp_cylinder = 0, temp_arrivel = 0;
bool flag_equel = false, flag_positive_cylinder = false, first = false, direction_first = false, opposite_direction_first = false;
for (int index = 0; index < *queue_cnt; index++){ //looking for the closest cylinder gap
if (current_cylinder == request_queue[index].cylinder){
if (first == false){
temp_arrivel = request_queue[index].arrival_timestamp;
temp_index = index;
first = true;
flag_equel = true;
}
else if (temp_arrivel > request_queue[index].arrival_timestamp){
temp_arrivel = request_queue[index].arrival_timestamp;
temp_index = index;
}
}
else if ((scan_direction == 1) && (flag_equel == false)){
if(request_queue[index].cylinder - current_cylinder > 0){ //looking for larger cylinder
if (direction_first == false){
temp_cylinder = request_queue[index].cylinder - current_cylinder;
temp_index = index;
flag_positive_cylinder = true;
direction_first = true;
}
else if (temp_cylinder > request_queue[index].cylinder - current_cylinder){
temp_cylinder = request_queue[index].cylinder - current_cylinder;
temp_index = index;
}
}
else if ((flag_positive_cylinder == false) && (current_cylinder - request_queue[index].cylinder > 0)){
if (opposite_direction_first == false){
temp_cylinder = abs(request_queue[index].cylinder - current_cylinder);
temp_index = index;
opposite_direction_first = true;
}
else if(temp_cylinder > abs(request_queue[index].cylinder - current_cylinder)){
temp_cylinder = abs(request_queue[index].cylinder - current_cylinder);
temp_index = index;
}
}
}
else if ((scan_direction == 0) && (flag_equel == false)){
if(current_cylinder - request_queue[index].cylinder > 0){ //looking for larger cylinder
if (direction_first == false){
temp_cylinder = current_cylinder - request_queue[index].cylinder;
temp_index = index;
flag_positive_cylinder = true;
direction_first = true;
}
else if (temp_cylinder > current_cylinder - request_queue[index].cylinder){
temp_cylinder = current_cylinder - request_queue[index].cylinder;
temp_index = index;
}
}
else if ((flag_positive_cylinder == false) && (current_cylinder - request_queue[index].cylinder < 0)){
if (opposite_direction_first == false){
temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
temp_index = index;
opposite_direction_first = true;
}
else if(temp_cylinder > abs(request_queue[index].cylinder - current_cylinder)){
temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
temp_index = index;
}
}
}
}
next_RCB = request_queue[temp_index];
for (int index = temp_index; index < *queue_cnt ; index++){
request_queue[index] = request_queue[index+1];
}
*queue_cnt -= 1;
return next_RCB;
}
}