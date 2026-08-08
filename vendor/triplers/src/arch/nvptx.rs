crate::triple_part! {
    Nvptx {
        Nvptx,
        Nvptx64,
    }

    canon(self) {
        match self {
            Nvptx::Nvptx => "nvptx",
            Nvptx::Nvptx64 => "nvptx64",
        }
    }

    parse(src) {
        if prefix!("nvptx" src) { return Some((Nvptx::Nvptx, 5)); }
        if prefix!("nvptx64" src) { return Some((Nvptx::Nvptx64, 7)); }
        None
    }

    bits(self) {
        if *self == Nvptx::Nvptx { return 32; }
        64
    }
}
