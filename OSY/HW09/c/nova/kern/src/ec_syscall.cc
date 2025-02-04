#include "../include/ec.h"
#include "../include/ptab.h"
#include "../include/string.h"

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yield  = 5,
} Syscall_numbers;

void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);

    switch (number) {
        case sys_print: {
            // Tisk řetězce na sériovou linku
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_sum: {
            // Naprosto nepotřebné systémové volání na sečtení dvou čísel
            int first_number = r->esi;
            int second_number = r->edi;
            r->eax = first_number + second_number;
            break;
        }
        case sys_break: {
            mword address = r->esi;
            if (address == 0) r->eax = Ec::break_current;
            else if (address < Ec::break_min || address >= 0xbffff000) r->eax = 0;
            else {
                mword backup = Ec::break_current;
                mword temp = Ec::break_current;
                bool error = false;
                if (address > temp){
                    if (temp % PAGE_SIZE){
                        if ((address & 0xfffff000) == (temp & 0xfffff000)){
                            memset(reinterpret_cast<void*>(temp), 0x00, address - temp);
                            temp = address;
                        }
                        else {
                            memset(reinterpret_cast<void*>(temp), 0x00, PAGE_SIZE - (temp & 0x00000fff));
                            temp &= 0xfffff000;
                            temp += PAGE_SIZE;
                        }
                    }
                    while (temp < address){
                        void *page = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
                        if (!page){
                            error = true;
                            printf("ERROR: Couldn't allocate page!\n");
                            break;
                        }
                        if (!Ptab::insert_mapping(temp, Kalloc::virt2phys(page), Ptab::PRESENT | Ptab::RW | Ptab::USER)){
                            error = true;
                            printf("ERROR: Couldn't map page!\n");
                            break;
                        }
                        if ((address & 0xfffff000) == (temp & 0xfffff000)){
                            memset(reinterpret_cast<void*>(temp), 0x00, address - temp);
                            temp = address;
                        }
                        else{
                            memset(reinterpret_cast<void*>(temp), 0x00, PAGE_SIZE);
                            temp &= 0xfffff000;
                            temp += PAGE_SIZE;
                        }
                    }
                    if (error){
                        mword *tab;
                        mword *dir;
                        void *virt_tab;
                        while(temp > backup){
                            temp -=PAGE_SIZE;
                            if (temp < backup) break;
                            Kalloc::allocator.free_page(reinterpret_cast<void*>((reinterpret_cast<unsigned long>(Kalloc::phys2virt(Ptab::get_mapping(temp)))) & 0xfffff000));
                            dir = static_cast<mword*>(Kalloc::phys2virt(Cpu::cr3()));
                            virt_tab = Kalloc::phys2virt(dir[temp >> 22] & ~PAGE_MASK);
                            tab = static_cast<mword*>(virt_tab);
                            tab[(temp >> PAGE_BITS) & 0x3ff] = 0;
                            Cpu::flush();
                        }
                        r->eax = 0;
                        break;
                    }
                }
                else if (temp > address){
                    mword *dir;
                    mword *tab;
                    void *virt_tab;
                    if (temp % PAGE_SIZE){
                        if ((address & 0xfffff000) == (temp & 0xfffff000)){
                            memset(reinterpret_cast<void*>(address), 0x00, temp - address);
                            temp = address;
                        }
                        else{
                            memset(reinterpret_cast<void*>(temp & 0xfffff000), 0x00, temp & 0x00000fff);
                            temp &= 0xfffff000;
                            Kalloc::allocator.free_page(reinterpret_cast<void*>((reinterpret_cast<unsigned long>(Kalloc::phys2virt(Ptab::get_mapping(temp)))) & 0xfffff000));
                            dir = static_cast<mword*>(Kalloc::phys2virt(Cpu::cr3()));
                            virt_tab = Kalloc::phys2virt(dir[temp >> 22] & ~PAGE_MASK);
                            tab = static_cast<mword*>(virt_tab);
                            tab[(temp >> PAGE_BITS) & 0x3ff] = 0;
                            Cpu::flush();
                        }
                    }
                    while(temp > address){
                        temp -= PAGE_SIZE;
                        if (temp < address) break;
                        Kalloc::allocator.free_page(reinterpret_cast<void*>((reinterpret_cast<unsigned long>(Kalloc::phys2virt(Ptab::get_mapping(temp)))) & 0xfffff000));
                        dir = static_cast<mword*>(Kalloc::phys2virt(Cpu::cr3()));
                        virt_tab = Kalloc::phys2virt(dir[temp >> 22] & ~PAGE_MASK);
                        tab = static_cast<mword*>(virt_tab);
                        tab[(temp >> PAGE_BITS) & 0x3ff] = 0;
                        Cpu::flush();
                    }
                    if (temp < address){
                        memset(reinterpret_cast<void*>(address), 0x00, temp + PAGE_SIZE - address);
                        temp = address;
                    }
                }
                Ec::break_current = address;
                r->eax = backup;
            }
            break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}
