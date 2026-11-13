# Mac M1 Handoff

This English document no longer mixes Hungarian passages into its text. The original Hungarian-language passages are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

This guide prepares a local, read-only Mac M1 firmware/SCI/MIDI experiment. First verify the toolchain, then build the ROM-free project and the pinned MAME research target, and finally run the paired experiment with a local firmware path and a new output directory. The experiment has not been run merely because these commands are available; it does not establish a complete H8 port, AN1x boot or sound. The original Hungarian handoff and its assumptions are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

```sh
git status --short
uname -m
export DEVELOPER_DIR=/Applications/Xcode_16.2.app/Contents/Developer
xcodebuild -version
clang++ --version
python3 --version
cmake --version
make --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

```sh
git clone --config core.autocrlf=false https://github.com/mamedev/mame.git work/mame
git -C work/mame checkout 706dbb73281320c54d7c2f16262c1c3786f2be01
python3 tools/prepare_mame_research.py --source work/mame
make -C work/mame SUBTARGET=yamanix SOURCES=src/mame/yamaha/yman1x.cpp OSD=mac NO_USE_MIDI=1 NO_USE_PORTAUDIO=1 USE_QTDEBUG=0 USE_TAPTUN=0 USE_PCAP=0 REGENIE=1 OPTIMIZE=0 SYMBOLS=0 -j2
lipo -archs work/mame/yamanix
work/mame/yamanix -listfull an1x
```

```sh
python3 tools/run_m1_serial_experiment.py \
  --mame work/mame/yamanix \
  --rom '/absolute/local/path/to/YAMAHA AN1x 1.04.BIN' \
  --work private_runs/m1-mac-first
```
