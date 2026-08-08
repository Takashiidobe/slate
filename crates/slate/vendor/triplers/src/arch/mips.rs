crate::triple_part! {
    Mips {
        Mips    ,
        Mips32R6,
        Mips64R6,
        MipsEL  ,
        Mips64  ,
        Mips64EL,
    }

    canon(self) {
        match self {
            Mips::Mips     => "mips"    ,
            Mips::Mips32R6 => "mips32r6",
            Mips::Mips64R6 => "mips64r6",
            Mips::MipsEL   => "mipsel"  ,
            Mips::Mips64   => "mips64"  ,
            Mips::Mips64EL => "mips64el",
        }
    }

    parse(src) {
        if prefix!("mips64el" src) { return Some((Mips::Mips64EL, 8)); }
        if prefix!("mips32r6" src) { return Some((Mips::Mips32R6, 8)); }
        if prefix!("mips64r6" src) { return Some((Mips::Mips64R6, 8)); }
        if prefix!("mips64"   src) { return Some((Mips::Mips64  , 6)); }
        if prefix!("mipsel"   src) { return Some((Mips::MipsEL  , 6)); }
        if prefix!("mips"     src) { return Some((Mips::Mips    , 4)); }
        None
    }

    bits(self) {
        if *self == Mips::Mips64 || *self == Mips::Mips64EL || *self == Mips::Mips64R6 { return 64; }
        32
    }
}
