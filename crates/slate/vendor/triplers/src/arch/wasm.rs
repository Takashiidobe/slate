crate::triple_part! {
    Wasm {
        Wasm32,
        Wasm64,
    }

    canon(self) {
        match self {
            Wasm::Wasm32 => "wasm32",
            Wasm::Wasm64 => "wasm64",
        }
    }

    parse(src) {
        if prefix!("wasm32" src) { return Some((Wasm::Wasm32, 6)); }
        if prefix!("wasm64" src) { return Some((Wasm::Wasm64, 6)); }
        None
    }

    bits(self) {
        if *self == Wasm::Wasm32 { return 32; }
        64
    }
}
