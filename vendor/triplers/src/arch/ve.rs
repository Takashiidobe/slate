crate::triple_part! {
    Ve {
        Ve
    }

    canon(self) {
        "ve"
    }

    parse(src) {
        if prefix!("ve" src) { return Some((Ve::Ve, 2)); }
        None
    }

    bits(self) {
        64
    }
}
