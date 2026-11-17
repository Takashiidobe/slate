use std::path::{Path, PathBuf};

pub fn set_aside(dir: &Path) -> PathBuf {
    let mut previous = dir.as_os_str().to_owned();
    previous.push(".previous");
    let previous = PathBuf::from(previous);
    let _ = std::fs::remove_dir_all(&previous);
    let _ = std::fs::rename(dir, &previous);
    previous
}

pub fn keep_unchanged_mtimes(previous: &Path, dir: &Path) {
    restore(previous, dir);
    let _ = std::fs::remove_dir_all(previous);
}

fn restore(previous: &Path, dir: &Path) {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        let old = previous.join(entry.file_name());
        let Ok(kind) = entry.file_type() else {
            continue;
        };
        if kind.is_dir() {
            restore(&old, &path);
            continue;
        }
        let Ok(modified) = std::fs::metadata(&old).and_then(|meta| meta.modified()) else {
            continue;
        };
        let same = match (std::fs::read(&old), std::fs::read(&path)) {
            (Ok(before), Ok(after)) => before == after,
            _ => false,
        };
        if same && let Ok(file) = std::fs::File::options().write(true).open(&path) {
            let _ = file.set_modified(modified);
        }
    }
}
