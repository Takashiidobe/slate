use crate::env::Env;
use crate::{Canonicalizable, Parsable};

#[test]
fn test_env_parse() {
    let cases = [
        ("raygeneration", Env::RayGeneration, 13),
        ("amplification", Env::Amplification, 13),
        ("rootsignature", Env::RootSignature, 13),
        ("gnueabihft64", Env::GNUEABIHFT64, 12),
        ("intersection", Env::Intersection, 12),
        ("gnueabit64", Env::GNUEABIT64, 10),
        ("muslabin32", Env::MuslABIN32, 10),
        ("musleabihf", Env::MuslEABIHF, 10),
        ("closesthit", Env::ClosestHit, 10),
        ("gnuabin32", Env::GNUABIN32, 9),
        ("gnueabihf", Env::GNUEABIHF, 9),
        ("muslabi64", Env::MuslABI64, 9),
        ("simulator", Env::Simulator, 9),
        ("pauthtest", Env::PAuthTest, 9),
        ("gnuabi64", Env::GNUABI64, 8),
        ("gnuilp32", Env::GNUILP32, 8),
        ("musleabi", Env::MuslEABI, 8),
        ("muslwali", Env::MuslWALI, 8),
        ("geometry", Env::Geometry, 8),
        ("callable", Env::Callable, 8),
        ("gnueabi", Env::GNUEABI, 7),
        ("android", Env::Android, 7),
        ("muslf32", Env::MuslF32, 7),
        ("muslx32", Env::MuslX32, 7),
        ("itanium", Env::Itanium, 7),
        ("coreclr", Env::CoreCLR, 7),
        ("compute", Env::Compute, 7),
        ("library", Env::Library, 7),
        ("openhos", Env::OpenHOS, 7),
        ("unknown", Env::Unknown, 7),
        ("gnut64", Env::GNUT64, 6),
        ("gnuf32", Env::GNUF32, 6),
        ("gnuf64", Env::GNUF64, 6),
        ("gnux32", Env::GNUX32, 6),
        ("code16", Env::CODE16, 6),
        ("eabihf", Env::EABIHF, 6),
        ("muslsf", Env::MuslSF, 6),
        ("cygnus", Env::Cygnus, 6),
        ("macabi", Env::MacABI, 6),
        ("vertex", Env::Vertex, 6),
        ("domain", Env::Domain, 6),
        ("anyhit", Env::AnyHit, 6),
        ("gnusf", Env::GNUSF, 5),
        ("pixel", Env::Pixel, 5),
        ("mlibc", Env::Mlibc, 5),
        ("eabi", Env::EABI, 4),
        ("musl", Env::Musl, 4),
        ("llvm", Env::LLVM, 4),
        ("msvc", Env::MSVC, 4),
        ("hull", Env::Hull, 4),
        ("miss", Env::Miss, 4),
        ("mesh", Env::Mesh, 4),
        ("mtia", Env::MTIA, 4),
        ("vaos", Env::Vaos, 4),
        ("gnu", Env::GNU, 3),
    ];
    for (input, expected, expected_len) in cases {
        let (env, len) = Env::reduce(input).unwrap();
        assert_eq!(env, expected);
        assert_eq!(len, expected_len);
        assert_eq!(env.canonicalize(), input);
    }
}
