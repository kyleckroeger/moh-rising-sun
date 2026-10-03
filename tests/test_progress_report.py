"""Public reporting safeguards run without the original game or compilers."""
import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from progress_report import export, fingerprints


class PublicProgress(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.config = self.root / "config/GR8E69"
        self.config.mkdir(parents=True)
        (self.root / "src").mkdir()
        (self.root / "src/example.c").write_text("int example(void) { return 1; }\n")
        self.manifest = {"id": "example", "source": "src/example.c",
                         "category": "reconstructed_game",
                         "functions": [{"name": "example", "address": "0x80001000", "size": 8}]}
        self.write("example.json", self.manifest)
        self.write("project.json", {"units": ["example.json"],
                                   "progress": {"code_bytes": 100, "executable_sections": {".text": 100}}})
        self.snapshot = {"schema_version": 1, "verification": "local-complete-image-match",
                         "input_sha256": fingerprints(self.root),
                         "units": [{"id": "example", "category": "reconstructed_game",
                                    "functions": 1, "code_bytes": 8}],
                         "progress": {"matching_code_bytes": 8, "total_executable_code_bytes": 100,
                                      "matching_functions": 1,
                                      "categories": {"reconstructed_game": 8, "restored_library": 0}}}

    def write(self, name, value):
        (self.config / name).write_text(json.dumps(value))

    def test_context_preserves_whole_executable_denominator(self):
        report = export(self.snapshot, self.root)
        self.assertEqual(report["version"], 2)
        self.assertEqual(report["measures"]["matched_code_percent"], 8)
        self.assertEqual(sum(int(u["measures"]["total_code"]) for u in report["units"]), 100)
        self.assertEqual(report["units"][-1]["measures"]["matched_code"], "0")
        self.assertNotIn("total_functions", report["measures"])
        self.assertNotIn(str(self.root), json.dumps(report))

    def test_changed_source_is_stale(self):
        (self.root / "src/example.c").write_text("changed\n")
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_added_source_is_stale(self):
        (self.root / "src/extra.c").write_text("new\n")
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_removed_source_is_stale(self):
        (self.root / "src/example.c").unlink()
        with self.assertRaisesRegex(ValueError, "Stale"):
            export(self.snapshot, self.root)

    def test_inflated_snapshot_total_is_rejected(self):
        self.snapshot["progress"]["matching_code_bytes"] += 1
        with self.assertRaisesRegex(ValueError, "totals"):
            export(self.snapshot, self.root)

    def test_overlapping_functions_are_rejected(self):
        self.manifest["functions"].append(copy.deepcopy(self.manifest["functions"][0]))
        self.write("example.json", self.manifest)
        self.snapshot["input_sha256"] = fingerprints(self.root)
        self.snapshot["units"][0].update(functions=2, code_bytes=16)
        with self.assertRaisesRegex(ValueError, "Overlapping"):
            export(self.snapshot, self.root)

    def test_duplicate_units_are_rejected(self):
        self.snapshot["units"].append(copy.deepcopy(self.snapshot["units"][0]))
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            export(self.snapshot, self.root)


if __name__ == "__main__":
    unittest.main()
