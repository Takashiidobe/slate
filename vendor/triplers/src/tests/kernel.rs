use crate::Parsable;
use crate::kernel::Kernel;

#[test]
fn test_kernel_parse() {
    let cases = [
        ("shadermodel", Kernel::ShaderModel, 11),
        ("cheriotrtos", Kernel::CheriotRTOS, 11),
        ("hermitcore", Kernel::HermitCore, 10),
        ("emscripten", Kernel::Emscripten, 10),
        ("dragonfly", Kernel::DragonFly, 9),
        ("driverkit", Kernel::DriverKit, 9),
        ("kfreebsd", Kernel::KFreeBSD, 8),
        ("managarm", Kernel::Managarm, 8),
        ("elfiamcu", Kernel::ELFIAMCU, 8),
        ("bridgeos", Kernel::BridgeOS, 8),
        ("serenity", Kernel::Serenity, 8),
        ("chipstar", Kernel::ChipStar, 8),
        ("firmware", Kernel::Firmware, 8),
        ("freebsd", Kernel::FreeBSD, 7),
        ("fuchsia", Kernel::Fuchsia, 7),
        ("openbsd", Kernel::OpenBSD, 7),
        ("solaris", Kernel::Solaris, 7),
        ("watchos", Kernel::WatchOS, 7),
        ("darwin", Kernel::Darwin, 6),
        ("macosx", Kernel::MacOSX, 6),
        ("netbsd", Kernel::NetBSD, 6),
        ("amdhsa", Kernel::AMDHSA, 6),
        ("mesa3d", Kernel::Mesa3D, 6),
        ("amdpal", Kernel::AMDPAL, 6),
        ("wasip3", Kernel::WASIp3, 6),
        ("wasip2", Kernel::WASIp2, 6),
        ("wasip1", Kernel::WASIp1, 6),
        ("liteos", Kernel::LiteOS, 6),
        ("vulkan", Kernel::Vulkan, 6),
        ("opencl", Kernel::OpenCL, 6),
        ("linux", Kernel::Linux, 5),
        ("win32", Kernel::Win32, 5),
        ("haiku", Kernel::Haiku, 5),
        ("rtems", Kernel::RTEMS, 5),
        ("none", Kernel::None, 4),
        ("uefi", Kernel::UEFI, 4),
        ("cuda", Kernel::CUDA, 4),
        ("nvcl", Kernel::NVCL, 4),
        ("tvos", Kernel::TvOS, 4),
        ("xros", Kernel::XROS, 4),
        ("hurd", Kernel::Hurd, 4),
        ("wasi", Kernel::WASI, 4),
        ("qurt", Kernel::QURT, 4),
        ("kvak", Kernel::KVAK, 4),
        ("ios", Kernel::IOS, 3),
        ("lv2", Kernel::Lv2, 3),
        ("zos", Kernel::ZOS, 3),
        ("aix", Kernel::AIX, 3),
        ("ps4", Kernel::PS4, 3),
        ("ps5", Kernel::PS5, 3),
        ("h2", Kernel::H2, 2),
    ];
    for (input, expected, expected_len) in cases {
        let (kernel, len) = Kernel::reduce(input).unwrap();
        assert_eq!(kernel, expected);
        assert_eq!(len, expected_len);
        assert_eq!(<Kernel as Into<&str>>::into(kernel), input);
    }
}
