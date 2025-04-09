/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv  = malloc(sizeof(svec));
    sv->data  = malloc(2 * sizeof(char*));
    sv->cdata =	calloc(2, sizeof(int));
    sv->size  = 0;
    sv->cap   = 2;
    return sv;
}

float keygen(svec* xs, int n)
{
	float k=0;
	for (int i=0; xs->data[n][i]!=0; i++)
	{
		float j = pow(32, -i+4);
		j*=xs->data[n][i];
		k+=j;
	}
	return k;
}

void
free_svec(svec* sv)
{
    for (int ii=sv->size-1; ii >=0; ii--) free(sv->data[ii]);
    free(sv->data);
    free(sv->cdata);
    free(sv);
    //pthread_mutex_destroy(&sv->lock);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    //assert(ii >= 0 && ii < sv->cap);
    //
    int k;
    for(k=0; item[k]!=0 && item[k]!=' '; k++);
    sv->data[ii] = calloc(k+1, sizeof(char));
    for(int i=0; item[i]!=0; i++) sv->data[ii][i]=item[i];
    sv->cdata[ii]=1;
    return;
}

void
svec_push_back(svec* sv, char* item)
{
    if (sv->size==sv->cap-1) {
	sv->cap*=2;
	char **data = malloc(2*sv->cap*sizeof(char*));
	int *cdata = calloc(2*sv->cap, sizeof(int));
	for(int ii = sv->size-1; ii>=0; ii--) data[ii]=sv->data[ii];
	for(int ii = sv->size-1; ii>=0; ii--) cdata[ii]=sv->cdata[ii];
	free(sv->data);
	free(sv->cdata);
	sv->data=data;
	sv->cdata=cdata;
    }
    char *word;
    int i;
    for (i=0; item[i]!=0 && item[i]!= ' '; i++);
    word = calloc(i, sizeof(char));
    for (i=0; item[i]!=0 && item[i]!= ' '; i++) {
        if (64<item[i] && item[i]<91)
            word[i]=item[i]+32;
        else
            word[i]=item[i];
    }
    if (sv->size==0) {
        int k;
        for(k=0; item[k]!=0 && item[k]!=' '; k++);
        if (k>1)
            svec_put(sv, sv->size++, word);
    }
    else {
	int ii, i=0;
    	for (ii=0; ii<sv->size; ii++) {
		if (strcmp(sv->data[ii], word)==0) sv->cdata[ii]++, i++;
    	}
	if (i==0)
		svec_put(sv, sv->size++, word);
    }
    if (item[i+1]!=0) {
	    if (item[i+1]==' ')
                svec_push_back(sv, item+i+2);
            else
	        svec_push_back(sv, item+i+1);
    }
    return;
}

void
svec_swap(svec* sv, int ii, int jj)
{
	int i, j, size;
	char aa[128];
	for(i=0; sv->data[ii][i]!=0; i++) aa[i] = sv->data[ii][i];
	aa[i]=0;
	j = sv->cdata[ii];
	free(sv->data[ii]);
	for(size=0; sv->data[jj][size]!=0; size++);
	sv->data[ii] = calloc(size+1, sizeof(char));
	for(i=0; sv->data[jj][i]!=0; i++) sv->data[ii][i]=sv->data[jj][i];
	sv->data[ii][i]=0;
	sv->cdata[ii]=sv->cdata[jj];
	free(sv->data[jj]);
	for(size=0; aa[size]!=0; size++);
	sv->data[jj] = calloc(size+1, sizeof(char));
	for(i=0; aa[i]!=0; i++) sv->data[jj][i]=aa[i];
	sv->data[jj][i]=0;
	sv->cdata[jj]=j;
	return;
}

void
svec_print(svec *sv)
{
	for(int ii=0; ii<sv->size; ii++) {
		char *line = svec_get(sv, ii);
		if (sv->cdata[ii]>1) {
		printf("%d\t%s\n",sv->cdata[ii], line);
		}
	}
	return;
}
