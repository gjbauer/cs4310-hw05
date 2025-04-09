#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "svec.h"

void
insertionSort(svec* xs)
{
    for (int i=1; i<xs->size; i++) {
	    int j = i - 1;
	    int key = keygen(xs, i);

	    while(j >= 0 && keygen(xs, j) > key) {
		    svec_swap(xs, j+1, j);
		    j--;
	    }
    }
    return;
}

void
insertionSortD(svec *xs)
{
	for (int i=1; i<xs->size; i++) {
		int j = i - 1;
		int key = keygen(xs, i);

		while(j >=0 && keygen(xs, j) < key) {
			svec_swap(xs, j+1, j);
			j--;
		}
	}
	return;
}

void
chomp(char* text)
{
    int j=strlen(text);
    char arr[j];
    strncpy(arr, text, j);
    strncpy(text, arr, j);
    if (text[0]==' ') {
        int i;
        for(i=0; text[i]==' '; i++);
        for(int j=0; text[j]!=0; j++, i++) text[j]=text[i];
    }
    for(int ii = 0; text[ii]; ++ii) {
        if (!((text[ii]>=97)&&(text[ii]<=122)||(text[ii]>=65)&&(text[ii]<=90)||(text[ii]==32))) {
            text[ii] = 0;
        }
    }
}

int get(char *buf, int n, const char *src, unsigned int pos, struct stat *fs)
{
	int i;
	for (i=0; i<n; i++) buf[i]=0;
	for (i=0; i<n && pos<fs->st_size && src[pos]!='\n'; i++, pos++) buf[i]=src[pos];
	return pos+1;
}

void loop(svec *sv, const char *src, struct stat *fs)
{
	char temp[128];
	unsigned int pos=0;
	while (1) {
		pos = get(temp, 128, src, pos, fs);
		if (pos>=fs->st_size)
		{
	        	break;
	        }

        	chomp(temp);
       		svec_push_back(sv, temp);
    	}
	return;
}

void loop2(svec *sv, FILE *fh)
{
	char temp[128];
	while (1) {
		char *line;
		line = fgets(temp, 128, fh);
		if (!line)
		{
	        	break;
	        }

        	chomp(line);
       		svec_push_back(sv, line);
    	}
	return;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n  %s input-file\n", argv[0]);
        return 1;
    }

    svec* xs = make_svec();
    struct stat *fs = malloc(sizeof(struct stat));;
    const char *map;

    if (strcmp(argv[1], "xmas.txt")!=0) {
        FILE* fh;
        fh = fopen(argv[1], "r");
        loop2(xs, fh);
	fclose(fh);
    }

    else {
    	int fd;

    	fd = open(argv[1], O_RDONLY);

    	fstat(fd, fs);

    	map = mmap(0, fs->st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    	loop(xs, map, fs);
    }


    insertionSort(xs);


    svec_print(xs);

    //free_svec(xs);
    return 0;
}
