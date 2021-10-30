/*
 * @Author: BerZaa 
 * @Date: 2021-10-24 22:37:06 
 * @Last Modified by: BerZaa
 * @Last Modified time: 2021-10-25 12:50:08
 */
#ifndef SQLIST_H__
#define SQLIST_H__

#define DATA_SIZE 1024

typedef int data_type;

typedef struct NODE_ST{
    data_type data[DATA_SIZE];
    int last;
} sqlist;  

sqlist *sqlist_create  (void);
int     sqlist_insert  (sqlist *sqlist_inst, int index, data_type *data_ptr);
int     sqlist_delete  (sqlist *sqlist_inst, int index);
int     sqlist_find    (sqlist *sqlist_inst, data_type *data_ptr);
void    sqlist_display (sqlist *sqlist_inst);
int     sqlist_destory (sqlist *sqlist_inst);
int     sqlist_isempty (sqlist *sqlist_inst);
int     sqlist_setempty(sqlist *sqlist_inst);
int     sqlist_getlen  (sqlist *sqlist_inst);
int     sqlist_union   (sqlist *sqlist_inst1, sqlist *sqlist_inst2);

#endif