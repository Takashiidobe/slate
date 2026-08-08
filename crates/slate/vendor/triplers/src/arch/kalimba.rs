crate::triple_part! {
    Kalimba {
        Kalimba ,
        V3      ,
        V4      ,
        V5      ,
    }

    canon(self) {
        match self {
            Kalimba::Kalimba => "kalimba",
            Kalimba::V3      => "kalimba3",
            Kalimba::V4      => "kalimba4",
            Kalimba::V5      => "kalimba5",
        }
    }

    parse(src) {
        if prefix!("kalimba5" src) { return Some((Kalimba::V5, 8)); }
        if prefix!("kalimba4" src) { return Some((Kalimba::V4, 8)); }
        if prefix!("kalimba3" src) { return Some((Kalimba::V3, 8)); }
        if prefix!("kalimba"  src) { return Some((Kalimba::Kalimba, 7)); }
        None
    }

    bits(self) {
        if *self == Kalimba::Kalimba { return 16; }
        if *self == Kalimba::V3 { return 24; }
        32
    }
}
