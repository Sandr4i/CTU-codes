#include "mem_alloc.h"
#include <stdio.h>
#include <stddef.h>

static inline void *nbrk(void *address);

#ifdef NOVA

/**********************************/
/* nbrk() implementation for NOVA */
/**********************************/

static inline unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile("   mov %%esp, %%ecx    ;"
                 "   mov $1f, %%edx      ;"
                 "   sysenter            ;"
                 "1:                     ;"
                 : "+a"(w0)
                 : "S"(w1)
                 : "ecx", "edx", "memory");
    return w0;
}

static void *nbrk(void *address)
{
    return (void *)syscall2(3, (unsigned)address);
}
#else

/***********************************/
/* nbrk() implementation for Linux */
/***********************************/

#include <unistd.h>

static void *nbrk(void *address)
{
    void *current_brk = sbrk(0);
    if (address != NULL) {
        int ret = brk(address);
        if (ret == -1)
            return NULL;
    }
    return current_brk;
}

#endif

typedef struct Block{
    unsigned long size;
    struct Block* next;
} Block;

static Block* free_list = NULL;

#define ALIGNMENT 16
#define HEADER_SIZE sizeof(Block)

void *my_malloc(unsigned long size)
{
    if (size == 0) return NULL;
    size  = (size + ALIGNMENT - 1 + HEADER_SIZE) & ~(ALIGNMENT - 1);
    Block *current = free_list;
    Block *previous = NULL;

    while(current != NULL){
        if(current->size >= size){
            if(current->size > size + HEADER_SIZE){
                Block *new_block = (Block*)((char*)current + size);
                new_block->size = current->size - size;
                new_block->next = current->next;

                if(previous) previous->next = new_block;
                else free_list = new_block;

                current->size = size;
            }
            else{
                if(previous) previous->next = current->next;
                else free_list = current->next;
            }
            return (void*)((char*)current + HEADER_SIZE);
        }

        previous = current;
        current = current->next;
    }

    void *new_block = nbrk(NULL);
    if(new_block == (void*)-1) return NULL;

    Block *header = (Block*)new_block;
    header->size = size;
    header->next = NULL;
    return (void*)((char*)new_block + HEADER_SIZE);
}

int my_free(void *address)
{
    if (address == NULL 
    || address < (void*)&free_list 
    || address >= (void*)nbrk(NULL)) return -1;

    Block *header = (Block*)((char*)address - HEADER_SIZE);
    if (((unsigned long)address) % ALIGNMENT != 0) return -1;

    Block *current = free_list;
    Block *previous = NULL;

    while (current && ((char*)current + current->size) < (char*)header){
        previous = current;
        current = current->next;
    }

    if (previous && ((char*)previous + previous->size) == (char*)header){
        previous->size += HEADER_SIZE + header->size;
        header = previous;
    }
    else{
        header->next = free_list;
        free_list = header;
    }

    if(current && ((char*)header + header->size) == (char*)current){
        header->size += HEADER_SIZE + current->size;
        header->next = current->next;
    }
    else{
        header->next = current;
    }
    return 0;
}
