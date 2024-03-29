/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#ifndef PAGE_REPLACEMENT_SIMULATOR_FIFO_PAGE_REPLACEMENT
#define PAGE_REPLACEMENT_SIMULATOR_FIFO_PAGE_REPLACEMENT

#include "error_handler.h"
#include "tsearch.h"
#include "doubly_linked_list.h"

typedef struct fifo_module {
    int max_size;
    int free_pages;
    doubly_linked_list *free_list;
    void *node_tree;
    doubly_linked_list *fifo_list;
} fifo_module;

typedef struct page {
    int ppn_id;
    int is_free;
} page;

extern fifo_module *page_replacement_algo;

fifo_module *CreateFIFOModule(int max_size);

int GetReplacementPage(fifo_module *fifo);

void FreePageFrame(fifo_module *fifo, int ppn_id);

int GetOccupiedPageFrames(fifo_module *fifo);

void UpdatePageFrameIntoMemory(fifo_module *fifo, int ppn_id);

void UpdateAccessedPageFrame(fifo_module *fifo, int ppn_id);

#endif
