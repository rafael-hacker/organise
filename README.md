# organise 🗂️

**organise** is a C++ command-line tool designed to automatically sort and move files into designated directories based on their extensions.

---

## 📂 Project Structure

```text
organise/
├── CMakeLists.txt
├── include/
│   ├── config.hpp
│   └── organiser.hpp
├── src/
│   ├── config.cpp
│   ├── organiser.cpp
│   └── main.cpp
└── README.md
```

📦 Dependencies
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
org /path/to/target/folder
```
# Uninstalled Run
If you built the project without installing it globally, you can run the binary directly from the build folder:
```
./build/org /path/to/target/folder
```
