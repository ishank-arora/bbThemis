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

//fallocates 4096 bytes each iteration

#define FSIZE (1ULL*1024*1024*1024)
#define IOSIZE (128*1024*1024)
#define BLKSIZE (4*1024)
#define CLSIZE (64)

char *addr;
struct timeval start,end;


/* Example usage: ./a.out read seq 4096 */
void parseArgs(char *argv[], uint32_t *num_iter) {

    char datasizestr[100];

    strcpy(datasizestr, argv[1]);

    *num_iter = atoi(datasizestr);
} 

int main(int argc, char *argv[]) {

    DIR* d;
	int fd;
    int err;
    time_t curtime;
	unsigned int i,t;
	char *buf;
	uint32_t num_iter;
	int operation = 2;
    char file_name[256];
    float numops = 0.0;

    uint32_t datasize = 4096;

	if(argc < 2) {
	  	perror("Usage: ./bench.out <num_iter>");
		exit(-1);
	}


	parseArgs(argv, &num_iter);

    //d = opendir("home1/06333/aroraish/bbThemis");
    //int d_fd = dirfd(d);

    struct stat sbuf;
   


    gettimeofday(&start, NULL);
     for(int i = 0; i < num_iter; i++){
        err = lstat("test_link.txt", &sbuf);
        if(err < 0){
            perror("Error in lxstat");
            exit(-1);
        }
    }
    gettimeofday(&end, NULL);

	t =(end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);

    printf("\nLatency of lxstat: %6.2lf us\n", 1.0*(t/num_iter));

	return 0;
}
