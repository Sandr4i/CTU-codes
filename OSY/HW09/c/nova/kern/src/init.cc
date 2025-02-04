/*
 * Initialization Code
 *
 * Copyright (C) 2009-2011 Udo Steinberg <udo@hypervisor.org>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of the NOVA microhypervisor.
 *
 * NOVA is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NOVA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 */

#include "../include/compiler.h"
#include "../include/types.h"
#include "../include/extern.h"
#include "../include/memory.h"
#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/gdt.h"
#include "../include/tss.h"
#include "../include/gsi.h"
#include "../include/idt.h"
#include "../include/kalloc.h"
#include "../include/assert.h"
#include "../include/cpu.h"
#include "../include/ptab.h"
#include "../include/ec.h"
#include "../include/io.h"
#include "../include/msr.h"

char const *version = "NOVA Microhypervisor 0.3 (Cleetwood Cove)";

extern "C"
void init ()
{
    for (void (**func)() = &CTORS_G; func != &CTORS_E; (*func++)()) ;

    serial.init();

    // Now we're ready to talk to the world
    printf ("\f%s: %s %s [%s]\n\n", version, __DATE__, __TIME__, COMPILER_STRING);

    mword iobm = Kalloc::virt2phys (Kalloc::allocator.alloc_page(2,Kalloc::FILL_1));
    Ptab::insert_mapping (KSTCK_ADDR, Kalloc::virt2phys (Kalloc::allocator.alloc_page(1)), 0x23);
    Ptab::insert_mapping (IOBMP_SADDR, iobm, 0x23);
    Ptab::insert_mapping (IOBMP_SADDR + PAGE_SIZE, iobm + PAGE_SIZE, 0x23);

    for (void (**func)() = &CTORS_L; func != &CTORS_G; (*func++)()) ;

    Gdt::build();
    Tss::build();
    Idt::build();
    Gsi::setup();

    Gdt::load();
    Tss::load();
    Idt::load();

    // offset and mask all IRQs
    Io::out<uint8> (0x20, 0x11);
    Io::out<uint8> (0x21, VEC_GSI);
    Io::out<uint8> (0x21, 0x4);
    Io::out<uint8> (0x21, 0x1);
    Io::out<uint8> (0x21, 0xff);

    // setup sysenter
    Msr::write<mword>(Msr::IA32_SYSENTER_CS,  SEL_KERN_CODE);
    Msr::write<mword>(Msr::IA32_SYSENTER_ESP, reinterpret_cast<mword>(&Tss::run.sp0));
    Msr::write<mword>(Msr::IA32_SYSENTER_EIP, reinterpret_cast<mword>(&entry_sysenter));
}

extern "C" REGPARM (1) NORETURN
void bootstrap (mword addr)
{
    // unmap everything below 3G : 0 - LOAD_E
    mword* pdir = static_cast<mword*>(Kalloc::phys2virt(Cpu::cr3()));
    mword e = reinterpret_cast<mword>(&LOAD_E) >> 22;
    for (mword a = 0; a <= e; pdir[a++] = 0) ;
    Cpu::flush();

    // Setup execution context for the root "task"
    Ec::current = new Ec();
    Ec::current->root_setup(addr);

    // Start executing the root task in user space
    Ec::current->make_current();

    UNREACHED;
}
