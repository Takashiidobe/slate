use std::fs::{self, File, OpenOptions};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};

use crate::{Paths, Target};

pub(crate) fn install_staged(
    paths: &Paths,
    target: Target,
    validate: impl Fn(&Path) -> io::Result<()>,
    build: impl FnOnce(&Path) -> io::Result<()>,
) -> io::Result<PathBuf> {
    let output = paths.sysroot_path(target);
    install_staged_at(&output, validate, build)
}

pub(crate) fn install_staged_at(
    output: &Path,
    validate: impl Fn(&Path) -> io::Result<()>,
    build: impl FnOnce(&Path) -> io::Result<()>,
) -> io::Result<PathBuf> {
    if output.exists() {
        validate(output)?;
        return Ok(output.to_path_buf());
    }

    let parent = output.parent().expect("installation path has a parent");
    fs::create_dir_all(parent)?;
    let name = output.file_name().expect("installation path has a name");
    let name = name.to_string_lossy();
    let _lock = InstallLock::acquire(&parent.join(format!("{name}.lock")))?;
    if output.exists() {
        validate(output)?;
        return Ok(output.to_path_buf());
    }

    let staging = staging_path(parent, &name)?;
    fs::create_dir(&staging)?;
    let root = staging.join("root");
    let result = (|| {
        build(&root)?;
        validate(&root)?;
        fs::rename(&root, output)?;
        Ok(output.to_path_buf())
    })();
    let _ = fs::remove_dir_all(&staging);
    result
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
