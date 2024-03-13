#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    memset(mem, 0, MEM_SIZE);

    int zpfree_addr = get_address(0, 0);
    mem[zpfree_addr] = 1;  // Mark zero page as allocated
}

//
// Get the page table page for a given process
//
unsigned char get_page_table(int proc_num)
{
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    return mem[ptp_addr];
}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    // allocate the page table
    int page_table = 0;

    // find a free page for the page table
    for (int i = 1; i < PAGE_COUNT; i++) {
        if (mem[get_address(0, i)] == 0) {
            page_table = i;
            mem[get_address(0, i)] = 1;
            break;
        }
    }

    // check if the allocation failed
    if (page_table == 0) {
        printf("OOM: proc %d: page table\n", proc_num);
        return;
    }

    // allocate page_count data pages
    for (int i = 0; i < page_count; i++) {
        // allocate a page
        int page = 0;

        // find a free page
        for (int j = 1; j < PAGE_COUNT; j++) {
            if (mem[get_address(0, j)] == 0) {
                page = j;
                mem[get_address(0, j)] = 1;
                break;
            }
        }

        // check if the allocation failed
        if (page == 0) {
            printf("OOM: proc %d: data page\n", proc_num);
            return;
        }

        // store the page number in the page table
        int page_table_entry = get_address(page_table, i);
        mem[page_table_entry] = page;
        
        // update the corresponding entry in the page table pointer section
        mem[get_address(0, PTP_OFFSET + proc_num)] = page_table;
    }
}

//
// Free the pages of a process
//
void free_page(int page)
{
    mem[get_address(0, page)] = 0;
}

//
// Kill a process and free its pages
//
void kill_process(int proc_num)
{
    // get the page table
    int page_table = get_page_table(proc_num);

    // free the page table
    free_page(page_table);

    // free the data pages
    for (int i = 0; i < PAGE_COUNT; i++) {
        int page_table_entry = get_address(page_table, i);
        free_page(mem[page_table_entry]);
    }

    // free the page table pointer
    free_page(PTP_OFFSET + proc_num);
}

//
// Get the physical address for a virtual address
//
int get_physical_address(int proc_num, int virt_addr)
{
    // get the page table
    int page_table = get_page_table(proc_num);

    // get the page number and offset
    int virt_page = virt_addr >> PAGE_SHIFT;
    int offset = virt_addr & (PAGE_SIZE - 1);

    // get the physical address from the page table
    int page_table_entry = get_address(page_table, virt_page);
    int page = mem[page_table_entry];
    int phys_addr = get_address(page, offset);

    return phys_addr;
}

//
// Store a value in a process's virtual memory
//
void store_byte(int proc_num, int virt_addr, int value)
{

    // get the physical address from the page table
    int phys_addr = get_physical_address(proc_num, virt_addr);

    // store the value
    mem[phys_addr] = value;

    // print result
    printf("Store proc %d: %d => %d, value=%d\n", proc_num, virt_addr, phys_addr, value);
}

//
// Load a value from a process's virtual memory
//
void load_byte(int proc_num, int virt_addr)
{
    // get the physical address
    int phys_addr = get_physical_address(proc_num, virt_addr);

    // load the value
    int value = mem[phys_addr];

    // print result
    printf("Load proc %d: %d => %d, value=%d\n", proc_num, virt_addr, phys_addr, value);
}

//
// Print the free page map
//
// Don't modify this
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0? '.': '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}

//
// Print the address map from virtual pages to physical
//
// Don't modify this
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}


//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "pfm") == 0) {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
        else if (strcmp(argv[i], "np") == 0) {
            int proc_num = atoi(argv[++i]);
            int page_count = atoi(argv[++i]);
            new_process(proc_num, page_count);
        }
        else if (strcmp(argv[i], "kp")) {
            int proc_num = atoi(argv[++i]);
            kill_process(proc_num);
        }
        else if (strcmp(argv[i], "sb")) {
            int proc_num = atoi(argv[++i]);
            int vaddr = atoi(argv[++i]);
            int addr = get_address(0, vaddr);
            int val = atoi(argv[++i]);
            store_byte(proc_num, vaddr, val);
        }
        else if (strcmp(argv[i], "lb")) {
            int proc_num = atoi(argv[++i]);
            int vaddr = atoi(argv[++i]);
            int addr = get_address(0, vaddr);
            load_byte(proc_num, vaddr);
        }
        else {
            fprintf(stderr, "unknown command: %s\n", argv[i]);
            return 1;
        }
    }

    return 0;
}
