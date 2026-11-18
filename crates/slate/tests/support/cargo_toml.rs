#![allow(
    dead_code,
    reason = "shared helpers are used by different integration test binaries"
)]

pub use slate_cargo_toml::*;

use serde::Serialize;
use std::collections::BTreeMap;
use std::path::Path;

pub fn read_manifest(path: &Path) -> Result<Manifest, String> {
    let text =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    Manifest::parse(&text).map_err(|e| format!("parse {}: {e}", path.display()))
}

pub fn render_manifest(manifest: &Manifest) -> Result<String, String> {
    manifest
        .to_toml()
        .map_err(|e| format!("serialize {}: {e}", manifest.package.name))
}

pub fn read_cargo_config(crate_dir: &Path) -> Result<CargoConfig, String> {
    let path = crate_dir.join(".cargo/config.toml");
    let text =
        std::fs::read_to_string(&path).map_err(|e| format!("read {}: {e}", path.display()))?;
    CargoConfig::parse(&text).map_err(|e| format!("parse {}: {e}", path.display()))
}

pub fn config_rustflags(config: &CargoConfig) -> &[String] {
    config
        .build
        .as_ref()
        .map_or(&[], |build| build.rustflags.as_slice())
}

pub fn render_package_profiles(manifest: &Manifest, alias: &str) -> Result<String, String> {
    #[derive(Serialize)]
    struct Profiles {
        profile: BTreeMap<String, Profile>,
    }
    let alias_packages = |profile: &Profile| {
        profile
            .package
            .values()
            .map(|package| (alias.to_string(), package.clone()))
            .collect()
    };
    manifest
        .profile
        .iter()
        .map(|(name, profile)| {
            let kept = match name.as_str() {
                "dev" | "release" => Profile {
                    package: alias_packages(profile),
                    ..Profile::default()
                },
                _ => Profile {
                    package: alias_packages(profile),
                    ..profile.clone()
                },
            };
            (name.clone(), kept)
        })
        .filter(|(_, profile)| *profile != Profile::default())
        .map(|(name, profile)| {
            toml::to_string(&Profiles {
                profile: BTreeMap::from([(name, profile)]),
            })
            .map_err(|e| format!("serialize profiles: {e}"))
        })
        .collect()
}
