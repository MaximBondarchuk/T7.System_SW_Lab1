//
// Created by max on 26.09.15.
//

#include "Memory.h"

Memory::Memory(uint16_t pages_number, uint16_t page_size) {
    page_size = (uint16_t) size_with_align_4B(page_size);

    pages = vector<Page>(pages_number, Page(page_size));
    pages_blocks = vector<pages_with_state_3_block>();
}

void *Memory::mem_alloc(size_t size) {
    size = size_with_align_4B((uint32_t) size);

    // If need more than half of the page
    if (pages[0].page_size / 2 + 1 <= size) {
        // How many
        size_t how_many = size / pages[0].page_size;
        if (how_many * pages[0].page_size < size)
            how_many++;

        uint16_t current_length = 0;
        int start_idx = -1;
        for (uint16_t i = 0; i < pages.size(); i++) {
            if (current_length == 0 && pages[i].state == 0) {
                current_length++;
                start_idx = i;
            }
            else if (current_length != 0 && pages[i].state == 0) {
                current_length++;
                if (current_length == how_many)
                    break;
            }
            else
                current_length = 0;
        }

        if (start_idx == -1)
            return NULL;

        for (uint16_t i = 0; i < how_many; i++)
            pages[start_idx + i].state = 3;

        pages_blocks.push_back(pages_with_state_3_block((uint16_t) start_idx, (uint16_t) how_many));

        return &pages[start_idx];
    }

    // Trying find already divided page
    for (uint16_t i = 0; i < pages.size(); i++)
        if (pages[i].state == 2 && pages[i].block_size / 2 < size && size <= pages[i].block_size &&
            pages[i].have_free_block())
            for (uint16_t j = 0; j < pages[i].blocks_count; j++)
                if (find(pages[i].in_use_blocks_info.begin(), pages[i].in_use_blocks_info.end(), j) ==
                    pages[i].in_use_blocks_info.end()) {
                    pages[i].in_use_blocks_info.push_back(j);
                    return &pages[i].memory_block[j];
                }

    // Dividing free page
    for (uint16_t i = 0; i < pages.size(); i++)
        if (pages[i].state == 0) {
            pages[i].split_to_blocks((uint16_t) size);
            pages[i].in_use_blocks_info.push_back(0);
            return &pages[i].memory_block[0];
        }

    return NULL;
}

void *Memory::mem_realloc(void *addr, size_t size) {
//    size = size_with_align_4B(size);
//
//    if (addr == NULL)
//        return mem_alloc(size);
//
//    // Trying enhance current block
//    long idx = what_number_am_i(addr);  // Index in memory
//    size_t my_size = 0;                 // Size of current block
//    int used_idx = 0;                   // Current block index in info vector
//    for (int i = 0; i < info_in_use.size(); i++)    // Finding this index
//        if (info_in_use[i].addr == idx) {
//            my_size = (int) info_in_use[i].size;
//            used_idx = i;
//            break;
//        }
//    int need_size = (int) (size - my_size);  // How many more we need
//    if (need_size > 0) {    // If we need to extend
//        for (int i = 0; i < info_free_pages.size(); i++)
//            if (info_free_pages[i].addr == idx + my_size &&
//                need_size <= info_free_pages[i].size) {     // If have on right free block
//                info_in_use[used_idx].size += need_size;
//                if (info_free_pages[i].size == need_size)
//                    info_free_pages.erase(info_free_pages.begin() + i);
//                else {
//                    info_free_pages[i].addr += need_size;
//                    info_free_pages[i].size -= need_size;
//                }
//                return addr;
//            }
//    }
//    else {  // If we need to reduce
//        info_in_use[used_idx].size += need_size;
//        for (int i = 0; i < info_free_pages.size(); i++)
//            if (info_free_pages[i].addr == idx + info_in_use[used_idx].size - need_size) {
//                info_free_pages[i].addr += need_size;
//                info_free_pages[i].size -= need_size;
//                return addr;
//            }
//        info_free_pages.push_back(Memory_unit_info((int) (info_in_use[used_idx].addr + info_in_use[used_idx].size),
//                                             (size_t) -need_size));
//    }
//
//    // If cannot enhance current block
//    for (int i = 0; i < info_free_pages.size(); i++)
//        if (size <= info_free_pages[i].size) {
//            info_in_use.push_back(Memory_unit_info(info_free_pages[i].addr, size));
//            if (info_free_pages[i].size == size)
//                info_free_pages.erase(info_free_pages.begin() + i);
//            else {
//                info_free_pages[i].addr += size;
//                info_free_pages[i].size -= size;
//            }
//
//            // Moving data to new place
//            for (int j = 0; j < info_in_use[used_idx].size; j++)
//                memory_block[info_in_use[used_idx].addr + j] = memory_block[info_free_pages[i].addr + j];
//            mem_free(addr);
//
//            return &memory_block[info_in_use[info_in_use.size() - 1].addr];
//        }

    return NULL;
}

void Memory::mem_free(void *addr) {
//    long idx = what_number_am_i(addr);
//    for (int i = 0; i < info_in_use.size(); i++)
//        if (idx == info_in_use[i].addr) {
//            info_free_pages.push_back(Memory_unit_info(info_in_use[i].addr, info_in_use[i].size));
//            info_in_use.erase(info_in_use.begin() + i);
//            break;
//        }
//
//    // Finding free block after current to merge
//    for (int i = 0; i < info_free_pages.size() - 1; i++)
//        if (info_free_pages[info_free_pages.size() - 1].addr + info_free_pages[info_free_pages.size() - 1].size == info_free_pages[i].addr) {
//            info_free_pages[info_free_pages.size() - 1].size += info_free_pages[i].size;
//            info_free_pages.erase(info_free_pages.begin() + i);
//            break;
//        }
//
//    // Finding free block before current to merge
//    for (int i = 0; i < info_free_pages.size() - 1; i++)
//        if (info_free_pages[i].addr + info_free_pages[i].size == info_free_pages[info_free_pages.size() - 1].addr) {
//            info_free_pages[i].size += info_free_pages[info_free_pages.size() - 1].size;
//            info_free_pages.erase(info_free_pages.begin() + info_free_pages.size() - 1);
//            break;
//        }
}

void Memory::mem_dump() {
//    int i = 0;
//    while (i < memory_block.size()) {
//        for (int j = 0; j < info_free_pages.size(); j++)
//            if (info_free_pages[j].addr == i) {
//                cout << string(info_free_pages[j].size, '_');
//                i += info_free_pages[j].size;
//            }
//
//        for (int j = 0; j < info_in_use.size(); j++)
//            if (info_in_use[j].addr == i) {
//                cout << string(info_in_use[j].size, '*');
//                i += info_in_use[j].size;
//            }
//    }
//    cout << endl;
}

long Memory::what_number_am_i(void *addr) {
    if (addr == NULL)
        return -1;

    long needed_addr = (long) addr;

//    void *s_addr = &pages[0];
//    long start_addr = (long) s_addr;

    for (int i = 0; i < pages.size(); i++) {
        void *c_addr = &pages[i];
        long current_addr = (long) c_addr;
        if (current_addr <= needed_addr && needed_addr < current_addr + sizeof(pages[i]))
            return i;
    }
    return -1;

//    return ((long) addr - (long) &pages[0]) / sizeof(Page);
}

uint32_t Memory::size_with_align_4B(uint32_t size) {
    if (size % 4 == 0)
        return size;
    return size - size % 4 + 4;
}
