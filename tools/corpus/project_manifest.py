import json
import shlex
from pathlib import Path

FLAG_PREFIXES = {"-D": "-D", "/D": "-D", "-I": "-I", "/I": "-I", "-U": "-U", "/U": "-U"}


def command_words(entry):
    return entry["arguments"] if "arguments" in entry else shlex.split(entry["command"])


def preprocessor_flags(words):
    flags = []
    index = 1
    while index < len(words):
        word = words[index]
        prefix = next((prefix for prefix in FLAG_PREFIXES if word.startswith(prefix)), None)
        if prefix:
            value = word[len(prefix):] or (words[index + 1] if index + 1 < len(words) else "")
            index += 0 if word[len(prefix):] else 1
            flags.append(FLAG_PREFIXES[prefix] + value)
        index += 1
    return flags


def target_units(project, target):
    entries = json.loads((project / target["compile_commands"]).read_text())
    build_target = target.get("build_target")
    units = {}
    for entry in entries:
        directory = Path(entry["directory"])
        file = (directory / entry["file"]).resolve()
        if file.suffix != ".c":
            continue
        words = command_words(entry)
        if build_target and f"{build_target}.dir/" not in entry.get("output", " ".join(words)):
            continue
        source = str(file.relative_to(project)) if file.is_relative_to(project) else str(file)
        units.setdefault(source, preprocessor_flags(words))
    return units


def load(path, project):
    project = Path(project).resolve()
    manifest = json.loads(Path(path).read_text())
    targets = []
    sources = set()
    for name, target in manifest["targets"].items():
        units = target_units(project, target)
        sources.update(units)
        first = next(iter(units.values()), [])
        targets.append({"name": name, "triple": target["triple"], "flavor": target["flavor"], "args": first,
                        "units": units})
    features = [{"name": name, "defines": defines} for name, defines in manifest.get("features", {}).items()]
    return {"sources": sorted(sources), "args": [], "targets": targets, "features": features,
            "invalid_features": manifest.get("invalid_features", [])}
