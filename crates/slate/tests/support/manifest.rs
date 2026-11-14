use serde_json::{Value, json};
use std::path::{Path, PathBuf};

pub fn write_manifest(spec: &Value, sources: &[String], project: &Path, path: &Path) -> PathBuf {
    let stem = path
        .file_stem()
        .expect("manifest file name")
        .to_string_lossy()
        .into_owned();
    let dir = path.parent().expect("manifest directory");
    std::fs::create_dir_all(dir).expect("create manifest directory");
    let mut targets = serde_json::Map::new();
    for (name, target) in spec["targets"].as_object().expect("spec targets") {
        let flavor = target["flavor"].as_str().expect("target flavor");
        let compiler = match flavor {
            "msvc" => "cl",
            "gcc" => "gcc",
            _ => "clang",
        };
        let args: Vec<Value> = target
            .get("args")
            .and_then(Value::as_array)
            .cloned()
            .unwrap_or_default();
        let entries: Vec<Value> = sources
            .iter()
            .map(|source| {
                let mut arguments = vec![json!(compiler), json!("-c"), json!(source)];
                arguments.extend(args.iter().cloned());
                json!({
                    "directory": project.display().to_string(),
                    "file": source,
                    "arguments": arguments,
                })
            })
            .collect();
        let database = dir.join(format!("{stem}.{name}.compile_commands.json"));
        std::fs::write(&database, Value::Array(entries).to_string())
            .expect("write compile commands");
        let mut written = json!({
            "triple": target["triple"],
            "flavor": flavor,
            "compile_commands": database.display().to_string(),
        });
        if let Some(sysroot) = target.get("sysroot") {
            written["sysroot"] = sysroot.clone();
        }
        targets.insert(name.clone(), written);
    }
    let manifest = json!({
        "version": 1,
        "targets": targets,
        "features": spec.get("features").cloned().unwrap_or_else(|| json!({})),
        "invalid_features": spec.get("invalid_features").cloned().unwrap_or_else(|| json!([])),
    });
    std::fs::write(path, manifest.to_string()).expect("write manifest");
    path.to_path_buf()
}
