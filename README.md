<div align="center">

# 🔓 Absinthe 2.0

**The untethered jailbreak for iOS 5.0 – 5.1.1**

*Developed by the Chronic-Dev Team — @pod2g, @planetbeing, @pimskeks*

[![CI](https://github.com/LightYagami28/Absinthe-2.0/actions/workflows/ci.yml/badge.svg)](https://github.com/LightYagami28/Absinthe-2.0/actions/workflows/ci.yml)
[![CodeQL](https://github.com/LightYagami28/Absinthe-2.0/actions/workflows/codeql.yml/badge.svg)](https://github.com/LightYagami28/Absinthe-2.0/actions/workflows/codeql.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-2.0.4-green.svg)](CHANGELOG.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](#-system-requirements)
[![iOS](https://img.shields.io/badge/iOS-5.0%20–%205.1.1-orange.svg)](#-supported-devices)
[![Language](https://img.shields.io/badge/language-C-blue.svg)](src/)
[![Last Commit](https://img.shields.io/github/last-commit/LightYagami28/Absinthe-2.0)](https://github.com/LightYagami28/Absinthe-2.0/commits/master)

</div>

---

> [!WARNING]
> **This is a historical jailbreak tool from 2012 for iOS 5.x.**
> It does **not** work on modern devices or iOS versions.
> It is provided for **educational and historical purposes only**.
> Use at your own risk on supported legacy hardware.

---

## 📋 Table of Contents

- [✨ Features](#-features)
- [📱 Supported Devices](#-supported-devices)
- [💻 System Requirements](#-system-requirements)
- [🚀 Quick Start](#-quick-start)
- [🔨 Build from Source](#-build-from-source)
- [📖 How It Works](#-how-it-works)
- [🗂️ Project Structure](#️-project-structure)
- [🤝 Contributing](#-contributing)
- [👥 Credits](#-credits)
- [📄 License](#-license)

---

## ✨ Features

- **Untethered jailbreak** — the device stays jailbroken after every reboot, no computer needed
- **Multi-device support** — works on iPhone 3GS, 4, 4S; iPod Touch 3G, 4G; iPad 1, 2, 3
- **Cross-platform tool** — GUI application for Linux (GTK+), macOS (Cocoa), and Windows (Win32)
- **Automatic Cydia installation** — installs Cydia, the jailbreak app store, automatically
- **Fully automatic process** — plug in, click one button, done in ~5 minutes
- **Command-line interface** — headless mode for automation and scripting
- **Open source** — MIT-licensed, fully auditable C/C++ code

---

## 📱 Supported Devices

> [!NOTE]
> Only **iOS 5.1.1** (build 9B206 / 9B208) and **iOS 5.0.x** (builds 9A334, 9A405, 9A406) are supported.

| Device | Model Identifier | iOS 5.0 / 5.0.1 | iOS 5.1.1 |
|--------|-----------------|:---------------:|:---------:|
| iPhone 4S | iPhone4,1 | ✅ | ✅ |
| iPhone 4 (CDMA) | iPhone3,3 | ✅ | ✅ |
| iPhone 4 (GSM) | iPhone3,1 | ✅ | ✅ |
| iPhone 3GS | iPhone2,1 | ✅ | ✅ |
| iPad 3 (Wi-Fi) | iPad3,1 | ❌ | ✅ |
| iPad 3 (Wi-Fi + Cellular) | iPad3,2 / iPad3,3 | ❌ | ✅ |
| iPad 2 (Wi-Fi) | iPad2,1 | ✅ | ✅ |
| iPad 2 (GSM) | iPad2,2 | ✅ | ✅ |
| iPad 2 (CDMA) | iPad2,3 | ✅ | ✅ |
| iPad 2 (Wi-Fi) R2 | iPad2,4 | ❌ | ✅ |
| iPad 1 | iPad1,1 | ✅ | ✅ |
| iPod Touch 4G | iPod4,1 | ✅ | ✅ |
| iPod Touch 3G | iPod3,1 | ✅ | ✅ |

---

## 💻 System Requirements

| Platform | Version | Notes |
|----------|---------|-------|
| **Linux** | Any modern distro | x86 or x86_64, kernel ≥ 2.6.15, GTK+ 2.22+ |
| **macOS** | 10.5 (Leopard) or later | Intel only (x86/i386) |
| **Windows** | XP / Vista / 7 or later | 32-bit runtime required |

**Also required:**
- **libimobiledevice** ≥ 1.1.0 (bundled as static library)
- **iTunes** installed (Windows/macOS) — used for USB device communication
- USB cable and a supported iOS device

---

## 🚀 Quick Start

> **Prerequisites:** The pre-built binary is the easiest way to jailbreak. Build from source only if you need to modify the tool.

### Using the GUI

1. **Back up your device** in iTunes before proceeding
2. **Disable the passcode** (Settings → General → Passcode → Turn Passcode Off)
3. **Connect** your device via USB
4. **Run Absinthe** (download from [Releases](https://github.com/LightYagami28/Absinthe-2.0/releases))
5. Click **Jailbreak** and wait ~5 minutes
6. After reboot, open **Cydia** on your device — done! 🎉

> [!IMPORTANT]
> Do not touch your device during the jailbreak process. It will reboot automatically.

### Using the CLI

```sh
./absinthe --udid <YOUR_DEVICE_UDID>
```

Find your UDID with:
```sh
./absinthe --help
```

---

## 🔨 Build from Source

See **[BUILDING.md](BUILDING.md)** for full instructions including:
- Prerequisites for Linux, macOS, and Windows
- Step-by-step build commands
- Debug build instructions
- Troubleshooting common errors

**TL;DR (Linux):**
```sh
git clone https://github.com/LightYagami28/Absinthe-2.0.git
cd Absinthe-2.0
./autogen.sh
./configure
make
make check        # run the test suite
```

---

## 📖 How It Works

Absinthe exploits a chain of vulnerabilities to achieve an untethered jailbreak:

```
[Desktop Tool]
      │
      ├─ 1. AFC symlink traversal → escapes iOS sandbox filesystem
      │
      ├─ 2. MobileBackup2 injection → plants patched racoon + config via backup restore
      │
      ├─ 3. racoon VPN daemon exploit → format-string %n write to arbitrary memory
      │
      ├─ 4. ROP chain execution → Return-Oriented Programming via libcopyfile gadgets
      │
      ├─ 5. ptrace(PT_ATTACH) on notifyd → hijacks instruction pointer
      │
      └─ 6. launchctl load /corona/jb.plist → bootstraps jailbreak on every boot
```

**Key exploit components:**
- **corona** — iOS 5.0.x kernel exploit
- **rocky-racoon** — iOS 5.1.1 exploit via racoon VPN daemon
- **fsgen** — generates the ROP chain payload
- **Cydia.tar.lzma** — Cydia package extracted to device after exploitation

---

## 🗂️ Project Structure

```
Absinthe-2.0/
├── src/                    # Core jailbreak engine (C)
│   ├── jailbreak.c         # Main orchestrator
│   ├── jailbreak_51.c      # iOS 5.1.1 exploit path
│   ├── fsgen.c             # ROP chain generator
│   ├── rop.c               # ROP gadget helpers
│   ├── bpatch.c            # Binary patching (bsdiff)
│   ├── idevicebackup2.c    # iTunes backup protocol
│   ├── mb2.c               # MobileBackup2 client
│   ├── afc.c               # Apple File Conduit
│   ├── macho.c             # Mach-O binary parser
│   └── dyldcache.c         # dyld shared cache parser
├── gui/                    # Cross-platform GUI (C++)
│   ├── AbsintheMainWnd_*.cpp  # GTK+/Cocoa/Win32 implementations
│   └── AbsintheWorker.cpp  # Background jailbreak thread
├── tools/                  # Developer utilities
├── tests/                  # Test suite (make check)
├── data/                   # iOS device payloads by build number
│   ├── 9B206/ 9B208/       # iOS 5.1.1 payloads
│   ├── 9A405/ 9A406/       # iOS 5.0.1 payloads
│   └── common/             # Cydia.tar.lzma, lzma, tar binaries
├── staticlibs/             # Pre-built static libs per platform
├── include/                # Shared header files
├── iOS/                    # iOS-side helper binaries (ARM)
├── configure.ac            # Autoconf configuration
├── Makefile.am             # Automake rules
└── build.sh                # Cross-platform packaging script
```

---

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) first.

Key points:
- All contributors must follow our [Code of Conduct](CODE_OF_CONDUCT.md)
- Bug reports → use the [issue template](.github/ISSUE_TEMPLATE/bug_report.yml)
- Pull requests → fill in the [PR template](.github/PULL_REQUEST_TEMPLATE.md)
- Check that `make check` passes before submitting

---

## 👥 Credits

| Contributor | Role |
|-------------|------|
| [@pod2g](https://twitter.com/pod2g) | iOS 5.1.1 vulnerability research & exploitation |
| [@planetbeing](https://twitter.com/planetbeing) | Kernel exploitation & ROP chain |
| [@pimskeks](https://twitter.com/pimskeks) | Exploitation, GUI development |
| @iOPK | Artwork and visual design |
| Hanene Samara | GUI development and UX |
| [Chronic-Dev Team](https://twitter.com/chronicdevteam) | Infrastructure & project support |

**Acknowledgements:** The iOS security research community, previous untethered jailbreak authors, and all the testers who made this possible.

---

## 📄 License

This project is licensed under the **MIT License** — see [LICENSE](LICENSE) for details.

```
Copyright (c) 2012 Chronic-Dev Team
```

> [!NOTE]
> This tool exploits now-patched vulnerabilities in iOS 5.x. All exploit techniques are publicly documented. This repository is for educational and historical preservation purposes only.
