crate::triple_part! {
    Kernel {
        ShaderModel,
        CheriotRTOS,
        HermitCore,
        Emscripten,
        DragonFly,
        DriverKit,
        SolidAsp3,
        KFreeBSD,
        Managarm,
        ELFIAMCU,
        BridgeOS,
        Serenity,
        ChipStar,
        Firmware,
        VisionOS,
        FreeBSD,
        Fuchsia,
        OpenBSD,
        Solaris,
        WatchOS,
        Horizon,
        Illumos,
        Vxworks,
        Darwin,
        MacOSX,
        NetBSD,
        AMDHSA,
        Mesa3D,
        AMDPAL,
        WASIp1,
        WASIp2,
        WASIp3,
        LiteOS,
        Vulkan,
        OpenCL,
        VexOS,
        Redox,
        Linux,
        Win32,
        Haiku,
        RTEMS,
        None,
        UEFI,
        CUDA,
        NVCL,
        TvOS,
        XROS,
        Hurd,
        WASI,
        QURT,
        KVAK,
        L4re,
        Vita,
        XOUS,
        IOS,
        NTO,
        Lv2,
        ZOS,
        AIX,
        PS4,
        PS5,
        H2,
    }

    canon(self) {
        if *self == Kernel::Win32 { return "windows"; }
        self.into()
    }

    parse(src) {
        if prefix!("shadermodel" src) { return Some((Kernel::ShaderModel, 11)); }
        if prefix!("cheriotrtos" src) { return Some((Kernel::CheriotRTOS, 11)); }
        if prefix!("hermitcore"  src) { return Some((Kernel::HermitCore,  10)); }
        if prefix!("emscripten"  src) { return Some((Kernel::Emscripten,  10)); }
        if prefix!("dragonfly"   src) { return Some((Kernel::DragonFly,    9)); }
        if prefix!("driverkit"   src) { return Some((Kernel::DriverKit,    9)); }
        if prefix!("solid_asp3"  src) { return Some((Kernel::SolidAsp3,    9)); }
        if prefix!("kfreebsd"    src) { return Some((Kernel::KFreeBSD,     8)); }
        if prefix!("managarm"    src) { return Some((Kernel::Managarm,     8)); }
        if prefix!("elfiamcu"    src) { return Some((Kernel::ELFIAMCU,     8)); }
        if prefix!("bridgeos"    src) { return Some((Kernel::BridgeOS,     8)); }
        if prefix!("serenity"    src) { return Some((Kernel::Serenity,     8)); }
        if prefix!("chipstar"    src) { return Some((Kernel::ChipStar,     8)); }
        if prefix!("firmware"    src) { return Some((Kernel::Firmware,     8)); }
        if prefix!("visionos"    src) { return Some((Kernel::VisionOS,     8)); }
        if prefix!("windows"     src) { return Some((Kernel::Win32,        7)); }
        if prefix!("freebsd"     src) { return Some((Kernel::FreeBSD,      7)); }
        if prefix!("fuchsia"     src) { return Some((Kernel::Fuchsia,      7)); }
        if prefix!("openbsd"     src) { return Some((Kernel::OpenBSD,      7)); }
        if prefix!("solaris"     src) { return Some((Kernel::Solaris,      7)); }
        if prefix!("watchos"     src) { return Some((Kernel::WatchOS,      7)); }
        if prefix!("horizon"     src) { return Some((Kernel::Horizon,      7)); }
        if prefix!("illumos"     src) { return Some((Kernel::Illumos,      7)); }
        if prefix!("vxworks"     src) { return Some((Kernel::Vxworks,      7)); }
        if prefix!("darwin"      src) { return Some((Kernel::Darwin,       6)); }
        if prefix!("macosx"      src) { return Some((Kernel::MacOSX,       6)); }
        if prefix!("netbsd"      src) { return Some((Kernel::NetBSD,       6)); }
        if prefix!("amdhsa"      src) { return Some((Kernel::AMDHSA,       6)); }
        if prefix!("mesa3d"      src) { return Some((Kernel::Mesa3D,       6)); }
        if prefix!("amdpal"      src) { return Some((Kernel::AMDPAL,       6)); }
        if prefix!("wasip3"      src) { return Some((Kernel::WASIp3,       6)); }
        if prefix!("wasip2"      src) { return Some((Kernel::WASIp2,       6)); }
        if prefix!("wasip1"      src) { return Some((Kernel::WASIp1,       6)); }
        if prefix!("liteos"      src) { return Some((Kernel::LiteOS,       6)); }
        if prefix!("vulkan"      src) { return Some((Kernel::Vulkan,       6)); }
        if prefix!("opencl"      src) { return Some((Kernel::OpenCL,       6)); }
        if prefix!("vexos"       src) { return Some((Kernel::VexOS,        5)); }
        if prefix!("redox"       src) { return Some((Kernel::Redox,        5)); }
        if prefix!("linux"       src) { return Some((Kernel::Linux,        5)); }
        if prefix!("win32"       src) { return Some((Kernel::Win32,        5)); }
        if prefix!("haiku"       src) { return Some((Kernel::Haiku,        5)); }
        if prefix!("rtems"       src) { return Some((Kernel::RTEMS,        5)); }
        if prefix!("none"        src) { return Some((Kernel::None,         4)); }
        if prefix!("uefi"        src) { return Some((Kernel::UEFI,         4)); }
        if prefix!("cuda"        src) { return Some((Kernel::CUDA,         4)); }
        if prefix!("nvcl"        src) { return Some((Kernel::NVCL,         4)); }
        if prefix!("tvos"        src) { return Some((Kernel::TvOS,         4)); }
        if prefix!("xros"        src) { return Some((Kernel::XROS,         4)); }
        if prefix!("hurd"        src) { return Some((Kernel::Hurd,         4)); }
        if prefix!("wasi"        src) { return Some((Kernel::WASI,         4)); }
        if prefix!("qurt"        src) { return Some((Kernel::QURT,         4)); }
        if prefix!("kvak"        src) { return Some((Kernel::KVAK,         4)); }
        if prefix!("l4re"        src) { return Some((Kernel::L4re,         4)); }
        if prefix!("vita"        src) { return Some((Kernel::Vita,         4)); }
        if prefix!("xous"        src) { return Some((Kernel::XOUS,         4)); }
        if prefix!("ios"         src) { return Some((Kernel::IOS,          3)); }
        if prefix!("nto"         src) { return Some((Kernel::NTO,          3)); }
        if prefix!("lv2"         src) { return Some((Kernel::Lv2,          3)); }
        if prefix!("zos"         src) { return Some((Kernel::ZOS,          3)); }
        if prefix!("aix"         src) { return Some((Kernel::AIX,          3)); }
        if prefix!("ps4"         src) { return Some((Kernel::PS4,          3)); }
        if prefix!("ps5"         src) { return Some((Kernel::PS5,          3)); }
        if prefix!("h2"          src) { return Some((Kernel::H2,           2)); }
        None
    }

    bits(self) { 0 }
}
