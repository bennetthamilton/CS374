#include <stdio.h>
#include <string.h>

/*
 * Multi-level page table simulator.
 *
 * This simulated machine has 64 KB RAM and a two-level page table.
 *
 * Each page is 256 bytes.
 *
 * The "Directory" level is always at Page 0 in physical memory, and
 * contains up to 256 entries pointing to page table pages.
 *
 * Each page table page contains up to 256 entries pointing to physical
 * data pages.
 *
 * A virtual address is 24 bits (making a virtual address space of 16
 * MB per process). It is split into 3 sections of 8 bits each:
 *
 * +------------------+-------------------+------------------+
 * | directory index  |  page table index |      offset      |
 * +------------------+-------------------+------------------+
 *
 * Task: Convert a virtual address to a physical address.
 *
 * You are given:
 *
 *   * The directory page number
 *   * The virtual address
 *
 * From that, you need to determine the corresponding physical address.
 */

const int DIR_PAGE_NUM = 0;
const int PAGE_SIZE = 256;

char physical_ram[65536];

/*
 * Return the starting address for a given page number.
 */
int get_page_start_addr(int page)
{
    return page * PAGE_SIZE;
}

/*
 * For a given physical page and page table entry number, return the
 * entry value. This works on the directory page and the page table
 * page.
 */
int get_page_table_entry(int ppage, int entry)
{
    int ppage_addr = get_page_start_addr(ppage);

    return physical_ram[ppage_addr + entry];
}

/*
 * For a given physical page and page table entry number, set the entry
 * value. This works on the directory page and the page table page.
 */
void set_page_table_entry(int ppage, int entry, int value)
{
    int ppage_addr = get_page_start_addr(ppage);

    physical_ram[ppage_addr + entry] = value;
}

/*
 * For a given virtual address, follows the two-level page table and
 * returns the corresponding physical address.
 */
int vaddr_to_paddr(int dir_page_num, int vaddr)
{
    // Split the virtual address into parts
    int dir_index = (vaddr >> 16) & 0xff;
    int page_table_index = (vaddr >> 8) & 0xff;
    int offset = vaddr & 0xff;

    // Look up the page table page
    int page_table_page = get_page_table_entry(dir_page_num, dir_index);
    if (page_table_page == 0) return 0;
    // Look up the physical address
    int page = get_page_table_entry(page_table_page, page_table_index);
    if (page == 0) return 0;

    return get_page_start_addr(page) + offset;
    
}

/*
 * Main
 */
int main(void)
{
    // Set up the page tables for this process. This would be the job of
    // the OS.
    //
    // The way this is set up, virtual address 3876654 will map to
    // physical address 6958.
    //
    // For your solution, you can't reference page 8 or 27 or address
    // 6958 directly--you must start with DIR_PAGE_NUM and follow the
    // pointers.
    //
    int dir_page_num = 4;

    set_page_table_entry(dir_page_num, 59, 8);
    set_page_table_entry(8, 39, 27);

    // Store some data in physical memory for testing at the same
    // place that the virtual location 3876654 maps to.
    strcpy(physical_ram + 6958, "Hello, world!");

    // Look up the physical address for virtual address 3876654
    int paddr = vaddr_to_paddr(dir_page_num, 3876654);

    // Print what's there--which should be "Hello, world!"
    puts(physical_ram + paddr);
}

