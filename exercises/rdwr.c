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

    // TODO
    // 
    // Open the FILENAME for O_RDWR int variable `fd`
    // Map it with mmap:
    //
    //  data = mmap(NULL, data_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    //
    // Close the file
    // Fork a child process:
    //    Child calls increment_data_a_lot()
    //    Then munmaps the file:
    //        munmap(data, data_len);
    //    Then exits
    //
    // Parent calls increment_data_a_lot()
    // Then waits for the child to complete
    // Then prints the value in *data
    // Then munmaps the file
    // Then exits
    //
    //  Both the parent and the child increment the value in the file
    //  100000 times. What should the result be? What is the result?
    //  If it's not 100000*2, why isn't it? How could we fix it?
    (void)data_len;(void)fd;
}

