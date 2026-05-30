# Absinthe 2.x

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: iOS](https://img.shields.io/badge/Platform-iOS%205.1.1-blue.svg)](#supported-devices--firmware)
[![Language: C](https://img.shields.io/badge/Language-C-brightgreen.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Build Status](https://img.shields.io/badge/Build-Autotools-success.svg)](#build-instructions)

An untethered jailbreak for iOS 5.1.1 devices. Absinthe provides permanent root access to jailbroken devices, allowing users to modify system files, install unauthorized applications, and customize their iOS experience.

**Website:** [greenpois0n.com](http://greenpois0n.com/)

## Features

- **Untethered jailbreak** — Devices remain jailbroken after reboot
- **Multi-device support** — Works with iPhone, iPod touch, iPad, and iPad mini
- **Cross-platform tools** — Available for macOS, Windows, and Linux
- **User-friendly GUI** — Simple, guided jailbreak process

## Supported Devices & Firmware

### Supported Devices (iOS 5.1.1 only)


- iPad 3 (WiFi, CDMA, Global)
- iPad 2 (WiFi, CDMA, GSM)
- iPad 1
- iPhone 4S
- iPhone 4 (GSM/CDMA)
- iPhone 3GS
- iPod Touch 4G
- iPod Touch 3G

### Not Supported


- iPad 2 WiFi R2 (new model)
- Apple TV 2 & 3

## System Requirements

| Platform    | Requirements                                    |
| ----------- | ----------------------------------------------- |
| **macOS**   | 10.5 or later (Intel)                          |
| **Windows** | XP, Vista, 7 or later                          |
| **Linux**   | x86/x86_64, Kernel ≥ 2.6.15, libgtk+-2.0 ≥ 2.22.1 |

## Usage

### Pre-Jailbreak Preparation

1. **Backup your device** using iTunes (or iCloud)
   - This is critical—if something goes wrong, you'll need this backup
2. **Disable iTunes backup password** (if enabled)
   - Create a fresh backup after disabling the password
3. **Disable the lock passcode** on your iOS device
   - This prevents issues during the jailbreak process

### Jailbreak Process

1. Open Absinthe and connect your device via USB
2. Ensure your device is recognized by Absinthe
3. Click the **Jailbreak** button and wait for the process to complete
   - **Do not disconnect** your device during this time
   - If the process hangs, it's safe to restart Absinthe or reboot the device
4. Once jailbreaking completes, restore your iTunes backup
   - Right-click your device in iTunes → "Restore From Backup"
   - Select the backup you created before jailbreaking

### Post-Jailbreak

- Your device will reboot and remain jailbroken
- All your apps, photos, and data will be restored from the backup
- Install Cydia or other package managers to customize your device

## Build Instructions

### Building on macOS

```bash
./autogen.sh
./configure
make
make install
```

### Building on Linux

```bash
./autogen.sh
./configure --with-libxml2=/usr --with-openssl=/usr
make
make install
```

### Building on Windows

Use the provided Visual Studio project or configure with MinGW:

```bash
./autogen.sh
./configure
make
```

## Credits & Acknowledgments

- **Exploitation by:** @pod2g, @planetbeing, @pimskeks
- **Artwork by:** @iOPK
- **GUI Development:** Hanene Samara, @pimskeks
- **Infrastructure:** Chronic-Dev, LLC (@chronicdevteam)

## Support & Contact

- **IRC:** Join `#greenpois0n` on `irc.chronic-dev.org`
- **Website:** [greenpois0n.com](http://greenpois0n.com/)
- **Discussion:** See the Chronic-Dev forums and wiki for detailed guides

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and improvements.

## Contributors

This project was developed by the Chronic-Dev Team:

- **@pod2g** — iOS 5.1.1 exploitation research
- **@planetbeing** — Kernel exploitation and vulnerability research  
- **@pimskeks** — Exploitation and GUI development
- **@iOPK** — Artwork and visual design
- **Hanene Samara** — GUI development and UX

Special thanks to the entire Chronic-Dev community for testing, feedback, and support.

For detailed contributor information, see [CONTRIBUTORS.md](CONTRIBUTORS.md).

## License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) file for details.

---

**⚠️ Note:** This is a legacy jailbreak for iOS 5.1.1. It is provided for educational and historical purposes only. Modern security practices and iOS versions have evolved significantly since this tool's release. Use at your own risk.
