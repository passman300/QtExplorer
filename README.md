<div align="center">

# 📂 Qt Explorer

### A fast, lightweight, cross-platform file manager built with **C++17** and **Qt 6**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Qt 6](https://img.shields.io/badge/Qt-6.5%2B-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Platforms](https://img.shields.io/badge/Windows%20%7C%20macOS%20%7C%20Linux-lightgrey?style=for-the-badge)

*A clean, familiar, two-pane explorer: browse your folders on the left, work with your files on the right.*

</div>

---

## ✨ Features

| | Feature | Details |
|---|---|---|
| 🌳 | **Directory tree** | Collapsible folder tree in the left pane for quick navigation. Open any folder with a double-click or <kbd>Enter</kbd>. |
| 🗂️ | **Two-pane layout** | Resizable splitter between the tree and the file view. |
| 🖼️ | **Icon & List views** | Switch between a visual icon grid and a compact list from the **View** menu. |
| 📍 | **Address bar** | Always shows the full path of the folder you're in. |
| ⬆️ | **Navigation toolbar** | Back, Forward, Up, Home, New Folder and Refresh in one tidy bar. |
| 📁 | **New folder** | Create a folder in the current directory with <kbd>Ctrl</kbd>+<kbd>N</kbd>. |
| 🔄 | **Refresh** | Reload the current directory with <kbd>F5</kbd>. |
| 🙈 | **Show hidden files** | Toggle hidden files on and off from the View menu. |
| 🚀 | **Open anything** | Folders open in place, and files launch in your system's default app. |
| 💬 | **Tooltips** | Hover a file to see its name, size and last-modified time. |
| 📊 | **Status bar** | Live feedback for actions such as folder creation and refresh. |
| 🖱️ | **Drag & drop ready** | The file list accepts dropped files (move/copy is on the roadmap). |

## ⌨️ Keyboard Shortcuts

| Shortcut | Action |
|---|---|
| <kbd>Enter</kbd> | Open the selected file or folder |
| <kbd>Backspace</kbd> | Go up one level |
| <kbd>F5</kbd> | Refresh |
| <kbd>Ctrl</kbd>+<kbd>N</kbd> | New folder |
| <kbd>Ctrl</kbd>+<kbd>C</kbd> / <kbd>X</kbd> / <kbd>V</kbd> | Copy / Cut / Paste *(menu entries in place, wiring in progress)* |
| <kbd>Del</kbd> | Delete *(wiring in progress)* |
| <kbd>F2</kbd> | Rename *(wiring in progress)* |
| <kbd>Ctrl</kbd>+<kbd>Q</kbd> | Quit |

## 🧱 Project Structure

```
QtExplorer/
├── main.cpp                 # Application entry point
├── mainwindow.{h,cpp}       # Main window: menus, toolbar, splitter, status bar
├── directorytreeview.{h,cpp}# Left-hand folder tree
├── filelistview.{h,cpp}     # Right-hand icon/list view with keyboard + drop handling
├── filesystemmodel.{h,cpp}  # QFileSystemModel subclass (hidden-file filter, tooltips, file ops)
└── CMakeLists.txt           # Build configuration
```

## 🛠️ Build

> No prebuilt binaries are included in this repo. Build it from source, which only takes a minute.

### Prerequisites

| Tool | Version |
|---|---|
| [Qt](https://www.qt.io/download-qt-installer) (Core, Widgets) | 6.5 or newer |
| [CMake](https://cmake.org/download/) | 3.16 or newer |
| C++ compiler | C++17 (MinGW, MSVC, GCC or Clang) |

### Option 1: Qt Creator (easiest)

1. Open **Qt Creator** and choose **File → Open File or Project…**
2. Select `CMakeLists.txt` and pick a Qt 6 kit (for example *Desktop Qt 6.9.1 MinGW 64-bit*).
3. Press ▶ **Run**.

### Option 2: Command line

```bash
git clone https://github.com/passman300/QtExplorer.git
cd QtExplorer

# configure (point CMAKE_PREFIX_PATH at your Qt kit)
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/<kit>

# compile
cmake --build build --config Release

# run
./build/QtExplorer            # Windows: build\QtExplorer.exe
```

**Windows (MinGW) example:**

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.9.1/mingw_64
cmake --build build
.\build\QtExplorer.exe
```

The `build/` folder is git-ignored, so your local builds never end up in the repo.

## 🗺️ Roadmap

- [x] Directory tree + file list panes
- [x] Icon / List view modes
- [x] Open files and folders, up-navigation, new folder, refresh
- [x] Show / hide hidden files
- [ ] Back / Forward history
- [ ] Editable address bar
- [ ] Copy, cut, paste, delete and rename
- [ ] Right-click context menu
- [ ] Drag-and-drop move/copy
- [ ] Custom icons and a dark theme

## 🧰 Built With

- [Qt 6 Widgets](https://doc.qt.io/qt-6/qtwidgets-index.html): `QFileSystemModel`, `QTreeView`, `QListView`, `QSplitter`
- [CMake](https://cmake.org/)

---

<div align="center">

Made with ☕ and Qt by [@passman300](https://github.com/passman300)

</div>
