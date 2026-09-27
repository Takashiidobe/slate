use crate::compiler_args::CompilerFlavor;
use crate::target_info::TargetInfo;
use crate::target_registry::ClangHeaders;
use directories::ProjectDirs;
use std::cmp::Ordering;
use std::fs;
use std::path::{Path, PathBuf};

pub fn include_paths(target: &TargetInfo, flavor: CompilerFlavor) -> Vec<PathBuf> {
    let root = std::env::var_os("SLATE_COMPILER_HEADERS")
        .map(PathBuf::from)
        .or_else(|| {
            ProjectDirs::from("", "", "Slate")
                .map(|dirs| dirs.data_local_dir().join("compiler-headers"))
        });
    let Some(root) = root else {
        return Vec::new();
    };

    let profiles: Vec<(String, String)> = match flavor {
        CompilerFlavor::Clang if target.profile.clang_headers == ClangHeaders::AppleFirst => {
            vec![
                ("apple-clang-".into(), "include".into()),
                ("clang-".into(), "include".into()),
            ]
        }
        CompilerFlavor::Clang => vec![("clang-".into(), "include".into())],
        CompilerFlavor::Gcc => vec![("gcc-".into(), "include".into())],
        CompilerFlavor::Msvc => vec![("msvc-".into(), format!("{}/include", target.triple))],
    };

    profiles
        .into_iter()
        .find_map(|(prefix, suffix)| latest_profile(&root, &prefix, &suffix))
        .into_iter()
        .collect()
}

fn latest_profile(root: &Path, prefix: &str, suffix: &str) -> Option<PathBuf> {
    let mut profiles: Vec<(Vec<u64>, PathBuf)> = fs::read_dir(root)
        .ok()?
        .filter_map(Result::ok)
        .filter_map(|entry| {
            let name = entry.file_name();
            let name = name.to_str()?;
            let version = name.strip_prefix(prefix)?;
            let include = entry.path().join(suffix);
            include.is_dir().then(|| (version_key(version), include))
        })
        .collect();
    profiles.sort_by(|left, right| compare_versions(&left.0, &right.0));
    profiles.pop().map(|(_, path)| path)
}

fn version_key(version: &str) -> Vec<u64> {
    version
        .split(|character: char| !character.is_ascii_digit())
        .filter(|part| !part.is_empty())
        .filter_map(|part| part.parse().ok())
        .collect()
}

fn compare_versions(left: &[u64], right: &[u64]) -> Ordering {
    left.cmp(right)
}
