crate::triple_part! {
    Aarch64 {
        Aarch64,
        Arm64e,
        Arm64ec,
        Arm64,
    }

    canon(self) {
        match self {
            Aarch64::Arm64   => "aarch64",
            Aarch64::Aarch64 => "aarch64",
            Aarch64::Arm64e  => "arm64e" ,
            Aarch64::Arm64ec => "arm64ec",
        }
    }

    parse(src) {
        if prefix!("arm64ec" src) { return Some((Aarch64::Arm64ec, 7)); }
        if prefix!("aarch64" src) { return Some((Aarch64::Aarch64, 7)); }
        if prefix!("arm64e"  src) { return Some((Aarch64::Arm64e , 6)); }
        if prefix!("arm64"   src) { return Some((Aarch64::Arm64  , 5)); }
        None
    }

    bits(self) {
        64
    }
}
