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
#include <sys/types.h>
#include <dirent.h>

#define FSIZE (1ULL*1024*1024*1024)
#define IOSIZE (128*1024*1024)
#define BLKSIZE (4*1024)
#define CLSIZE (64)

char *addr;
struct timeval start,end;
int precomputed_rand[FSIZE/CLSIZE];

void precompute_rand(unsigned long datasize) {

	int i = 0;
	int num_blocks = IOSIZE / datasize;

	for (i = 0; i < num_blocks; i++)
		precomputed_rand[i] = rand() % (IOSIZE - datasize);
}


void performExperiment(int operation, uint32_t datasize, int seq, char *buf, int fd) {

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

    DIR * d = NULL;

    char * file_name = "test.txt";

    if(operation == 0) {
        for(i = 0; i < num_blks; i++)
        {
            if(pread(fd, buf, datasize, offset) != datasize) {
                perror("pread failed\n");
                printf("%s: offset = %llu\n", __func__, offset);
                exit(-1);
            }
            offset += datasize;
            r = precomputed_rand[i];

            if(seq == 0)
                offset = r;
        }
    } 
    else if(operation == 1) {
        for(i = 0; i < num_blks; i++)
        {
            if(write_size = (pwrite(fd, buf, datasize, offset)) != datasize) {
                perror("pwrite failed\n");
                printf("%s: Returned value = %d. Offset = %llu\n", __func__, write_size, offset);
                exit(-1);
            }

            offset += datasize;
            r = precomputed_rand[i];

            if(seq == 0)
                offset = r;
        }
    }
    else if(operation == 2) {
        for(i = 0; i < num_blks; i++)
        {
            if(read(fd, buf, datasize) != datasize) {
                perror("read failed\n");
                printf("%s: offset = %llu\n", __func__, offset);
                exit(-1);
            }
            offset += datasize;
            r = precomputed_rand[i];

            if(seq == 0)
                offset = r;
        }
    } 
    else if(operation == 3) {
        for(i = 0; i < num_blks; i++)
        {
            if(write_size = (write(fd, buf, datasize)) != datasize) {
                perror("write failed\n");
                printf("%s: Returned value = %d. Offset = %llu\n", __func__, write_size, offset);
                exit(-1);
            }

            offset += datasize;
            r = precomputed_rand[i];

            if(seq == 0)
                offset = r;
        }
    } 
    else if(operation == 4) {
        int access_err = 0;
        if(access_err = (access(file_name, F_OK)) < 0) {
            perror("access failed\n");
            printf("%s: Returned value = %d.\n", __func__, access_err);
            exit(-1);
        }
    }
    else if(operation == 5) {
        int open_err = 0;
        if(open_err = (open(file_name, O_RDONLY)) < 0) {
            perror("open failed\n");
            printf("%s: Returned value = %d.\n", __func__, open_err);
            exit(-1);
        }
        fd = open_err;
    }
    else if(operation == 6) {
        DIR * d = opendir("home1/06333/aroraish/bbThemis");
        int d_fd = dirfd(d);
        int open_err = 0;
        if(open_err = (openat(d_fd, file_name, O_RDONLY)) < 0) {
            perror("openat failed\n");
            printf("%s: Returned value = %d.\n", __func__, open_err);
            exit(-1);
        }
        fd = open_err;
    }
    else if(operation == 7) {
        d= opendir("home1/06333/aroraish/bbThemis");
        if(errno < 0) {
            perror("opendir failed\n");
            printf("%s: Returned value = %d.\n", __func__, errno);
            exit(-1);
        }
    }

	gettimeofday(&end, NULL);
    close(fd);
    if(d != NULL)
        closedir(d);
}

void parseArgs(char *argv[], int *operation, int *seq, uint32_t *datasize) {

    char datasizestr[100];
    char size_granularity;

    if ((strcmp(argv[1], "pread") == 0)) {
        *operation = 0;
    }

    else if ((strcmp(argv[1], "pwrite") == 0)) {
        *operation = 1;
    }

    else if ((strcmp(argv[1], "read") == 0)) {
        *operation = 2;
    }

    else if ((strcmp(argv[1], "write") == 0)) {
        *operation = 3;
    }

    else if ((strcmp(argv[1], "access") == 0)) {
        *operation = 4;
    }

    else if ((strcmp(argv[1], "open") == 0)) {
        *operation = 5;
    }
    else if ((strcmp(argv[1], "openat") == 0)) {
        *operation = 6;
    }
    else if ((strcmp(argv[1], "opendir") == 0)) {
        *operation = 7;
    }
    else if ((strcmp(argv[1], "close") == 0)) {
        *operation = 8;
    }
    else if ((strcmp(argv[1], "closedir") == 0)) {
        *operation = 9;
    }
    else if ((strcmp(argv[1], "fallocate") == 0)) {
        *operation = 10;
    }
    else if ((strcmp(argv[1], "faccessat") == 0)) {
        *operation = 11;
    }
    else if ((strcmp(argv[1], "fchmod") == 0)) {
        *operation = 12;
    }
    else if ((strcmp(argv[1], "fchmodat") == 0)) {
        *operation = 13;
    }
    else if ((strcmp(argv[1], "ftruncate") == 0)) {
        *operation = 14;
    }
    else if ((strcmp(argv[1], "fxstat") == 0)) {
        *operation = 15;
    }
    else if ((strcmp(argv[1], "futimens") == 0)) {
        *operation = 16;
    }
    else if ((strcmp(argv[1], "fdopendir") == 0)) {
        *operation = 17;
    }
    else if ((strcmp(argv[1], "lseek") == 0)) {
        *operation = 18;
    }
    else if ((strcmp(argv[1], "lxstat") == 0)) {
        *operation = 19;
    }
    else if ((strcmp(argv[1], "mkdir") == 0)) {
        *operation = 20;
    }
    else if ((strcmp(argv[1], "mkdirat") == 0)) {
        *operation = 21;
    }
    else if ((strcmp(argv[1], "readdir") == 0)) {
        *operation = 22;
    }
    else if ((strcmp(argv[1], "rmdir") == 0)) {
        *operation = 23;
    }
    else if ((strcmp(argv[1], "truncate") == 0)) {
        *operation = 24;
    }
    else if ((strcmp(argv[1], "unlink") == 0)) {
        *operation = 25;
    }
    else if ((strcmp(argv[1], "unlinkat") == 0)) {
        *operation = 26;
    }
    else if ((strcmp(argv[1], "utimensat") == 0)) {
        *operation = 27;
    }
    else if ((strcmp(argv[1], "utimes") == 0)) {
        *operation = 28;
    }
    else if ((strcmp(argv[1], "xstat") == 0)) {
        *operation = 29;
    }


  strcpy(datasizestr, argv[3]);

  *datasize = atoi(datasizestr);

  printf("The arguments passed are: operation = %d, seq = %d, rw_size = %u\n", *operation, *seq, *datasize);

}

/* Example usage: ./a.out read seq 4096 */

int main(int argc, char *argv[]) {

	int fd;
    time_t curtime;
	unsigned int i,t;
	char *buf;
	int seq;
	uint32_t datasize;
	int operation = 2;
    char file_name[256];
    float numops = 0.0;

	if(argc < 4) {
	  	perror("Usage: ./a.out <read/write> <seq/rand> <write_size>");
		exit(-1);
	}


	parseArgs(argv, &operation, &seq, &datasize);

    numops = IOSIZE/datasize;

	buf = (char *)malloc(sizeof(char)*datasize);

    fd = open("test.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf("%s: Open failed. Err = %s\n", __func__, strerror(errno));
        exit(0);
    }

	srand(5);
	precompute_rand(datasize);

	for(i = 0; i < datasize; i++)
		buf[i] = 'R';

	performExperiment(operation, datasize, seq, buf, fd);

	//pause();

	t =(end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);


    printf("\nOperation num: %d takes %f seconds\n", operation, t);

	close(fd);

	return 0;
}