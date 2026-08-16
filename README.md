# organise 🗂️

**organise** is a C++ tool designed to help you organize your file paths efficiently. The project is currently under active development and will be improved in the future!

---

## 📂 Project Structure

The repository directory tree is organized as follows:
```text
.
├── Makefile
├── README.md
├── bin
│   ├── config
│   │   └── config.json
│   └── org
└── src
    └── main.cpp
```

📦 Dependencies
To compile and run this project, you need the nlohmann/json modern JSON library for C++.

If you are using Arch Linux, you can easily install it via the official repositories using your package manager:

``` Bash
sudo pacman -S nlohmann-json
```


## ⚙️ How to Configure
To set up the tool, you need to edit the config.json file located inside the bin/config/ directory.

The mapping associates file extensions with your desired destination directories. For example:

``` JSON
{
  ".txt": "/home/your_username/Documents"
}
```
Note: Make sure to replace the path with the actual directory on your machine where you want your files to be sorted!

🛠️ How to Build
The project uses a Makefile to simplify the build process. To compile the C++ source code and generate the executable in the correct folder, navigate to the project root and run the standard build command with make.

🚀 Project Status
Under active development! Stay tuned for upcoming features and improvements.
