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

typedef char ALIGN[16];
typedef union header{
    struct{
        unsigned long size;
        unsigned is_free;
        union header *next;
        union header *previopus;
    } data;
    ALIGN align;
} header_s;


#define ALIGNMENT 16
#define HEADER_SIZE sizeof(header_s)

header_s *head = NULL;
header_s *tail = NULL;

header_s *get_free_block(unsigned long size){
    header_s *current = head;
    while(current){
        if(current->data.is_free && current->data.size >= size) return current;
        current = current->data.next;
    }
    return 0;
}

void *my_malloc(unsigned long size){
    void *block;
    header_s *header = head;
    if(!size) return NULL;
    header = get_free_block(size);

    if (header != NULL){
        header->data.is_free = 0;
        return (void*)(header + 1);
    }
    block = nbrk((void*)(HEADER_SIZE + size + (char*)nbrk(0)));
    
    if (block == NULL) return NULL;
    
    header = (header_s*)block;
    header->data.is_free = 0;
    header->data.size = size;
    header->data.next = NULL;

    if(head == NULL) head = header;
    if(tail != NULL) tail->data.next = header;
    tail = header;

    return (void*)(header + 1);
}

int my_free(void *address){
    header_s *current = (header_s*)address - 1;
    header_s *temp;

    if (current < head || current > tail) return -1;
    void *brk = nbrk(0);
    if((char*)address + current->data.size == brk){
        if(head == tail) head = tail = NULL;
        else{
            temp = head;
            while(temp != NULL){
                if (temp->data.next == tail){
                    temp->data.next = NULL;
                    tail = temp;
                }
                temp = temp->data.next;
            }
        }
        nbrk((void*)((unsigned long int)nbrk(0) - HEADER_SIZE - current->data.size));
        return 0;
    }
    current->data.is_free = 1;
    return 0;
}

