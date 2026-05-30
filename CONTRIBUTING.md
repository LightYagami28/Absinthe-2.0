# Contributing to Absinthe

First off, thanks for taking the time to contribute! 🎉

## Code of Conduct

This project and everyone participating in it is governed by our [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the issue list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible:

- **Use a clear and descriptive title**
- **Describe the exact steps which reproduce the problem**
- **Provide specific examples to demonstrate the steps**
- **Describe the behavior you observed after following the steps**
- **Explain which behavior you expected to see instead and why**
- **Include screenshots and animated GIFs if possible**
- **Include your environment details** (OS, architecture, etc.)

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, please include:

- **Use a clear and descriptive title**
- **Provide a step-by-step description of the suggested enhancement**
- **Provide specific examples to demonstrate the steps**
- **Describe the current behavior** and **expected behavior**
- **Include screenshots and animated GIFs if possible**
- **Explain why this enhancement would be useful**

### Pull Requests

- Follow the C code style conventions used in the project
- Include appropriate test cases
- Update documentation as needed
- End all files with a newline
- Avoid platform-specific code when possible

## Development Setup

### Prerequisites

- GCC or Clang compiler
- Autotools (autoconf, automake)
- libxml2 development files
- OpenSSL development files
- libgtk+ 2.0 development files (for GUI)

### Building

```bash
./autogen.sh
./configure
make
```

### Testing

After making changes:

```bash
./autogen.sh
./configure
make clean
make
```

## Style Guide

- Follow the existing code style in the project
- Use meaningful variable and function names
- Comment complex logic, but not obvious code
- Keep functions focused and reasonably sized
- Use proper error handling

## Commit Messages

- Use the present tense ("Add feature" not "Added feature")
- Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit the first line to 72 characters or less
- Reference issues and pull requests liberally after the first line
- Consider starting the commit message with an applicable emoji:
  - 🎨 `:art:` when improving the format/structure of the code
  - 🐛 `:bug:` when fixing a bug
  - ✨ `:sparkles:` when introducing a new feature
  - 🚀 `:rocket:` when improving performance
  - 📝 `:memo:` when writing docs
  - 🔧 `:wrench:` when configuring tools or build system
  - ✅ `:white_check_mark:` when adding tests

## Recognition

Contributors will be recognized in:
- The [CONTRIBUTORS.md](CONTRIBUTORS.md) file
- Release notes for major contributions
- GitHub contributor statistics

## Questions?

Feel free to reach out on the IRC channel `#greenpois0n` on `irc.chronic-dev.org` or open an issue on GitHub.

---

Thank you for contributing to Absinthe! 🙏
