#include<stdio.h>

int main(int argc, char** argv){
    int year;
    sscanf(argv[1], "%d", &year);

    if(year%400 == 0 || (year%4 == 0 && year%100 != 0)){
        printf("%d was a leap year",year);
    }
    else
    printf("%d was not a leap year",year);
    return 0;
}