# 📘Organise User Guide

**organise** is a command‑line tool that automatically sorts and moves files into designated directories based on their extensions or regular expression patterns. It is written in C++ and designed to be fast, lightweight, and reliable.

---

# Table of Contents

- Installation
- Configuration
- Basic Usage
- Key Features
	- Dry Run (`-n/--dry-run`)
	- Recursive Scan (`-r/--recursive`)
	- Verbose Output (`-v/--verbose`)
	- Conflict Resolution
	- Watch Mode (`-w`)
	- Undo (`-u/--undo`)
- Activity Logging
- Examples
- Troubleshooting

---

# Installation

## From source

```Bash
git clone https://github.com/rafael-hacker/organise.git
cd organise
cmake -B build
cmake --build build
sudo cmake --install build   # installs to /usr/local/bin/org
```

## Arch Linux (AUR)
```Bash
# Stable release
paru -S organise

# Development version (latest git)
paru -S organise-git
```

---

# Configuration

The tool reads its configuration from `~/.config/organise/config.json`
If the file does not exist, it is created automatically with default rules:
```Bash
{
  ".pdf": "/home/your_username/Documents/PDFs",
  ".png": "/home/your_username/Pictures",
  ".jpg": "/home/your_username/Pictures",
  ".zip": "/home/your_username/Downloads/Archives"
}
```

# Rule Format
Each rule has a **pattern** (file extension or regular expression) to a **destination directory**.

- **Extension** (Exact match): `*.pdf` -> matches `file.pdf`
- **Regular Expression** `".*\\.(jpg|jpeg|png)$"` -> matches any file ending with those extensions

> **Note**:  Patterns are checked in the order they appear. The first match wins.

You can mix extensions and regexes. Directories are created automatically if they don't exist.

---

# Basic Usage

```Bash
org [OPTIONS] /path/to/target/directory
```
If you're already in the target directory, you can just run:
```Bash
org
```

---

# Key Features

## **Dry Run** (`-n/--dry-run`)
Simulate what would happen without actually moving any files.
```Bash
org -n ~/Downloads # You can use -n or --dry-run
```
Output shows which files would be moved and where, with `[DRY-RUN]` prefix.

## **Recursive Scan** (`-r/--recursive`)
Process all subdirectories recursively.
```Bash
org -r ~/Downloads
```
Without `-r/--recursive`, only files directly inside the target directory are processed.

## **Verbose Output** (`-v/--verbose`)
Display detailed information about the scanning process and rule matching.
```Bash
org -v ~/Downloads
```

## **Conflict Resolution**
When a file would be moved to a destination where a file with the same name already exists, `organise` can handle it in one of these ways:

- **Interactive (default)**: Prompt the user to choose between **rename**, **skip**, or **overwrite**.
- **Auto-rename (`-y/--auto-rename`): Automatically rename the new file by adding `(1)`, `(2)`, etc.
- **Skip** (`--conflit skip`): Leave the file in its original location.
- **Overwrite** (`--conflict overwrite`): Replace the existing file.

```Bash
org --conflict rename ~/Downloads   # always rename on conflict
org -y ~/Downloads                  # same as --conflict rename
org --conflict skip ~/Downloads
org --conflict overwrite ~/Downloads
```

## **Watch Mode (`-w/--watch`)
Monitor a directory **recursively** for new files and automatically organise them as they appear.
```Bash
org --watch ~/Downloads
```

- Watches all subdirectories, including newly created ones.
- Runs until you press `Ctrl+C`.
- All moves are logged to `~/.local/state/organise/activity.log`.
> **Tip**: Combine with `-v/--verbose` to see real-time activity.

## **Undo** (`-u/--undo`)
Revert the **most recent** batch of moves (the last invocation of `org` that actually moved files).
```Bash
org -u
```

- Reads the history from `~/.local/state/organise/history.json`.
- Moves files back to their original locations.
- After successfull undo, the history is cleared.
> **Caution**: Only the last operation can be undone. If you run `org` again after an undo, the undone moves are not logged again (they are considered a new operation).

---

# **Activity Logging**
All successfull moves and errors are recorded with timestamps in:
```Text
~/.local/state/organise/activity.log
```
Example:
```Text
[2026-08-18 14:23:10] Run started on /home/user/Downloads
[2026-08-18 14:23:11] Moved photo.jpg -> /home/user/Pictures/photo.jpg
[2026-08-18 14:23:12] Watcher started on /home/user/Downloads
```
This is especially useful when running in watch mode, as you can review what was moved while you were away.

## **Examples**

1. **Organise you Downloads folder (non-recursive)**
```Bash
org ~/Downloads
```
2. **See what would happen before actually doing it**
```Bash
org -n -r ~/Downloads
```
3. **Watch your Downloads folder and automatically sort new files**
```Bash
org --watch ~/Downloads
```
4. **Use a custom config file**
```Bash
org -c ~/my-rules.json ~/Documents
```
5. **Undo a mistaken move**
```Bash
org -u
```
6. **Combine flags for maximum information**
```Bash
org -v -r --conflict rename ~/Downloads
```

---

# **Troubleshooting**

### **"Error: Target directory is needed."**
* You forgot to pass a directory. Either provide it as an argument or use `-p` (deprecated).

### **Config file not found / parsing error
* `organise` automatically creates a default config if missing. If you see a parsing error, your *JSON* may be malformed. Use a *JSON* validator.

### **Undo fails with "Invalid cross-device link"
* This happens when the original move was across different file systems (e.g., from `/home` to `/mnt/usb`). The undo will attempt to copy the file back and delete the destination. If you still see issues, you can manually move the file back.

---

# **Contributing**
If you'd like to contribute, please read `CONTRIBUTING.md` in the repository.

---

# **License**
MIT License – see `LICENSE` file.

---

Happy organising! 🗂️
