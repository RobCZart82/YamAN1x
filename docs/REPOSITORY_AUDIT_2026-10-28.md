# Repository Audit 2026 10 28

This English document no longer mixes Hungarian passages into its text. The original Hungarian-language passages are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

# YamAN1x repository-audit — 2026-10-28

```sh
cmake -S . -B build-audit -DCMAKE_BUILD_TYPE=Release
cmake --build build-audit --parallel 4
ctest --test-dir build-audit --output-on-failure
python3 -m unittest discover -s tests -p 'test_*.py'
python3 tools/repository_audit.py
python3 tools/license_coverage.py
python3 tools/check_h8_first_instructions.py --archive /absolute/path/to/mame-master.zip --work work/audit-h8-reference
```
