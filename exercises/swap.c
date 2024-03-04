#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define RAM_SIZE 65536  // 64 KB
#define PAGE_SIZE 256 // bytes

char ram[RAM_SIZE];  // Simulated RAM

#define SWAP_FILE "swapfile.dat"
#define BLOCK_SIZE PAGE_SIZE

/*
 * Returns a pointer to the start of a physical page.
 */
char *ppage_get_start_addr(int ppage_num)
{
    return ram + ppage_num * PAGE_SIZE;
}

/*
 * Given a block number, returns the byte offset where that block starts
 * in the swap file.
 */
int get_block_offset(int block_num)
{
    return block_num * BLOCK_SIZE;
}

/*
 * Moves the file pointer to the first byte of a given block number.
 *
 * Returns the result of the lseek() syscall.
 */
int block_seek(int fd, int block_num)
{
    return lseek(fd, get_block_offset(block_num), SEEK_SET);
}

/*
 * Writes the given data buffer to the given block_num.
 *
 * Returns the result of the write() syscall.
 */
int block_write(int fd, int block_num, char *data)
{
    return write(fd, data, get_block_offset(block_num));
}

/*
 * Reads the given block_num into the data buffer.
 *
 * Returns the result of the read() syscall.
 */
int block_read(int fd, int block_num, char *data)
{
    return read(fd, data, get_block_offset(block_num));
}

/*
 * Opens the swap file for reading, creating (with mode 0600) and
 * truncating it if necessary.
 *
 * Returns the result of the open() syscall.
 */
int swap_open(void)
{
    return open(SWAP_FILE, O_RDWR | O_CREAT | O_TRUNC, 0600);
}

/*
 * Closes the swap file.
 *
 * Returns the result of the close() syscall.
 */
int swap_close(int fd)
{
    return close(fd);
}

/*
 * Swap a physical page of memory out to a swap page on disk.
 */
void swap_out(int fd, int ppage_num, int block_num)
{
    printf("SWAP OUT: mem %d -> %d disk\n", ppage_num, block_num);

    // read the page from memory
    char *ppage_start = ppage_get_start_addr(ppage_num);

    // write the page to disk
    block_seek(fd, block_num);
    block_write(fd, block_num, ppage_start);

    // clear the page in memory
    for (int i = 0; i < PAGE_SIZE; i++)
        ppage_start[i] = 0;
}

/*
 * Swap a swap page of memory on disk in to a physical page in memory.
 */
void swap_in(int fd, int ppage_num, int block_num)
{
    printf("SWAP IN : mem %d <- %d disk\n", ppage_num, block_num);

    // read the page from disk
    char *ppage_start = ppage_get_start_addr(ppage_num);

    // write the page to memory
    block_seek(fd, block_num);
    block_read(fd, block_num, ppage_start);

    // clear the page on disk
    for (int i = 0; i < PAGE_SIZE; i++)
        ppage_start[i] = 0;
}

/*
 * Initialize the RAM to something sensible.
 *
 * Page 0 will be all zeros, page 14 will be all 14s, page 255 will be
 * all 255s, etc.
 */
void init_ram(void)
{
    for (int i = 0; i < RAM_SIZE; i++)
        ram[i] = i >> 8;
}

/*
 * Verifies that a particular page contains a particular value. For
 * testing.
 */
void verify_page(int ppage_num, int expected_value)
{
    char *ppage_start = ppage_get_start_addr(ppage_num);

    for (int i = 0; i < PAGE_SIZE; i++)
        if (ppage_start[i] != expected_value) {
            printf("BAD     : page %d contains %d (expected %d)\n",
                ppage_num, ppage_start[i], expected_value);
            return;
        }

    printf("GOOD    : page %d contains %d\n",
        ppage_num, ppage_start[0]);
}

/*
 * Fills a ppage with garbage to simulate being used by another process.
 * For testing.
 */
void reuse_ppage_for_something(int ppage_num)
{
    char *ppage_start = ppage_get_start_addr(ppage_num);

    for (int i = 0; i < PAGE_SIZE; i++)
        *(ppage_start++) = rand() & 0xff;
}

/*
 * Main
 */
int main(void)
{
    srand(getppid() + getpid());

    init_ram();

    int fd = swap_open();

    swap_out(fd, 0, 9);
    swap_out(fd, 1, 41);
    swap_out(fd, 2, 7);
    swap_out(fd, 3, 19);

    reuse_ppage_for_something(0);
    reuse_ppage_for_something(1);
    reuse_ppage_for_something(2);
    reuse_ppage_for_something(3);

    verify_page(0, 0);
    verify_page(1, 1);
    verify_page(2, 2);
    verify_page(3, 3);

    swap_in(fd, 0, 9);
    swap_in(fd, 1, 41);
    swap_in(fd, 2, 7);
    swap_in(fd, 3, 19);

    verify_page(0, 0);
    verify_page(1, 1);
    verify_page(2, 2);
    verify_page(3, 3);

    swap_close(fd);

    /*
    Expected output (BAD numbers will vary):

    SWAP OUT: mem 0 -> 9 disk
    SWAP OUT: mem 1 -> 41 disk
    SWAP OUT: mem 2 -> 7 disk
    SWAP OUT: mem 3 -> 19 disk
    BAD     : page 0 contains 32 (expected 0)
    BAD     : page 1 contains 49 (expected 1)
    BAD     : page 2 contains 4 (expected 2)
    BAD     : page 3 contains -99 (expected 3)
    SWAP IN : mem 0 <- 9 disk
    SWAP IN : mem 1 <- 41 disk
    SWAP IN : mem 2 <- 7 disk
    SWAP IN : mem 3 <- 19 disk
    GOOD    : page 0 contains 0
    GOOD    : page 1 contains 1
    GOOD    : page 2 contains 2
    GOOD    : page 3 contains 3
    */
}
