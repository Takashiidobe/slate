crate::triple_part! {
    Riscv {
        Riscv32,
        Riscv64,
    }

    canon(self) {
        self.into()
    }

    parse(src) {
        if prefix!("riscv32" src) { return Some((Riscv::Riscv32, 7)); }
        if prefix!("riscv64" src) { return Some((Riscv::Riscv64, 7)); }
        None
    }

    bits(self) {
        match self {
            Riscv::Riscv32 => 32,
            Riscv::Riscv64 => 64,
        }
    }
}
