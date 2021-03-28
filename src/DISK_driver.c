/****************************************************
 * DISK_driver.c - Contains data-structures and     *
 * algorithms to implement disk partition creation, *
 * mounting, and file IO operations.                *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include "DISK_driver.h"

// Local copy of active partition metadata
struct PARTITION {
    unsigned char total_blocks; // Total blocks for file data
    unsigned char used_blocks;  // Number of non-empty blocks
    unsigned int block_size;    // Size of each block in bytes
} localPartition;

// Local copy of active FAT
struct FAT {
    char *filename;             // Limit is 20 chars (including null end character)
    unsigned char file_length;  // Number of data blocks affiliated with the file
    unsigned char blockPtrs[10];// Pointers to the blocks in the parttion populated with data. These pointers are block numbers.
    signed char current_location;   // Pointer to the current read/write location of the file in block number. Initialized to -1 when not used.
} localFAT;

// Block buffer is malloced to the correct size 
// when the partition() function is called, with the size
// of block_size. The block buffer is used by read_block()
// to bring in blocks from disk.
char *block_buffer = NULL;

// The active file table contains all
// the system wide open file pointers,
// which is 5 maximum.
FILE *active_file_table[5] = {NULL};

// The active file map keeps track of which
// file, represented as FAT index number, is
// associated with the file pointer. The value
// set to -1 when there is no file affiliated.
int active_file_map[5];

