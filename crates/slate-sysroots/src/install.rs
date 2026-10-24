use std::fs::{self, File, OpenOptions};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};

use crate::{Paths, Target};

pub(crate) const MANIFEST_FILE: &str = "SYSROOT-MANIFEST.txt";

pub(crate) fn manifest_matches(root: &Path, expected: &str) -> bool {
    fs::read_to_string(root.join(MANIFEST_FILE)).is_ok_and(|installed| installed == expected)
}

pub(crate) fn install_staged(
    paths: &Paths,
    target: Target,
    manifest: Option<&str>,
    validate: impl Fn(&Path) -> io::Result<()>,
    build: impl FnOnce(&Path) -> io::Result<()>,
) -> io::Result<PathBuf> {
    let output = paths.sysroot_path(target);
    install_pinned_at(&output, manifest, validate, build)
}

pub(crate) fn install_staged_at(
    output: &Path,
    validate: impl Fn(&Path) -> io::Result<()>,
    build: impl FnOnce(&Path) -> io::Result<()>,
) -> io::Result<PathBuf> {
    install_pinned_at(output, None, validate, build)
}

pub(crate) fn install_pinned_at(
    output: &Path,
    manifest: Option<&str>,
    validate: impl Fn(&Path) -> io::Result<()>,
    build: impl FnOnce(&Path) -> io::Result<()>,
) -> io::Result<PathBuf> {
    let current =
        |output: &Path| manifest.is_none_or(|expected| manifest_matches(output, expected));
    if output.exists() && current(output) {
        validate(output)?;
        return Ok(output.to_path_buf());
    }

    let parent = output.parent().expect("installation path has a parent");
    fs::create_dir_all(parent)?;
    let name = output.file_name().expect("installation path has a name");
    let name = name.to_string_lossy();
    let _lock = InstallLock::acquire(&parent.join(format!("{name}.lock")))?;
    if output.exists() && current(output) {
        validate(output)?;
        return Ok(output.to_path_buf());
    }

    let staging = staging_path(parent, &name)?;
    fs::create_dir(&staging)?;
    let root = staging.join("root");
    let result = (|| {
        build(&root)?;
        validate(&root)?;
        if output.exists() {
            let previous = staging.join("previous");
            fs::rename(output, &previous)?;
            if let Err(error) = fs::rename(&root, output) {
                let _ = fs::rename(&previous, output);
                return Err(error);
            }
        } else {
            fs::rename(&root, output)?;
        }
        Ok(output.to_path_buf())
    })();
    let _ = fs::remove_dir_all(&staging);
    result
}

pub(crate) fn remove(paths: &Paths, target: Target) -> io::Result<()> {
    let output = paths.sysroot_path(target);
    if !output.exists() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("{} is not installed", target.triple()),
        ));
    }
    let parent = output.parent().expect("installation path has a parent");
    let name = output.file_name().expect("installation path has a name");
    let _lock = InstallLock::acquire(&parent.join(format!("{}.lock", name.to_string_lossy())))?;
    fs::remove_dir_all(output)
}

pub(crate) fn staging_path(parent: &Path, name: &str) -> io::Result<PathBuf> {
    let nonce = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_err(io::Error::other)?
        .as_nanos();
    Ok(parent.join(format!(".{name}-{}-{nonce}.tmp", std::process::id())))
}

struct InstallLock {
    path: PathBuf,
    file: Option<File>,
}

impl InstallLock {
    fn acquire(path: &Path) -> io::Result<Self> {
        let mut file = OpenOptions::new()
            .write(true)
            .create_new(true)
            .open(path)
            .map_err(|error| {
                if error.kind() == io::ErrorKind::AlreadyExists {
                    io::Error::new(
                        error.kind(),
                        format!("installation already in progress: {}", path.display()),
                    )
                } else {
                    error
                }
            })?;
        writeln!(file, "{}", std::process::id())?;
        Ok(Self {
            path: path.to_path_buf(),
            file: Some(file),
        })
    }
}

impl Drop for InstallLock {
    fn drop(&mut self) {
        drop(self.file.take());
        let _ = fs::remove_file(&self.path);
    }
}
