# OpenGL Multi-Project CMake Build System

## 1. Ubuntu (zsh / bash) Dependencies

Run all of these before invoking CMake:

```bash
# 1. Compiler, build tools & CMake
sudo apt update
sudo apt install -y build-essential cmake git ninja-build

# 2. Python3 + Jinja2 (required so CMake can *generate* GLAD at configure time)
sudo apt install -y python3 python3-pip python3-venv python3-jinja2

# 3. X11 / Wayland / display-server development libs (needed by GLFW)
sudo apt install -y libx11-dev libxrandr-dev libxi-dev libxinerama-dev \
                    libxcursor-dev libxkbcommon-dev libwayland-dev \
                    wayland-protocols

# 4. System OpenGL & windowing libraries
sudo apt install -y libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev

# 5. GLFW	sudo apt install -y libglfw3-dev

# 6. GLEW
sudo apt install -y libglew-dev

# 7. GLM
sudo apt install -y libglm-dev

# 8. Hardware detection utility (so CMake can detect your GPU's OpenGL version)
sudo apt install -y mesa-utils
```

> **Note on SOIL2 & GLAD**  
> SOIL2 is **not** packaged for Ubuntu. The root `CMakeLists.txt` will automatically fetch and build it from GitHub during configuration.  
> GLAD is also not a system package — the root `CMakeLists.txt` fetches the *glad2* generator and produces a hardware-matching loader at configure time.

### Ubuntu build command

```bash
cd /path/to/OpenGL_CMake_Project
# On Linux, Ninja is strongly recommended because it handles FetchContent parallelism well.
cmake -B build -S . -G Ninja
cmake --build build
# Executables appear under build/Project_1, build/Project_2, etc.
```

---

## 2. Windows Dependencies

### Prerequisites

1. **Visual Studio 2022** (or the "Build Tools for Visual Studio 2022")  
   Install workload: **"Desktop development with C++"**

2. **CMake** ≥ 3.21  
   Install via the Visual Studio installer or from <https://cmake.org/download/>

3. **Git for Windows**

4. **Python 3** (with pip)  
   Download from <https://python.org> or the Microsoft Store.

5. **Jinja2** (GLAD generator depends on it)  
   Open a terminal and run:
   ```powershell
   pip install jinja2
   ```

> **Note**  
> You **do not** need to manually download GLEW, GLFW, GLM, or SOIL2. The root `CMakeLists.txt` fetches them automatically via CMake's `FetchContent`. You only need the tools above.

### Windows build command (PowerShell / CMD / VS Developer Prompt)

```powershell
cd Z:\path\to\OpenGL_CMake_Project
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
# Or open build/OpenGL_MultiProject.sln in Visual Studio
```

If you prefer **Ninja** (faster, no .sln file):

```powershell
cmake -B build -S . -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
cmake --build build
```

---

## 3. How the GLAD hardware-adaptive generation works

| OS      | Detection mechanism                                                                 |
|---------|--------------------------------------------------------------------------------------|
| **Linux** | CMake runs `glxinfo` (requires `mesa-utils`). It parses the line:  
           `OpenGL core profile version string: X.Y`  
           The detected `X.Y` is passed to the GLAD generator so the loader only exposes functions your GPU actually supports. |
| **Windows** | No runtime GPU detection at configure time (would require compiling & running a test binary).  
             CMake defaults to **OpenGL 4.6 core**. You can override it manually:  
             `cmake -DGLAD_GL_VERSION=4.5 ..` |

If you ever change GPUs or drivers, simply delete the build folder and re-run CMake — GLAD will be regenerated for the new hardware.

---

## 4. Project layout expected by these CMakeLists.txt files

```
OpenGL_CMake_Project/
├── CMakeLists.txt              ← Root (handles all dependencies)
├── README.md                   ← This file
│
├── my_lib/
│   ├── CMakeLists.txt          ← Builds static library my_lib
│   ├── utils.hpp               ← Your shared headers
│   ├── utils.cpp               ← And their implementations
│   └── ...
│
├── Project_1/
│   ├── CMakeLists.txt          ← Builds executable Project_1
│   └── main.cpp                ← Entry point for scene 1
│
├── Project_2/
│   ├── CMakeLists.txt          ← Builds executable Project_2
│   └── main.cpp                ← Entry point for scene 2
│
├── Project_3/                  ← Add more folders as you go…
└── ...
```

### Adding a new scene / subproject

1. Create a folder `Project_N/`.
2. Copy `Project_1/CMakeLists.txt` into it.
3. Rename the project and executable strings inside that file from `Project_1` → `Project_N`.
4. Add a block in the **root** `CMakeLists.txt`:

   ```cmake
   option(BUILD_PROJECT_N "Build Project_N" ON)
   # … later …
   if(BUILD_PROJECT_N)
       add_subdirectory(Project_N)
   endif()
   ```

5. Set it to `OFF` (or comment out the `add_subdirectory` line) when you don't want to rebuild that scene.

---

## 5. Key CMake targets you can link against

If you ever need **finer-grained** control in a specific project (instead of linking the whole `my_lib` bundle):

| Target     | Purpose                                  |
|------------|------------------------------------------|
| `glad`     | Modern OpenGL function loader (GLAD2)    |
| `glfw`     | Window & input context                   |
| `glew` / `GLEW::GLEW` | Legacy extension wrangler      |
| `glm::glm` | Header-only math library                 |
| `SOIL2`    | Image loading (PNG, JPG, DDS, etc.)      |
| `my_lib`   | **Your** shared code & abstractions      |

---

## 6. Troubleshooting

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| `glxinfo: command not found` during configure | `mesa-utils` missing | `sudo apt install mesa-utils` |
| `GLAD generation failed` | Missing Python / Jinja2 | Linux: `sudo apt install python3-jinja2`  <br>Windows: `pip install jinja2` |
| `fatal error: GL/glew.h: No such file` | GLEW fetch failed or not found | Check that Git is installed and you have internet access. CMake will fetch it automatically if the system package is absent. |
| GLFW window creation fails on Linux / Wayland | Missing Wayland dev libs | `sudo apt install libwayland-dev wayland-protocols` |
| Project can't find `my_lib` headers | `add_subdirectory(my_lib)` must appear **before** `add_subdirectory(Project_N)` in the root CMakeLists.txt. | Already handled in the provided root file; keep that order when extending. |
