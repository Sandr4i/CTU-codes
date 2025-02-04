        case sys_break: {
            mword address = r->esi;
            if (address == 0) r->eax = Ec::break_current;
            else if (address < Ec::break_min || address >= 0xbffff000) r->eax = 0;
            else{
                mword backup = Ec::break_current;
                mword temp_address = Ec::break_current;

                if (address > temp_address){
                    if (temp_address % PAGE_SIZE){
                        if ((temp_address & 0xfffff000) == (address & 0xfffff000)){
                            memset(reinterpret_cast<void*>(temp_address), 0x00, address - temp_address);
                            temp_address = address;
                        }
                        else{
                            memset(reinterpret_cast<void*>(temp_address), 0x00, PAGE_SIZE - (temp_address & 0xfffff000));
                            temp_address &= 0xfffff000;
                            temp_address += PAGE_SIZE;
                        }
                    }
                    while (temp_address < address){
                        void *page = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
                        if (!page) panic("Lack of memory!\n");
                        
                        if (!Ptab::insert_mapping(temp_address, Kalloc::virt2phys(page), Ptab::PRESENT | Ptab::RW | Ptab::USER)) panic("Lack of memory!\n");
                        if ((temp_address & 0xfffff000) == (address & 0xfffff000)){
                            memset(reinterpret_cast<void*>(temp_address), 0x00, address - temp_address);
                            temp_address = address;
                        }
                        else{
                            memset(reinterpret_cast<void*>(temp_address), 0x00, PAGE_SIZE);
                            temp_address &= 0xfffff000;
                            temp_address += PAGE_SIZE;
                        }
                    }
                }
                else if (address < temp_address){
                    temp_address -= PAGE_SIZE;
                    while (temp_address > address){
                        mword *pdir = static_cast<mword*>(Kalloc::phys2virt(Cpu::cr3()));
                        void *ptab_virt = Kalloc::phys2virt(pdir[Ec::break_min >> 22] & ~PAGE_MASK);
                        mword *ptab = static_cast<mword*>(ptab_virt);
                        ptab[(temp_address >> PAGE_BITS) & 0x3ff] = 0;
                        Kalloc::allocator.free_page(reinterpret_cast<void*>((reinterpret_cast<unsigned long>(ptab_virt) + reinterpret_cast<unsigned long>(temp_address)) & 0xfffff000));
                        Cpu::flush();
                        temp_address -= PAGE_SIZE;
                    }
                    if (temp_address < address){
                        memset(reinterpret_cast<void*>(address), 0x00, address - temp_address + PAGE_SIZE);
                        temp_address = address;
                    }
                }
                Ec::break_current = address;
                r->eax = backup;
            }
            break;
        }


         case sys_break: {
            mword address = r->esi;
            if (address == 0) {
                // Return current break value
                r->eax = Ec::break_current;
            } else if (address < Ec::break_min || address >= 0xbffff000) {
                // Invalid address, return 0
                r->eax = 0;
            } else {
                mword old_break = Ec::break_current;
                mword temp_address = old_break;
                // Increase break
                while (temp_address < address) {
                    void *page = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
                    if (!page) {
                        // Rollback on allocation failure
                        while (temp_address > old_break) {
                            temp_address -= PAGE_SIZE;
                            Ptab::remap(temp_address);
                            Kalloc::allocator.free_page(reinterpret_cast<void*>(temp_address));
                            Cpu::flush();
                        }
                        panic("Lack of memory!\n");
                    }
                    if (!Ptab::insert_mapping(temp_address, Kalloc::virt2phys(page), Ptab::PRESENT | Ptab::RW | Ptab::USER)) {
                        // Rollback on mapping failure
                        Ptab::remap(temp_address);
                        Kalloc::allocator.free_page(page);
                        panic("Mapping failure!\n");
                    }

                    temp_address += PAGE_SIZE;
                }
                // Decrease break
                while (temp_address > address) {
                    temp_address -= PAGE_SIZE;
                    Ptab::remap(temp_address);
                    Kalloc::allocator.free_page(reinterpret_cast<void*>(temp_address));
                    Cpu::flush();
                }
                // Update break value
                Ec::break_current = address;
                r->eax = old_break;
            }
            break;
        }