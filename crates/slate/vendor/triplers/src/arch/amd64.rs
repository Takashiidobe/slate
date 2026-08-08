crate::triple_part! {
    Amd64 {
        Amd64,
        X86_64,
        X64,
        Em64t,
    }

    canon(self) {
        "x86_64"
    }

    parse(src) {
        if prefix!("x86_64" src) { return Some((Amd64::X86_64, 6)); }
        if prefix!("amd64"  src) { return Some((Amd64::Amd64 , 5)); }
        if prefix!("em64t"  src) { return Some((Amd64::Em64t , 5)); }
        if prefix!("x64"    src) { return Some((Amd64::X64   , 3)); }
        None
    }

    bits(self) {
        64
    }
}
