crate::triple_part! {
    X86 {
        X86 ,
        I386,
        I486,
        I586,
        I686,
    }

    canon(self) {
        if *self == X86::X86 { return "i386"; }
        self.into()
    }

    parse(src) {
        if prefix!("x86"  src) { return Some((X86::X86 , 3)); }
        if prefix!("i386" src) { return Some((X86::I386, 4)); }
        if prefix!("i486" src) { return Some((X86::I486, 4)); }
        if prefix!("i586" src) { return Some((X86::I586, 4)); }
        if prefix!("i686" src) { return Some((X86::I686, 4)); }
        None
    }

    bits(self) {
        32
    }
}
