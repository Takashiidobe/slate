use crate::install::{install_staged_at, staging_path};
use crate::{DoctorCheck, Paths, Target};
use std::collections::{HashMap, HashSet};
use std::env;
use std::ffi::OsStr;
use std::fs::{self, File};
use std::io::{self, Read};
use std::path::{Path, PathBuf};
use std::process::Command;

pub const CLANG_VERSION: &str = "22.1.8";
pub const GCC_VERSION: &str = "16.2.0";
const GCC_SHA256: &str = "e6738e29597f733270731aa90600f37ffdc045079dfc27ec7e8192cc81085c3e";

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum CompilerHeaders {
    Clang,
    AppleClang,
    Gcc(GccFamily),
    Msvc(Target),
}

impl CompilerHeaders {
    pub fn name(self) -> &'static str {
        match self {
            Self::Clang => "clang",
            Self::AppleClang => "apple-clang",
            Self::Gcc(_) => "gcc",
            Self::Msvc(_) => "msvc",
        }
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum GccFamily {
    X86,
    Aarch64,
    Arm,
}

impl GccFamily {
    pub const ALL: [Self; 3] = [Self::X86, Self::Aarch64, Self::Arm];

    pub fn name(self) -> &'static str {
        match self {
            Self::X86 => "x86",
            Self::Aarch64 => "aarch64",
            Self::Arm => "arm",
        }
    }

    pub fn of(target: Target) -> Self {
        match target {
            Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::X86_64UnknownLinuxGnu
            | Target::I686UnknownLinuxGnu
            | Target::X86_64UnknownLinuxMusl
            | Target::X86_64AppleDarwin
            | Target::X86_64UnknownFreebsd
            | Target::X86_64LinuxAndroid => Self::X86,
            Target::Aarch64PcWindowsMsvc
            | Target::Aarch64UnknownLinuxGnu
            | Target::Aarch64UnknownLinuxMusl
            | Target::Aarch64AppleDarwin
            | Target::Aarch64UnknownFreebsd
            | Target::Aarch64LinuxAndroid => Self::Aarch64,
            Target::Thumbv7aPcWindowsMsvc
            | Target::Armv7UnknownLinuxGnueabi
            | Target::Armv7UnknownLinuxGnueabihf => Self::Arm,
        }
    }

    fn config_dir(self) -> &'static str {
        match self {
            Self::X86 => "gcc/config/i386",
            Self::Aarch64 => "gcc/config/aarch64",
            Self::Arm => "gcc/config/arm",
        }
    }

    fn extra_headers(self) -> &'static [&'static str] {
        match self {
            Self::X86 => GCC_X86_HEADERS,
            Self::Aarch64 => GCC_AARCH64_HEADERS,
            Self::Arm => GCC_ARM_HEADERS,
        }
    }

    fn assembled_headers(self) -> &'static [(&'static str, &'static str)] {
        match self {
            Self::X86 => &[
                ("unwind.h", "libgcc/unwind-generic.h"),
                ("mm_malloc.h", "gcc/config/i386/pmm_malloc.h"),
            ],
            Self::Aarch64 => &[("unwind.h", "libgcc/unwind-generic.h")],
            Self::Arm => &[
                ("unwind.h", "libgcc/config/arm/unwind-arm.h"),
                ("unwind-arm-common.h", "gcc/ginclude/unwind-arm-common.h"),
            ],
        }
    }

    fn required_headers(self) -> &'static [&'static str] {
        match self {
            Self::X86 => &["immintrin.h", "cpuid.h", "mm_malloc.h"],
            Self::Aarch64 => &["arm_neon.h", "arm_sve.h"],
            Self::Arm => &["arm_neon.h", "unwind-arm-common.h"],
        }
    }

    fn outputs(self) -> Vec<(&'static str, Vec<String>)> {
        GCC_COMMON_HEADERS
            .iter()
            .map(|(output, sources)| {
                (
                    *output,
                    sources.iter().map(|source| source.to_string()).collect(),
                )
            })
            .chain(
                self.extra_headers()
                    .iter()
                    .map(|header| (*header, vec![format!("{}/{header}", self.config_dir())])),
            )
            .chain(
                self.assembled_headers()
                    .iter()
                    .map(|(output, source)| (*output, vec![source.to_string()])),
            )
            .collect()
    }
}

impl std::str::FromStr for GccFamily {
    type Err = io::Error;

    fn from_str(value: &str) -> io::Result<Self> {
        Self::ALL
            .into_iter()
            .find(|family| family.name() == value)
            .ok_or_else(|| {
                io::Error::new(
                    io::ErrorKind::InvalidInput,
                    format!("unsupported GCC header family: {value}"),
                )
            })
    }
}

impl Paths {
    pub fn compiler_header_path(&self, compiler: CompilerHeaders) -> PathBuf {
        match compiler {
            CompilerHeaders::Clang => self
                .data
                .join("compiler-headers")
                .join(format!("clang-{CLANG_VERSION}"))
                .join("include"),
            CompilerHeaders::AppleClang => self
                .data
                .join("compiler-headers/apple-clang-current/include"),
            CompilerHeaders::Gcc(family) => {
                self.gcc_bundle_path().join(family.name()).join("include")
            }
            CompilerHeaders::Msvc(target) => self.sysroot_path(target).join("crt/include"),
        }
    }

    fn gcc_bundle_path(&self) -> PathBuf {
        self.data
            .join("compiler-headers")
            .join(format!("gcc-{GCC_VERSION}"))
    }

    pub fn resolve_compiler_headers(&self, compiler: CompilerHeaders) -> io::Result<PathBuf> {
        let path = self.compiler_header_path(compiler);
        validate(compiler, &path)?;
        Ok(path)
    }

    pub fn doctor_compiler_headers(&self, compiler: CompilerHeaders) -> Vec<DoctorCheck> {
        checks(compiler, self.compiler_header_path(compiler))
    }

    pub fn install_compiler_headers(&self, compiler: CompilerHeaders) -> io::Result<PathBuf> {
        match compiler {
            CompilerHeaders::Msvc(target) => {
                if !is_msvc_target(target) {
                    return Err(io::Error::new(
                        io::ErrorKind::InvalidInput,
                        format!("{} is not a Windows MSVC target", target.triple()),
                    ));
                }
                self.install(target)?;
                self.resolve_compiler_headers(compiler)
            }
            CompilerHeaders::AppleClang => {
                install_apple_clang(self, env::consts::OS, OsStr::new("xcrun"))
            }
            CompilerHeaders::Clang => {
                let include = self.compiler_header_path(compiler);
                let output = include.parent().expect("compiler header path has a parent");
                install_staged_at(
                    output,
                    |root| validate(compiler, &root.join("include")),
                    install_clang,
                )?;
                Ok(include)
            }
            CompilerHeaders::Gcc(_) => {
                install_staged_at(
                    &self.gcc_bundle_path(),
                    |root| {
                        GccFamily::ALL.into_iter().try_for_each(|family| {
                            validate(
                                CompilerHeaders::Gcc(family),
                                &root.join(family.name()).join("include"),
                            )
                        })
                    },
                    |root| install_gcc(&self.cache, root),
                )?;
                Ok(self.compiler_header_path(compiler))
            }
        }
    }
}

fn is_msvc_target(target: Target) -> bool {
    matches!(
        target,
        Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::Aarch64PcWindowsMsvc
            | Target::Thumbv7aPcWindowsMsvc
    )
}

fn checks(compiler: CompilerHeaders, path: PathBuf) -> Vec<DoctorCheck> {
    let (label, files): (&'static str, &[&str]) = match compiler {
        CompilerHeaders::Clang => (
            "Clang resource headers",
            &["stdarg.h", "stddef.h", "immintrin.h"],
        ),
        CompilerHeaders::AppleClang => (
            "Apple Clang resource headers",
            &["stdarg.h", "stddef.h", "immintrin.h"],
        ),
        CompilerHeaders::Gcc(family) => (
            match family {
                GccFamily::X86 => "GCC x86 compiler headers",
                GccFamily::Aarch64 => "GCC aarch64 compiler headers",
                GccFamily::Arm => "GCC arm compiler headers",
            },
            &[
                "stdarg.h",
                "stddef.h",
                "stdint.h",
                "limits.h",
                "syslimits.h",
                "unwind.h",
            ],
        ),
        CompilerHeaders::Msvc(_) => ("MSVC compiler headers", &["vcruntime.h", "yvals_core.h"]),
    };
    let family_files = match compiler {
        CompilerHeaders::Gcc(family) => family.required_headers(),
        _ => &[],
    };
    let license_files: &[&str] = match compiler {
        CompilerHeaders::Clang => &["LICENSE.TXT"],
        CompilerHeaders::AppleClang => &["COMPILER-HEADERS-MANIFEST.txt"],
        CompilerHeaders::Gcc(_) => &["COPYING3", "COPYING.RUNTIME"],
        CompilerHeaders::Msvc(_) => &[],
    };
    let license_dir = match compiler {
        CompilerHeaders::Gcc(_) => path.parent().and_then(Path::parent),
        _ => path.parent(),
    };
    let present = path.is_dir()
        && files
            .iter()
            .chain(family_files)
            .all(|file| path.join(file).is_file())
        && license_files
            .iter()
            .all(|file| license_dir.is_some_and(|parent| parent.join(file).is_file()));
    vec![DoctorCheck {
        label,
        path,
        present,
    }]
}

pub(crate) fn install_apple_clang(
    paths: &Paths,
    host_os: &str,
    xcrun: &OsStr,
) -> io::Result<PathBuf> {
    if host_os != "macos" {
        return Err(io::Error::new(
            io::ErrorKind::Unsupported,
            "Apple Clang headers can only be acquired from Xcode or Command Line Tools on macOS",
        ));
    }
    let clang = command_output(xcrun, &["--find", "clang"])?;
    let clang = PathBuf::from(clang.trim());
    let resource = command_output(clang.as_os_str(), &["-print-resource-dir"])?;
    let resource = PathBuf::from(resource.trim());
    let version_info = command_output(clang.as_os_str(), &["--version"])?;
    let version = apple_clang_version(&version_info)?;
    let source = resource.join("include");
    if !source.is_dir() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "Apple Clang resource headers are missing at {}",
                source.display()
            ),
        ));
    }
    let parent = paths.data.join("compiler-headers");
    let output = parent.join(format!("apple-clang-{version}"));
    install_staged_at(
        &output,
        |root| validate(CompilerHeaders::AppleClang, &root.join("include")),
        |root| {
            copy_tree(&source, &root.join("include"))?;
            fs::write(
                root.join("COMPILER-HEADERS-MANIFEST.txt"),
                format!(
                    "Compiler: Apple Clang\nVersion: {version}\nCompiler path: {}\nResource directory: {}\nAcquisition: copied from locally installed Xcode or Command Line Tools\nDistribution: local use only; Apple toolchain license applies\n",
                    clang.display(),
                    resource.display()
                ),
            )?;
            Ok(())
        },
    )?;
    let current = parent.join("apple-clang-current");
    let staging = staging_path(&parent, "apple-clang-current")?;
    link_current(&output, &staging)?;
    if let Err(error) = fs::rename(&staging, &current) {
        let _ = fs::remove_file(&staging);
        return Err(error);
    }
    paths.resolve_compiler_headers(CompilerHeaders::AppleClang)
}

#[cfg(unix)]
fn link_current(output: &Path, link: &Path) -> io::Result<()> {
    std::os::unix::fs::symlink(output.file_name().expect("bundle has a name"), link)
}

#[cfg(windows)]
fn link_current(_output: &Path, _link: &Path) -> io::Result<()> {
    Err(io::Error::new(
        io::ErrorKind::Unsupported,
        "Apple Clang requires macOS",
    ))
}

fn command_output(command: &OsStr, args: &[&str]) -> io::Result<String> {
    let output = Command::new(command).args(args).output().map_err(|error| {
        io::Error::new(
            error.kind(),
            format!("could not run {}: {error}", command.to_string_lossy()),
        )
    })?;
    if !output.status.success() {
        return Err(io::Error::other(format!(
            "{} failed with {}",
            command.to_string_lossy(),
            output.status
        )));
    }
    String::from_utf8(output.stdout).map_err(io::Error::other)
}

fn apple_clang_version(info: &str) -> io::Result<String> {
    let line = info.lines().find_map(|line| {
        line.split_once("Apple clang version ")
            .map(|(_, rest)| rest)
    });
    let line = line.ok_or_else(|| {
        io::Error::new(
            io::ErrorKind::InvalidData,
            "could not determine Apple Clang version",
        )
    })?;
    let version = line.split_whitespace().next().unwrap_or_default();
    let build = line
        .split_once("(clang-")
        .and_then(|(_, rest)| rest.split_once(')').map(|(build, _)| build));
    let label = match build {
        Some(build) => format!("{version}-clang-{build}"),
        None => version.to_owned(),
    };
    if label.is_empty()
        || !label
            .chars()
            .all(|c| c.is_ascii_alphanumeric() || "._+-".contains(c))
    {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            "invalid Apple Clang version label",
        ));
    }
    Ok(label)
}

fn validate(compiler: CompilerHeaders, path: &Path) -> io::Result<()> {
    if let CompilerHeaders::Msvc(target) = compiler
        && !is_msvc_target(target)
    {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "expected an MSVC target",
        ));
    }
    let check = checks(compiler, path.to_path_buf()).remove(0);
    if check.present {
        Ok(())
    } else {
        Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("missing {} at {}", check.label, path.display()),
        ))
    }
}

fn install_clang(root: &Path) -> io::Result<()> {
    let source = root.join("source");
    let tag = format!("llvmorg-{CLANG_VERSION}");
    run_git(
        [
            "clone",
            "--quiet",
            "--depth",
            "1",
            "--filter=blob:none",
            "--sparse",
            "--branch",
            &tag,
            "https://github.com/llvm/llvm-project.git",
        ],
        &source,
    )?;
    let status = Command::new("git")
        .arg("-C")
        .arg(&source)
        .args([
            "sparse-checkout",
            "set",
            "--no-cone",
            "/clang/lib/Headers/",
            "/LICENSE.TXT",
            "/NOTICE.TXT",
        ])
        .status()?;
    if !status.success() {
        return Err(io::Error::other(format!(
            "git sparse-checkout failed with {status}"
        )));
    }
    copy_tree(&source.join("clang/lib/Headers"), &root.join("include"))?;
    fs::copy(source.join("LICENSE.TXT"), root.join("LICENSE.TXT"))?;
    if source.join("NOTICE.TXT").is_file() {
        fs::copy(source.join("NOTICE.TXT"), root.join("NOTICE.TXT"))?;
    }
    fs::remove_dir_all(source)?;
    fs::write(
        root.join("COMPILER-HEADERS-MANIFEST.txt"),
        format!(
            "Compiler: upstream Clang\nVersion: {CLANG_VERSION}\nSource repository: https://github.com/llvm/llvm-project.git\nSource tag: {tag}\nSource path: clang/lib/Headers\n"
        ),
    )?;
    Ok(())
}

fn run_git<const N: usize>(args: [&str; N], destination: &Path) -> io::Result<()> {
    let status = Command::new("git")
        .args(["-c", "advice.detachedHead=false"])
        .args(args)
        .arg(destination)
        .status()
        .map_err(|error| io::Error::new(error.kind(), format!("could not run git: {error}")))?;
    if status.success() {
        Ok(())
    } else {
        Err(io::Error::other(format!("git clone failed with {status}")))
    }
}

fn copy_tree(source: &Path, destination: &Path) -> io::Result<()> {
    fs::create_dir_all(destination)?;
    for entry in fs::read_dir(source)? {
        let entry = entry?;
        let target = destination.join(entry.file_name());
        let kind = entry.file_type()?;
        if kind.is_dir() {
            copy_tree(&entry.path(), &target)?;
        } else if kind.is_file() {
            fs::copy(entry.path(), target)?;
        } else if kind.is_symlink() {
            copy_link(&entry.path(), &target)?;
        } else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "unexpected Clang header entry",
            ));
        }
    }
    Ok(())
}

#[cfg(unix)]
fn copy_link(source: &Path, target: &Path) -> io::Result<()> {
    std::os::unix::fs::symlink(fs::read_link(source)?, target)
}

#[cfg(windows)]
fn copy_link(source: &Path, target: &Path) -> io::Result<()> {
    if source.is_dir() {
        copy_tree(source, target)
    } else {
        fs::copy(source, target).map(|_| ())
    }
}

const GCC_COMMON_HEADERS: &[(&str, &[&str])] = &[
    ("float.h", &["gcc/ginclude/float.h"]),
    ("iso646.h", &["gcc/ginclude/iso646.h"]),
    ("stdarg.h", &["gcc/ginclude/stdarg.h"]),
    ("stdbool.h", &["gcc/ginclude/stdbool.h"]),
    ("stddef.h", &["gcc/ginclude/stddef.h"]),
    ("varargs.h", &["gcc/ginclude/varargs.h"]),
    ("stdfix.h", &["gcc/ginclude/stdfix.h"]),
    ("stdnoreturn.h", &["gcc/ginclude/stdnoreturn.h"]),
    ("stdalign.h", &["gcc/ginclude/stdalign.h"]),
    ("stdatomic.h", &["gcc/ginclude/stdatomic.h"]),
    ("stdckdint.h", &["gcc/ginclude/stdckdint.h"]),
    ("stdcountof.h", &["gcc/ginclude/stdcountof.h"]),
    ("stdint-gcc.h", &["gcc/ginclude/stdint-gcc.h"]),
    ("stdint.h", &["gcc/ginclude/stdint-wrap.h"]),
    (
        "limits.h",
        &["gcc/limitx.h", "gcc/glimits.h", "gcc/limity.h"],
    ),
    ("syslimits.h", &["gcc/gsyslimits.h"]),
];

const GCC_LICENSES: &[&str] = &["COPYING3", "COPYING.RUNTIME"];

const GCC_X86_HEADERS: &[&str] = &[
    "cpuid.h",
    "mmintrin.h",
    "mm3dnow.h",
    "xmmintrin.h",
    "emmintrin.h",
    "pmmintrin.h",
    "tmmintrin.h",
    "ammintrin.h",
    "smmintrin.h",
    "nmmintrin.h",
    "bmmintrin.h",
    "fma4intrin.h",
    "wmmintrin.h",
    "immintrin.h",
    "x86intrin.h",
    "avxintrin.h",
    "xopintrin.h",
    "ia32intrin.h",
    "cross-stdarg.h",
    "lwpintrin.h",
    "popcntintrin.h",
    "lzcntintrin.h",
    "bmiintrin.h",
    "bmi2intrin.h",
    "tbmintrin.h",
    "avx2intrin.h",
    "avx512fintrin.h",
    "fmaintrin.h",
    "f16cintrin.h",
    "rtmintrin.h",
    "xtestintrin.h",
    "rdseedintrin.h",
    "prfchwintrin.h",
    "adxintrin.h",
    "fxsrintrin.h",
    "xsaveintrin.h",
    "xsaveoptintrin.h",
    "avx512cdintrin.h",
    "shaintrin.h",
    "clflushoptintrin.h",
    "xsavecintrin.h",
    "xsavesintrin.h",
    "avx512dqintrin.h",
    "avx512bwintrin.h",
    "avx512vlintrin.h",
    "avx512vlbwintrin.h",
    "avx512vldqintrin.h",
    "avx512ifmaintrin.h",
    "avx512ifmavlintrin.h",
    "avx512vbmiintrin.h",
    "avx512vbmivlintrin.h",
    "avx512vpopcntdqintrin.h",
    "clwbintrin.h",
    "mwaitxintrin.h",
    "clzerointrin.h",
    "pkuintrin.h",
    "sgxintrin.h",
    "cetintrin.h",
    "gfniintrin.h",
    "cet.h",
    "avx512vbmi2intrin.h",
    "avx512vbmi2vlintrin.h",
    "avx512vnniintrin.h",
    "avx512vnnivlintrin.h",
    "vaesintrin.h",
    "vpclmulqdqintrin.h",
    "avx512vpopcntdqvlintrin.h",
    "avx512bitalgintrin.h",
    "avx512bitalgvlintrin.h",
    "pconfigintrin.h",
    "wbnoinvdintrin.h",
    "movdirintrin.h",
    "waitpkgintrin.h",
    "cldemoteintrin.h",
    "avx512bf16vlintrin.h",
    "avx512bf16intrin.h",
    "enqcmdintrin.h",
    "serializeintrin.h",
    "avx512vp2intersectintrin.h",
    "avx512vp2intersectvlintrin.h",
    "tsxldtrkintrin.h",
    "amxtileintrin.h",
    "amxint8intrin.h",
    "amxbf16intrin.h",
    "x86gprintrin.h",
    "uintrintrin.h",
    "hresetintrin.h",
    "keylockerintrin.h",
    "avxvnniintrin.h",
    "mwaitintrin.h",
    "avx512fp16intrin.h",
    "avx512fp16vlintrin.h",
    "avxifmaintrin.h",
    "avxvnniint8intrin.h",
    "avxneconvertintrin.h",
    "cmpccxaddintrin.h",
    "amxfp16intrin.h",
    "prfchiintrin.h",
    "raointintrin.h",
    "amxcomplexintrin.h",
    "avxvnniint16intrin.h",
    "sm3intrin.h",
    "sha512intrin.h",
    "sm4intrin.h",
    "usermsrintrin.h",
    "avx10_2mediaintrin.h",
    "avx10_2convertintrin.h",
    "avx10_2bf16intrin.h",
    "avx10_2satcvtintrin.h",
    "avx10_2minmaxintrin.h",
    "avx10_2copyintrin.h",
    "amxavx512intrin.h",
    "amxtf32intrin.h",
    "amxfp8intrin.h",
    "movrsintrin.h",
    "amxmovrsintrin.h",
    "avx512bmmintrin.h",
    "avx512bmmvlintrin.h",
];

const GCC_AARCH64_HEADERS: &[&str] = &[
    "arm_fp16.h",
    "arm_neon.h",
    "arm_bf16.h",
    "arm_acle.h",
    "arm_sve.h",
    "arm_sme.h",
    "arm_neon_sve_bridge.h",
    "arm_private_fp8.h",
    "arm_private_neon_types.h",
];

const GCC_ARM_HEADERS: &[&str] = &[
    "mmintrin.h",
    "arm_neon.h",
    "arm_acle.h",
    "arm_fp16.h",
    "arm_cmse.h",
    "arm_bf16.h",
    "arm_mve_types.h",
    "arm_mve.h",
    "arm_cde.h",
];

fn install_gcc(cache: &Path, root: &Path) -> io::Result<()> {
    let filename = format!("gcc-{GCC_VERSION}.tar.xz");
    let url = format!("https://ftp.gnu.org/gnu/gcc/gcc-{GCC_VERSION}/{filename}");
    let archive =
        crate::download::fetch(&cache.join("compiler-headers"), &filename, &url, GCC_SHA256)?;
    let family_outputs: Vec<_> = GccFamily::ALL
        .into_iter()
        .map(|family| (family, family.outputs()))
        .collect();
    let wanted: HashSet<String> = family_outputs
        .iter()
        .flat_map(|(_, outputs)| {
            outputs
                .iter()
                .flat_map(|(_, sources)| sources.iter().cloned())
        })
        .chain(GCC_LICENSES.iter().map(|license| license.to_string()))
        .collect();
    let file = File::open(archive)?;
    let decoder = xz2::read::XzDecoder::new(file);
    let mut archive = tar::Archive::new(decoder);
    let prefix = format!("gcc-{GCC_VERSION}/");
    let mut sources: HashMap<String, Vec<u8>> = HashMap::new();
    for entry in archive.entries()? {
        let mut entry = entry?;
        if !entry.header().entry_type().is_file() {
            continue;
        }
        let path = entry.path()?;
        let Some(relative) = path
            .strip_prefix(&prefix)
            .ok()
            .and_then(|relative| relative.to_str())
            .filter(|relative| wanted.contains(*relative))
            .map(str::to_owned)
        else {
            continue;
        };
        let mut contents = Vec::new();
        entry.read_to_end(&mut contents)?;
        sources.insert(relative, contents);
    }
    let source = |name: &str| {
        sources.get(name).ok_or_else(|| {
            io::Error::new(
                io::ErrorKind::InvalidData,
                format!("GCC archive is missing {name}"),
            )
        })
    };
    for (family, outputs) in &family_outputs {
        let include = root.join(family.name()).join("include");
        fs::create_dir_all(&include)?;
        for (output, inputs) in outputs {
            let mut contents = Vec::new();
            for input in inputs {
                contents.extend_from_slice(source(input)?);
            }
            fs::write(include.join(output), contents)?;
        }
    }
    for license in GCC_LICENSES {
        fs::write(root.join(license), source(license)?)?;
    }
    fs::write(
        root.join("COMPILER-HEADERS-MANIFEST.txt"),
        format!(
            "Compiler: GCC\nVersion: {GCC_VERSION}\nSource URL: {url}\nAssembly: stmp-int-hdrs from gcc/Makefile.in for a use_gcc_stdint=wrap Linux target, once per header family; limits.h is limitx.h + glimits.h + limity.h, stdint.h is ginclude/stdint-wrap.h\nFamilies: x86 (i[34567]86 and x86_64 extra_headers from gcc/config/i386, mm_malloc.h from pmm_malloc.h, unwind.h from libgcc/unwind-generic.h); aarch64 (extra_headers from gcc/config/aarch64, unwind.h from libgcc/unwind-generic.h); arm (extra_headers from gcc/config/arm, unwind.h from libgcc/config/arm/unwind-arm.h, ginclude/unwind-arm-common.h)\n"
        ),
    )?;
    Ok(())
}
