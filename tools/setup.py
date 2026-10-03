#!/usr/bin/env python3
"""Install checksum-pinned analysis tools locally; no system changes."""
import hashlib
import json
import platform
import shutil
import urllib.request
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / "build/tools"


def fetch(spec, output):
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists() and hashlib.sha256(output.read_bytes()).hexdigest() == spec["sha256"]:
        return
    temporary = output.with_suffix(".download")
    with urllib.request.urlopen(spec["url"], timeout=120) as response, temporary.open("wb") as stream:
        shutil.copyfileobj(response, stream)
    if hashlib.sha256(temporary.read_bytes()).hexdigest() != spec["sha256"]:
        temporary.unlink()
        raise RuntimeError(f"Checksum mismatch: {output.name}")
    temporary.replace(output)


def setup():
    if (platform.system(), platform.machine()) != ("Darwin", "arm64"):
        raise RuntimeError("Initial tool lock supports macOS ARM64 only")
    lock = json.loads((ROOT / "tools/toolchain.json").read_text())
    fetch(lock["dtk"], DEST / "dtk")
    (DEST / "dtk").chmod(0o755)
    archive = DEST / "binutils.zip"
    fetch(lock["binutils"], archive)
    with zipfile.ZipFile(archive) as package:
        for name in ("powerpc-eabi-as", "powerpc-eabi-ld", "powerpc-eabi-objdump", "powerpc-eabi-readelf", "powerpc-eabi-objcopy", "powerpc-eabi-nm"):
            path = DEST / name
            path.write_bytes(package.read(name))
            path.chmod(0o755)
    return DEST


if __name__ == "__main__":
    print(setup())
