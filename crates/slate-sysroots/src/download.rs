use sha2::{Digest, Sha256};
use std::fs::{self, File};
use std::io::{self, Read, Write};
use std::path::{Path, PathBuf};

pub(crate) fn fetch(cache: &Path, filename: &str, url: &str, sha256: &str) -> io::Result<PathBuf> {
    fs::create_dir_all(cache)?;
    let path = cache.join(filename);
    if path.is_file() && verify_sha256(&path, sha256)? {
        return Ok(path);
    }

    let partial = cache.join(format!(".{filename}-{}.partial", std::process::id()));
    let result = (|| {
        let response = ureq::get(url)
            .call()
            .map_err(|error| io::Error::other(format!("download failed for {url}: {error}")))?;
        let mut reader = response.into_body().into_reader();
        let mut file = File::create(&partial)?;
        io::copy(&mut reader, &mut file)?;
        file.flush()?;
        if !verify_sha256(&partial, sha256)? {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                format!("SHA-256 mismatch for {url}"),
            ));
        }
        if path.exists() {
            fs::remove_file(&path)?;
        }
        fs::rename(&partial, &path)?;
        Ok(path.clone())
    })();
    if result.is_err() {
        let _ = fs::remove_file(&partial);
    }
    result
}

fn verify_sha256(path: &Path, expected: &str) -> io::Result<bool> {
    let mut file = File::open(path)?;
    let mut hasher = Sha256::new();
    let mut buffer = [0u8; 64 * 1024];
    loop {
        let count = file.read(&mut buffer)?;
        if count == 0 {
            break;
        }
        hasher.update(&buffer[..count]);
    }
    let actual: String = hasher
        .finalize()
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect();
    Ok(actual == expected)
}
