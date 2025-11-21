# File Organizer

A simple command-line tool to automatically organize files by their extensions.

Inspired by: [this tweet](https://x.com/_trish_xD/status/1989764993274057071)

## Features

- Automatically categorizes files based on file extensions
- Case-insensitive extension matching
- Creates organized folders: Documents, Images, Videos, Music, Archives, Books, Others

## Supported File Types

- **Documents**: .txt, .pdf, .doc, .docx, .yml, .md
- **Images**: .jpg, .jpeg, .png, .gif
- **Videos**: .mkv, .mp4, .m4a, .wav
- **Music**: .mp3
- **Archives**: .zip, .tar, .gz
- **Books**: .epub
- **Others**: All other file types

## Building

```bash
make
```

## Usage

Organize files in the current directory:
```bash
./build/organizer
```

Organize files in a specific directory:
```bash
./build/organizer /path/to/directory
```

Example:
```bash
./build/organizer ~/Downloads
```

## Installation

Install to system (requires sudo):
```bash
sudo make install
```

After installation, you can run from anywhere:
```bash
organizer ~/Downloads
```

Uninstall:
```bash
sudo make uninstall
```

## Cleaning

Remove build artifacts:
```bash
make clean
```
