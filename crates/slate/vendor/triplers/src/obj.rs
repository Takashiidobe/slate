crate::triple_part! {
    Obj {
        DXContainer,
        MachO,
        SPIRV,
        XCOFF,
        COFF,
        GOFF,
        Wasm,
        ELF,
    }

    canon(self) {
        self.into()
    }

    parse(src) {
        if prefix!("dxcontainer" src) { return Some((Obj::DXContainer, 11)); }
        if prefix!("macho"       src) { return Some((Obj::MachO,       5)); }
        if prefix!("spirv"       src) { return Some((Obj::SPIRV,       5)); }
        if prefix!("xcoff"       src) { return Some((Obj::XCOFF,       5)); }
        if prefix!("coff"        src) { return Some((Obj::COFF,        4)); }
        if prefix!("goff"        src) { return Some((Obj::GOFF,        4)); }
        if prefix!("wasm"        src) { return Some((Obj::Wasm,        4)); }
        if prefix!("elf"         src) { return Some((Obj::ELF,         3)); }
        None
    }

    bits(self) { 0 }
}
