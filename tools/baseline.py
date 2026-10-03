#!/usr/bin/env python3
"""Relink original objects and compare the entire ELF-derived DOL reference."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from audit import load_target
from formats import verify_load_image
from setup import ROOT, setup


def run(*args):
    subprocess.run([str(arg) for arg in args], cwd=ROOT, check=True)


def baseline():
    tools = setup()
    original, elf = load_target()
    config = json.loads((ROOT / "config/GR8E69/baseline.json").read_text())
    build = ROOT / "build/baseline"
    if build.exists():
        shutil.rmtree(build)
    build.mkdir(parents=True)
    reference = build / "reference.dol"
    run(tools / "dtk", "elf2dol", original, reference)
    expected = reference.read_bytes()
    if len(expected) != config["normalized_dol_size"] or hashlib.sha1(expected).hexdigest() != config["normalized_dol_sha1"]:
        raise ValueError("ELF conversion differs from the pinned reference")
    verify_load_image(elf, expected)
    (build / "symbols.txt").write_text("")
    (build / "splits.txt").write_text("")
    (build / "config.yml").write_text(
        "object: " + json.dumps(str(reference)) + "\n"
        "hash: " + config["normalized_dol_sha1"] + "\n"
        "symbols: " + json.dumps(str(build / "symbols.txt")) + "\n"
        "splits: " + json.dumps(str(build / "splits.txt")) + "\nquick_analysis: true\n")
    run(tools / "dtk", "dol", "split", build / "config.yml", build / "split")
    units = json.loads((build / "split/config.json").read_text())["units"]
    # The original ELF has an allocated zero-sized .sbss2 at 0x804535c0.
    # elf2dol includes its address in the BSS extent; preserve this metadata
    # with an empty input section, without adding or modifying instructions.
    empty = next(s for s in elf.sections if s["name"] == ".sbss2")
    if empty["type"] != 8 or empty["size"] or not empty["flags"] & 2:
        raise ValueError("Unexpected original empty .sbss2")
    (build / "empty-bss.s").write_text(
        '.section .bss.original_empty,"aw",@nobits\n'
        '.global __original_empty_sbss2\n__original_empty_sbss2:\n')
    run(tools / "powerpc-eabi-as", "-o", build / "empty-bss.o", build / "empty-bss.s")
    script = "ENTRY(__start)\nSECTIONS {\n"
    script += f"__start = {config['entry']};\n_SDA_BASE_ = {config['sda_base']};\n_SDA2_BASE_ = {config['sda2_base']};\n"
    script += "".join(f"{name} {address} : {{ *({name}) }}\n" for name, address in config["sections"].items())
    script += f".sbss2 {empty['address']:#x} (NOLOAD) : {{ KEEP(*(.bss.original_empty)) }}\n}}\n"
    (build / "link.ld").write_text(script)
    run(tools / "powerpc-eabi-ld", "-e", config["entry"], "-T", build / "link.ld",
        "-o", build / "relinked.elf", *[u["object"] for u in units], build / "empty-bss.o")
    run(tools / "dtk", "elf2dol", build / "relinked.elf", build / "relinked.dol")
    actual = (build / "relinked.dol").read_bytes()
    if actual != expected:
        raise ValueError("Complete reference comparison failed")
    verify_load_image(elf, actual)
    report = dict(status="identical", target=original.name,
                  original_elf_sha256=hashlib.sha256(elf.data).hexdigest(),
                  comparison="complete ELF-derived DOL; every allocated ELF byte also checked",
                  dol_sha1=hashlib.sha1(actual).hexdigest(), dol_size=len(actual),
                  reconstructed_source_bytes=0, original_object_count=len(units),
                  tool_lock_sha256=hashlib.sha256((ROOT / "tools/toolchain.json").read_bytes()).hexdigest())
    (build / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"Baseline verified: {len(actual)} bytes, SHA-1 {report['dol_sha1']}")
    print("Original objects only. No reconstructed-source or full-original-ELF match is claimed.")


if __name__ == "__main__":
    baseline()
