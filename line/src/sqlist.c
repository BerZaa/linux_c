/*
 * @Author: BerZaa 
 * @Date: 2021-10-25 12:50:01 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-25 13:05:58
 */
#include "stdio.h"
#include "stdlib.h"
#include "sqlist.h"

sqlist *sqlist_create(void) {
    sqlist *memory;

    memory = malloc(sizeof(*memory));
    if(memory == NULL) {
        return NULL;
    }
    else {
        memory->last = -1;
        return memory;
    }
}

int sqlist_insert(sqlist *sqlist_inst, int index, data_type *data_ptr) {
    int temp;

    if(sqlist_inst->last == DATA_SIZE - 1) {
        return -1;
    }
    if(index < 0 || index > sqlist_inst->last + 1) {
        return -1;
    }
    for(temp = sqlist_inst->last; index <= temp; --temp) 
        sqlist_inst->data[temp + 1] = sqlist_inst->data[temp];
    sqlist_inst->data[index] = *data_ptr;
    ++sqlist_inst->last;
    return 0;
}

int sqlist_delete(sqlist *sqlist_inst, int index) {
    int temp;

    if(index < 0 || index > sqlist_inst->last) {
        return -1;
    }
    for(temp = index + 1; temp <= sqlist_inst->last; ++temp) {
        sqlist_inst->data[temp - 1] = sqlist_inst->data[temp];
    }
    --sqlist_inst->last;
    return 0;
}

int sqlist_find(sqlist *sqlist_inst, data_type *data_ptr) {
    int index;
    if(sqlist_isempty(sqlist_inst) == 0) {
        return -1;
    }
    else {
        for(index = 0; index <= sqlist_inst->last; ++index) {
            if(sqlist_inst->data[index] == *data_ptr) {
                return index;
            }
        }
        return -1;
    }
}

void sqlist_display(sqlist *sqlist_inst) {
    if(sqlist_inst->last == -1) {
        return ;
    }
    for(int index = 0; index <= sqlist_inst->last; ++index) {
        printf("%d ", sqlist_inst->data[index]);
    }
    printf("\n");
    return ;
}

int sqlist_destory(sqlist *sqlist_inst) {
    free(sqlist_inst);
    return 0;
}

int sqlist_isempty(sqlist *sqlist_inst) {
    if(sqlist_inst->last == -1) {
        return 0;
    }
    else 
        return -1;
}

int sqlist_setempty(sqlist *sqlist_inst) {
    sqlist_inst->last = -1;
    return 0;
}

int sqlist_getlen(sqlist *sqlist_inst) {
    return sqlist_inst->last + 1;
}

int sqlist_union(sqlist *sqlist_inst1, sqlist *sqlist_inst2) {
    int index = 0;

    if(sqlist_isempty(sqlist_inst1) == 0) {
        return -1;
    }
    for(index = 0;index <= sqlist_inst2->last; ++index) {
        if(sqlist_find(sqlist_inst1, &sqlist_inst2->data[index]) == -1) {
            sqlist_insert(sqlist_inst1, 0, &sqlist_inst2->data[index]);
        }
    }
    
    return 0;
}
