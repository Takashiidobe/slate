use std::path::Path;

pub fn write_if_changed(path: &Path, contents: &str) -> Result<(), String> {
    if std::fs::read(path).is_ok_and(|current| current == contents.as_bytes()) {
        return Ok(());
    }
    std::fs::write(path, contents).map_err(|error| format!("write {}: {error}", path.display()))
}
