crate::triple_part! {
    Arm {
        Arm     , V4t     , V5      , V5te    , V6      ,
        V6m     , V6k     , V6t2    , V7      , V7em    ,
        V7m     , V7s     , V7k     , V7ve    , V8      ,
        V8_1a   , V8_2a   , V8_3a   , V8_4a   , V8_5a   ,
        V8_6a   , V8_7a   , V8_8a   , V8_9a   , V8r     ,
        V9      , V9_1a   , V9_2a   , V9_3a   , V9_4a   ,
        V9_5a   , V9_6a   , V9_7a   ,
    }

    canon(self) {
        match self {
            Arm::Arm    => "arm",
            Arm::V4t    => "armv4t",
            Arm::V5     => "armv5",
            Arm::V5te   => "armv5te",
            Arm::V6     => "armv6",
            Arm::V6m    => "armv6m",
            Arm::V6k    => "armv6k",
            Arm::V6t2   => "armv6t2",
            Arm::V7     => "armv7",
            Arm::V7em   => "armv7em",
            Arm::V7m    => "armv7m",
            Arm::V7s    => "armv7s",
            Arm::V7k    => "armv7k",
            Arm::V7ve   => "armv7ve",
            Arm::V8     => "armv8a",
            Arm::V8_1a  => "armv8.1a",
            Arm::V8_2a  => "armv8.2a",
            Arm::V8_3a  => "armv8.3a",
            Arm::V8_4a  => "armv8.4a",
            Arm::V8_5a  => "armv8.5a",
            Arm::V8_6a  => "armv8.6a",
            Arm::V8_7a  => "armv8.7a",
            Arm::V8_8a  => "armv8.8a",
            Arm::V8_9a  => "armv8.9a",
            Arm::V8r    => "armv8r",
            Arm::V9     => "armv9a",
            Arm::V9_1a  => "armv9.1a",
            Arm::V9_2a  => "armv9.2a",
            Arm::V9_3a  => "armv9.3a",
            Arm::V9_4a  => "armv9.4a",
            Arm::V9_5a  => "armv9.5a",
            Arm::V9_6a  => "armv9.6a",
            Arm::V9_7a  => "armv9.7a",
        }
    }

    parse(src) {
        if prefix!("armv9.7a" src) { return Some((Arm::V9_7a, 8)); }
        if prefix!("armv9.6a" src) { return Some((Arm::V9_6a, 8)); }
        if prefix!("armv9.5a" src) { return Some((Arm::V9_5a, 8)); }
        if prefix!("armv9.4a" src) { return Some((Arm::V9_4a, 8)); }
        if prefix!("armv9.3a" src) { return Some((Arm::V9_3a, 8)); }
        if prefix!("armv9.2a" src) { return Some((Arm::V9_2a, 8)); }
        if prefix!("armv9.1a" src) { return Some((Arm::V9_1a, 8)); }
        if prefix!("armv8.9a" src) { return Some((Arm::V8_9a, 8)); }
        if prefix!("armv8.8a" src) { return Some((Arm::V8_8a, 8)); }
        if prefix!("armv8.7a" src) { return Some((Arm::V8_7a, 8)); }
        if prefix!("armv8.6a" src) { return Some((Arm::V8_6a, 8)); }
        if prefix!("armv8.5a" src) { return Some((Arm::V8_5a, 8)); }
        if prefix!("armv8.4a" src) { return Some((Arm::V8_4a, 8)); }
        if prefix!("armv8.3a" src) { return Some((Arm::V8_3a, 8)); }
        if prefix!("armv8.2a" src) { return Some((Arm::V8_2a, 8)); }
        if prefix!("armv8.1a" src) { return Some((Arm::V8_1a, 8)); }
        if prefix!("armv7ve"  src) { return Some((Arm::V7ve,  7)); }
        if prefix!("armv7em"  src) { return Some((Arm::V7em,  7)); }
        if prefix!("armv6t2"  src) { return Some((Arm::V6t2,  7)); }
        if prefix!("armv5te"  src) { return Some((Arm::V5te,  7)); }
        if prefix!("armv9a"   src) { return Some((Arm::V9,    6)); }
        if prefix!("armv8r"   src) { return Some((Arm::V8r,   6)); }
        if prefix!("armv8a"   src) { return Some((Arm::V8,    6)); }
        if prefix!("armv7s"   src) { return Some((Arm::V7s,   6)); }
        if prefix!("armv7k"   src) { return Some((Arm::V7k,   6)); }
        if prefix!("armv7m"   src) { return Some((Arm::V7m,   6)); }
        if prefix!("armv6k"   src) { return Some((Arm::V6k,   6)); }
        if prefix!("armv6m"   src) { return Some((Arm::V6m,   6)); }
        if prefix!("armv4t"   src) { return Some((Arm::V4t,   6)); }
        if prefix!("armv7"    src) { return Some((Arm::V7,    5)); }
        if prefix!("armv6"    src) { return Some((Arm::V6,    5)); }
        if prefix!("armv5"    src) { return Some((Arm::V5,    5)); }
        if prefix!("arm"      src) { return Some((Arm::Arm,   3)); }
        None
    }

    bits(self) {
        if *self <= Arm::V8 { return 32; }
        64
    }
}
