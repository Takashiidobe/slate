crate::triple_part! {
    Dxil {
        Dxil,
        V1_0,
        V1_1,
        V1_2,
        V1_3,
        V1_4,
        V1_5,
        V1_6,
        V1_7,
        V1_8,
        V1_9,
    }

    canon(self) {
        match self {
            Dxil::Dxil => "dxil",
            Dxil::V1_0 => "dxil1.0",
            Dxil::V1_1 => "dxil1.1",
            Dxil::V1_2 => "dxil1.2",
            Dxil::V1_3 => "dxil1.3",
            Dxil::V1_4 => "dxil1.4",
            Dxil::V1_5 => "dxil1.5",
            Dxil::V1_6 => "dxil1.6",
            Dxil::V1_7 => "dxil1.7",
            Dxil::V1_8 => "dxil1.8",
            Dxil::V1_9 => "dxil1.9",
        }
    }

    parse(src) {
        if prefix!("dxil1.9" src) { return Some((Dxil::V1_9, 7)); }
        if prefix!("dxil1.8" src) { return Some((Dxil::V1_8, 7)); }
        if prefix!("dxil1.7" src) { return Some((Dxil::V1_7, 7)); }
        if prefix!("dxil1.6" src) { return Some((Dxil::V1_6, 7)); }
        if prefix!("dxil1.5" src) { return Some((Dxil::V1_5, 7)); }
        if prefix!("dxil1.4" src) { return Some((Dxil::V1_4, 7)); }
        if prefix!("dxil1.3" src) { return Some((Dxil::V1_3, 7)); }
        if prefix!("dxil1.2" src) { return Some((Dxil::V1_2, 7)); }
        if prefix!("dxil1.1" src) { return Some((Dxil::V1_1, 7)); }
        if prefix!("dxil1.0" src) { return Some((Dxil::V1_0, 7)); }
        if prefix!("dxil"    src) { return Some((Dxil::Dxil, 4)); }
        None
    }

    bits(self) {
        0
    }
}
