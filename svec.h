/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#ifndef SVEC_H
#define SVEC_H
//#include <pthread.h>

typedef struct svec {
    int size;
    int cap;
    char** data;
    int* cdata;
    //pthread_mutex_t lock;
} svec;

svec* make_svec();
void  free_svec(svec* sv);

char* svec_get(svec* sv, int ii);
void  svec_put(svec* sv, int ii, char* item);

void svec_push_back(svec* sv, char* item);
void svec_swap(svec* sv, int ii, int jj);

void svec_print(svec *sv);

float keygen(svec *xs, int n);

void chomp(char *line);

#endif
