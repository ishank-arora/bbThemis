#define _GNU_SOURCE 
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <asm/unistd.h>
#include <inttypes.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/syscall.h>
#include <dirent.h>

#define FSIZE (1ULL*1024*1024*1024)
#define IOSIZE (128*1024*1024)
#define BLKSIZE (4*1024)
#define CLSIZE (64)

char *addr;
struct timeval start,end;


void performExperiment(int operation, uint32_t datasize, char *buf, int fd) {

	uint32_t num_blks;
	unsigned long long offset = 0;
	unsigned int r = 0;
    int write_size = 0;
	int i = 0, j = 0;
	int num_clines = 0;
    struct stat sbuf;

    fstat(fd, &sbuf);

	num_blks = IOSIZE / datasize;

	gettimeofday(&start, NULL);

	r = 0;
	offset = 0;
	i = 0;

    if(operation == 0) {
        for(i = 0; i < num_blks; i++)
        {
            if(pread(fd, buf, datasize, offset) != datasize) {
                perror("pread failed\n");
                printf("%s: offset = %llu\n", __func__, offset);
                exit(-1);
            }
            offset += datasize;
        }
    } else if(operation == 1) {
        for(i = 0; i < num_blks; i++)
        {
            if(write_size = (pwrite(fd, buf, datasize, offset)) != datasize) {
                perror("pwrite failed\n");
                printf("%s: Returned value = %d. Offset = %llu\n", __func__, write_size, offset);
                exit(-1);
            }

            offset += datasize;
        }
    }

	gettimeofday(&end, NULL);
}

void parseArgs(char *argv[], int *operation) {

    char datasizestr[100];
    char size_granularity;

    if ((strcmp(argv[1], "pwrite") == 0)) {
        *operation = 1;
    }

    else if ((strcmp(argv[1], "pread") == 0)) {
        *operation = 0;
    }

}

/* Example usage: ./a.out read seq 4096 */

int main(int argc, char *argv[]) {

	int fd;
    time_t curtime;
	unsigned int i,t;
	char *buf;
	uint32_t num_iter;
	int operation = 2;
    char file_name[256];
    float numops = 0.0;

    uint32_t datasize = 4096;

	if(argc < 3) {
	  	perror("Usage: ./bench.out <pread/pwrite>");
		exit(-1);
	}


	parseArgs(argv, &operation);

    numops = IOSIZE/datasize;

	buf = (char *)malloc(sizeof(char)*datasize);

    fd = open("test.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf("%s: Open failed. Err = %s\n", __func__, strerror(errno));
        exit(0);
    }

	for(i = 0; i < datasize; i++)
		buf[i] = 'R';

	performExperiment(operation, datasize, buf, fd);

	//pause();

	t =(end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);

    char * op = operation == 1 ? "pwrite" : "pread"; 
    printf("\nThroughput of %s: %0.2fM ops/sec\n", op, (numops/t));
    printf("\nLatency of %s: %6.2lf us\n", op, 1.0*(t/numops));



	close(fd);

	return 0;
}