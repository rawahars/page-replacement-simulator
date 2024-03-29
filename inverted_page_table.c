/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdlib.h>
#include "inverted_page_table.h"

page_frame *get_page_frame(void *ipt_root, int pf_id);

page_frame *create_page_frame(int id);

int compare_memory_trace_page_frame(const void *a, const void *b);

void *CreateInvertedPageTable(int max_pages) {
    void *root = NULL;
    for (int i = 1; i <= max_pages; i++) {
        page_frame *new_page_frame = create_page_frame(i);
        Put(&root, new_page_frame, &compare_memory_trace_page_frame);
    }
    return root;
}

void Unlink_pf_pte(void *ipt_root, int pf_id) {
    page_frame *existing = get_page_frame(ipt_root, pf_id);
    // Handle unlink if page table entry does not exist
    if (existing->page_table_entry == NULL)
        return;
    existing->page_table_entry->page_frame = NULL;
    existing->page_table_entry = NULL;
}

void Link_pf_pte(void *ipt_root, int pf_id, page_table_entry *pte) {
    page_frame *existing = get_page_frame(ipt_root, pf_id);
    existing->page_table_entry = pte;
    pte->page_frame = existing;
}

void Callback_free_page_frames(const void *data, VISIT order, int depth) {
    page_table_entry *entry = *(page_table_entry **) data;
    if (entry->page_frame == NULL) return;

    //Free the page frame
    page_frame *page_frame = entry->page_frame;
    FreePageFrame(page_replacement_algo, page_frame->ppn_id);
    page_frame->page_table_entry = NULL;
    entry->page_frame = NULL;
    //Unused params of callback
    UNUSED(order);
    UNUSED(depth);
}

page_frame *get_page_frame(void *ipt_root, int pf_id) {
    page_frame *reference_page_frame = create_page_frame(pf_id);
    page_frame *existing = Get(&ipt_root, reference_page_frame, &compare_memory_trace_page_frame);

    //No such page frame exists
    if (existing == NULL)
        ThrowError("Page frame is referenced which does not exist.");
    free(reference_page_frame);

    return existing;
}

page_frame *create_page_frame(int id) {
    page_frame *new_page_frame = malloc(sizeof(page_frame));
    ValidateMemoryAllocationError(new_page_frame);
    new_page_frame->ppn_id = id;
    new_page_frame->page_table_entry = NULL;
    return new_page_frame;
}

int compare_memory_trace_page_frame(const void *a, const void *b) {
    page_frame *node_a = (page_frame *) a;
    page_frame *node_b = (page_frame *) b;

    if (node_a->ppn_id < node_b->ppn_id)
        return -1;
    else if (node_a->ppn_id > node_b->ppn_id)
        return 1;
    else
        return 0;
}
