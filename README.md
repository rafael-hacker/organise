# organise 🗂️

**organise** is a C++ command-line tool designed to automatically sort and move files into designated directories based on their extensions.

---

## 📂 Project Structure

```text
organise/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── completions
│   ├── org.bash
│   ├── org.fish
│   └── org.zsh
├── docs
├── include
│   └── organise
│       ├── colors.hpp
│       ├── config.hpp
│       ├── history.hpp
│       ├── options.hpp
│       ├── organiser.hpp
│       └── version.hpp.in
├── man
│   └── org.1
├── packaging
│   └── PKGBUILD
├── src
│   ├── config.cpp
│   ├── history.cpp
│   ├── main.cpp
│   ├── options.cpp
│   ├── organiser.cpp
│   └── watcher.cpp
└── tests
```

---

## 📦 Dependencies

The project uses **CMake** (v3.14+) and automatically fetches the nlohmann/json library during the build step.

**Requirements**
- A C++17 compliant compile (`g++` or `clang++`)
- `cmake` (version 3.14 or higher)
- `git` (to fetch dependencies)

### Build instructions

``` Bash
cmake -B build
cmake --build build
sudo cmake --install build
```

### 🐧 Arch Linux (AUR)
You can install `organise` from the Arch User Repository using your favorite AUR helper:

```bash
# Stable version
paru/yay -S organise

# Latest development version (Git)
paru/yay -S organise-git
```

## ⚙️ How to Configure
The tool reads its configuration from the user's home directory at  `~/.config/organise/config.json`.

1. Create the configuration directory:
```
mkdir -p $HOME/.config/organise
```
2. Create and edit the `config.json` file:
``` JSON
{
  ".pdf": "/home/your_username/Documents/PDFs",
  ".jpg": "/home/your_username/Pictures",
  ".png": "/home/your_username/Pictures"
}
```
Note: The target directory will be created automatically if it does not already exist.

# 🚀 Usage
Once installed, you can run `org` from any directory in your terminal:
``` Bash
org [FLAGS] /path/to/target/folder
```
# ✨ Key Features

| Command | Description |
| :--- | :--- |
| **`org -n/--dry-run`** | Simulate actions without actually doing them |
| **`org -r/--recursive`** | Scan directories recursively |
| **`org -v/--verbose`** | Display detailed execution output |
| **`org -y/--auto-rename`** | Automatically  rename conflicting files (no prompt) |
| **`org --conflict <mode>`** | Set conflict strategy: rename, skip or overwrite |
| **`org -c/--config <path>`** | Use a custom config file |
| **`org --watch`** | Watch the directory continuously and organise new files as they're created or moved in |
| **`org -u/--undo`** | Undo the last batch of moves, using the history log |
| **`org --version`** | Display the project's version |
| **`org -h/--help`** | Display a help message |

# Uninstalled Run
If you built the project without installing it globally, you can run the binary directly from the build folder:
```
./build/org /path/to/target/folder
```
