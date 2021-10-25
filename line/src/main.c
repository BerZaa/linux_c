/*
 * @Author: BerZaa 
 * @Date: 2021-10-25 11:05:36 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-25 13:04:01
 */
#include "stdio.h"
#include "stdlib.h"
#include "sqlist.h"

int main(int argc, char *argv[]) {
    sqlist   *sqlist_inst, *sqlist_inst2;
    data_type arr[] = {12, 456, 47, 35};        
    data_type arr2[] = {95, 42, 47}; 
    int       status; 

    sqlist_inst = sqlist_create();       
    if(sqlist_inst == NULL) {             
        fprintf(stderr, "sqlist_create() failed\n");  
        exit(1);
    }

    for(int index = 0; index < sizeof(arr) / sizeof(*arr); ++index) {
        status = sqlist_insert(sqlist_inst, 0, (arr + index));
        if(status != 0) {
            fprintf(stderr, "sqlist_insert() failed\n");
            exit(2);
        }
    }

    printf("sqlist data: ");
    sqlist_display(sqlist_inst);

    status = sqlist_delete(sqlist_inst, 0);
    if(status != 0) {
            fprintf(stderr, "sqlist_insert() failed\n");
            exit(3);
    }
    printf("sqlist data: ");
    sqlist_display(sqlist_inst);

    printf("sqlist empty: %d\n", sqlist_isempty(sqlist_inst) == -1 ? 0 : 1);

    printf("47 data index is %d\n", sqlist_find(sqlist_inst, &arr[1]));

    sqlist_inst2 = sqlist_create();       
    if(sqlist_inst2 == NULL) {             
        fprintf(stderr, "sqlist_create() failed\n");  
        exit(1);
    }

    for(int index = 0; index < sizeof(arr2) / sizeof(*arr2); ++index) {
        status = sqlist_insert(sqlist_inst2, 0, (arr2 + index));
        if(status != 0) {
            fprintf(stderr, "sqlist_insert() failed\n");
            exit(2);
        }
    }

    printf("\n");

    printf("sqlist2 data: ");
    sqlist_display(sqlist_inst2);
    printf("data len: %d\n", sqlist_getlen(sqlist_inst2));

    printf("sqlist data: ");
    sqlist_display(sqlist_inst);
    printf("data len: %d\n", sqlist_getlen(sqlist_inst));

    status = sqlist_union(sqlist_inst, sqlist_inst2);
    if(status == -1) {
            fprintf(stderr, "sqlist_union() failed\n");
            exit(4);
    }

    printf("sqlist data after union: ");
    sqlist_display(sqlist_inst);
    printf("data len: %d\n", sqlist_getlen(sqlist_inst));

    return 0;
}