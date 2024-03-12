#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define FILENAME "shared.dat"
#define COUNT 100000

void create_file(void)
{
    long long int x = 0;

    int fd;

    if ((fd = open(FILENAME, O_CREAT|O_TRUNC|O_WRONLY, 0644)) == -1) {
        perror("open");
        exit(1);
    }

    write(fd, &x, sizeof x);

    close(fd);
}

void increment_data_a_lot(long long int *data)
{
    for (int i = 0; i < COUNT; i++) {
        long long int t = *data;
        t++;
        *data = t;
    }
}

int main(void)
{
    int fd;
    long long int *data;
    int data_len = sizeof *data;

    create_file();

    // Open the FILENAME for O_RDWR int variable `fd`
    fd = open(FILENAME, O_RDWR);

    // Map it with mmap:
    data = mmap(NULL, data_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Close the file
    close(fd);

    // Fork a child process:
    if (fork() == 0) {
        // Child calls increment_data_a_lot()
        increment_data_a_lot(data);
        // Then munmaps the file:
        munmap(data, data_len);
        // Then exits
        exit(0);
    }

    // Parent calls increment_data_a_lot()
    increment_data_a_lot(data);
    // Then waits for the child to complete
    wait(NULL);
    // Then prints the value in *data
    printf("%lld\n", *data);
    // Then munmaps the file
    munmap(data, data_len);
    // Then exits
    exit(0);

    //  Both the parent and the child increment the value in the file
    //  100000 times. What should the result be? What is the result?
    //  If it's not 100000*2, why isn't it? How could we fix it?
    
}

