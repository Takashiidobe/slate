crate::triple_part! {
    SpirV {
        SpirV,
        V10,
        V11,
        V12,
        V13,
        V14,
        V15,
        V16,
    }

    canon(self) {
        match self {
            SpirV::SpirV => "spirv",
            SpirV::V10   => "spirv1.0",
            SpirV::V11   => "spirv1.1",
            SpirV::V12   => "spirv1.2",
            SpirV::V13   => "spirv1.3",
            SpirV::V14   => "spirv1.4",
            SpirV::V15   => "spirv1.5",
            SpirV::V16   => "spirv1.6",
        }
    }

    parse(src) {
        if prefix!("spirv1.6" src) { return Some((SpirV::V16, 8)); }
        if prefix!("spirv1.5" src) { return Some((SpirV::V15, 8)); }
        if prefix!("spirv1.4" src) { return Some((SpirV::V14, 8)); }
        if prefix!("spirv1.3" src) { return Some((SpirV::V13, 8)); }
        if prefix!("spirv1.2" src) { return Some((SpirV::V12, 8)); }
        if prefix!("spirv1.1" src) { return Some((SpirV::V11, 8)); }
        if prefix!("spirv1.0" src) { return Some((SpirV::V10, 8)); }
        if prefix!("spirv"    src) { return Some((SpirV::SpirV, 5)); }
        None
    }

    bits(self) {
        0
    }
}
