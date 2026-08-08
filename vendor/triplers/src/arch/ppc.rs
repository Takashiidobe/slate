crate::triple_part! {
    Ppc {
        Ppc    ,
        PpcEL  ,
        Ppc64  ,
        PpcSPE ,
        Ppc64EL,
    }

    canon(self) {
        match self {
            Ppc::Ppc     => "powerpc"    ,
            Ppc::PpcEL   => "powerpcel"  ,
            Ppc::Ppc64   => "powerpc64"  ,
            Ppc::PpcSPE  => "powerpcspe" ,
            Ppc::Ppc64EL => "powerpc64el",
        }
    }

    parse(src) {
        if prefix!("powerpc64el" src) { return Some((Ppc::Ppc64EL, 11)); }
        if prefix!("powerpcspe"  src) { return Some((Ppc::PpcSPE,  10)); }
        if prefix!("powerpc64"   src) { return Some((Ppc::Ppc64,   9)); }
        if prefix!("powerpcel"   src) { return Some((Ppc::PpcEL,   9)); }
        if prefix!("powerpc"     src) { return Some((Ppc::Ppc,     7)); }
        if prefix!("ppc64el" src) { return Some((Ppc::Ppc64EL, 7)); }
        if prefix!("ppcspe"  src) { return Some((Ppc::PpcSPE,  6)); }
        if prefix!("ppc64"   src) { return Some((Ppc::Ppc64,   5)); }
        if prefix!("ppcel"   src) { return Some((Ppc::PpcEL,   5)); }
        if prefix!("ppc"     src) { return Some((Ppc::Ppc,     3)); }
        None
    }

    bits(self) {
        if *self == Ppc::Ppc64 || *self == Ppc::Ppc64EL { return 64; }
        32
    }
}
