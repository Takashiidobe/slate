crate::triple_part! {
    Env {
        RayGeneration,
        Amplification,
        RootSignature,
        GNUEABIHFT64,
        Intersection,
        GNUEABIT64,
        MuslABIN32,
        MuslEABIHF,
        ClosestHit,
        GNUABIN32,
        GNUEABIHF,
        MuslABI64,
        Simulator,
        PAuthTest,
        GNUABI64,
        GNUILP32,
        MuslEABI,
        MuslWALI,
        Geometry,
        Callable,
        Unknown,
        GNUEABI,
        Android,
        MuslF32,
        MuslX32,
        Itanium,
        CoreCLR,
        Compute,
        Library,
        OpenHOS,
        GNUT64,
        GNUF32,
        GNUF64,
        GNUX32,
        CODE16,
        EABIHF,
        MuslSF,
        Cygnus,
        MacABI,
        Vertex,
        Domain,
        AnyHit,
        GNUSF,
        Pixel,
        Mlibc,
        EABI,
        Musl,
        LLVM,
        MSVC,
        Hull,
        Miss,
        Mesh,
        MTIA,
        Vaos,
        GNU,
    }

    canon(self) {
        self.into()
    }

    parse(src) {
        if prefix!("raygeneration"  src) { return Some((Env::RayGeneration,  13)); }
        if prefix!("amplification"  src) { return Some((Env::Amplification,  13)); }
        if prefix!("rootsignature"  src) { return Some((Env::RootSignature,  13)); }
        if prefix!("gnueabihft64"   src) { return Some((Env::GNUEABIHFT64,   12)); }
        if prefix!("intersection"   src) { return Some((Env::Intersection,   12)); }
        if prefix!("gnueabit64"     src) { return Some((Env::GNUEABIT64,     10)); }
        if prefix!("muslabin32"     src) { return Some((Env::MuslABIN32,     10)); }
        if prefix!("musleabihf"     src) { return Some((Env::MuslEABIHF,     10)); }
        if prefix!("closesthit"     src) { return Some((Env::ClosestHit,     10)); }
        if prefix!("gnuabin32"      src) { return Some((Env::GNUABIN32,       9)); }
        if prefix!("gnueabihf"      src) { return Some((Env::GNUEABIHF,       9)); }
        if prefix!("muslabi64"      src) { return Some((Env::MuslABI64,       9)); }
        if prefix!("simulator"      src) { return Some((Env::Simulator,       9)); }
        if prefix!("pauthtest"      src) { return Some((Env::PAuthTest,       9)); }
        if prefix!("gnuabi64"       src) { return Some((Env::GNUABI64,        8)); }
        if prefix!("gnuilp32"       src) { return Some((Env::GNUILP32,        8)); }
        if prefix!("musleabi"       src) { return Some((Env::MuslEABI,        8)); }
        if prefix!("muslwali"       src) { return Some((Env::MuslWALI,        8)); }
        if prefix!("geometry"       src) { return Some((Env::Geometry,        8)); }
        if prefix!("callable"       src) { return Some((Env::Callable,        8)); }
        if prefix!("gnueabi"        src) { return Some((Env::GNUEABI,         7)); }
        if prefix!("android"        src) { return Some((Env::Android,         7)); }
        if prefix!("muslf32"        src) { return Some((Env::MuslF32,         7)); }
        if prefix!("muslx32"        src) { return Some((Env::MuslX32,         7)); }
        if prefix!("itanium"        src) { return Some((Env::Itanium,         7)); }
        if prefix!("coreclr"        src) { return Some((Env::CoreCLR,         7)); }
        if prefix!("compute"        src) { return Some((Env::Compute,         7)); }
        if prefix!("library"        src) { return Some((Env::Library,         7)); }
        if prefix!("openhos"        src) { return Some((Env::OpenHOS,         7)); }
        if prefix!("unknown"        src) { return Some((Env::Unknown,         7)); }
        if prefix!("gnut64"         src) { return Some((Env::GNUT64,          6)); }
        if prefix!("gnuf32"         src) { return Some((Env::GNUF32,          6)); }
        if prefix!("gnuf64"         src) { return Some((Env::GNUF64,          6)); }
        if prefix!("gnux32"         src) { return Some((Env::GNUX32,          6)); }
        if prefix!("code16"         src) { return Some((Env::CODE16,          6)); }
        if prefix!("eabihf"         src) { return Some((Env::EABIHF,          6)); }
        if prefix!("muslsf"         src) { return Some((Env::MuslSF,          6)); }
        if prefix!("cygnus"         src) { return Some((Env::Cygnus,          6)); }
        if prefix!("macabi"         src) { return Some((Env::MacABI,          6)); }
        if prefix!("vertex"         src) { return Some((Env::Vertex,          6)); }
        if prefix!("domain"         src) { return Some((Env::Domain,          6)); }
        if prefix!("anyhit"         src) { return Some((Env::AnyHit,          6)); }
        if prefix!("gnusf"          src) { return Some((Env::GNUSF,           5)); }
        if prefix!("pixel"          src) { return Some((Env::Pixel,           5)); }
        if prefix!("mlibc"          src) { return Some((Env::Mlibc,           5)); }
        if prefix!("eabi"           src) { return Some((Env::EABI,            4)); }
        if prefix!("musl"           src) { return Some((Env::Musl,            4)); }
        if prefix!("llvm"           src) { return Some((Env::LLVM,            4)); }
        if prefix!("msvc"           src) { return Some((Env::MSVC,            4)); }
        if prefix!("hull"           src) { return Some((Env::Hull,            4)); }
        if prefix!("miss"           src) { return Some((Env::Miss,            4)); }
        if prefix!("mesh"           src) { return Some((Env::Mesh,            4)); }
        if prefix!("mtia"           src) { return Some((Env::MTIA,            4)); }
        if prefix!("vaos"           src) { return Some((Env::Vaos,            4)); }
        if prefix!("gnu"            src) { return Some((Env::GNU,             3)); }
        None
    }

    bits(self) { 0 }
}
