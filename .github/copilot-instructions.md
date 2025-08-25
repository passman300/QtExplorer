# AI Coding Agent Instructions for QtExplorer

## Project Overview
QtExplorer is a cross-platform file manager application built using the Qt framework. The project uses the Qt Widgets module for its GUI and adheres to modern C++ standards (C++17). The application is structured to provide a clean separation of concerns between the GUI components and the underlying file system logic.

### Key Components
- **MainWindow**: The primary window of the application, defined in `mainwindow.cpp` and `mainwindow.h`.
- **FileSystemModel**: Encapsulates the logic for interacting with the file system, located in `filesystemmodel.cpp` and `filesystemmodel.h`.
- **DirectoryTreeView**: Manages the display of the directory structure, implemented in `directorytreeview.cpp` and `directorytreeview.h`.
- **FileListView**: Handles the display of files within a selected directory, implemented in `filelistview.cpp` and `filelistview.h`.
- **Main.qml**: Placeholder for potential QML integration, though the current implementation focuses on Qt Widgets.

## Developer Workflows

### Building the Project
The project uses CMake for its build system. To build the project:
1. Navigate to the `build/` directory.
2. Run the following commands:
   ```cmd
   cmake ..
   cmake --build .
   ```
3. The executable `QtExplorer.exe` will be generated in the `build/` directory.

### Running the Application
After building, run the application by executing:
```cmd
build\QtExplorer.exe
```

### Debugging
- Debug builds include the `QT_QML_DEBUG` definition for enhanced debugging capabilities.
- Use a debugger compatible with Qt and C++17 (e.g., Qt Creator or Visual Studio).

## Project-Specific Conventions

### C++ Standards
- The project uses C++17. Ensure all new code adheres to this standard.
- Use `std::unique_ptr` and `std::shared_ptr` for memory management where applicable.

### Qt Conventions
- Widgets and GUI components should only be created after initializing `QApplication`.
- Use `qt_standard_project_setup` for consistent project configuration.
- Avoid mixing QML and Widgets unless explicitly required.

### File Organization
- Header files (`.h`) and implementation files (`.cpp`) are paired and located in the root directory.
- Resources (e.g., icons) should be added to the `resources/` directory and linked via `qt_add_resources` in `CMakeLists.txt`.

## Integration Points

### External Dependencies
- The project depends on the Qt6 framework, specifically the `Core` and `Widgets` modules.
- Ensure Qt6 is installed and properly configured in your development environment.

### Cross-Component Communication
- Use Qt's signal-slot mechanism for communication between components.
- Avoid direct dependencies between unrelated components to maintain modularity.

## Examples

### Adding a New Widget
1. Create a new pair of `.h` and `.cpp` files for the widget.
2. Add the files to the `qt_add_executable` section in `CMakeLists.txt`.
3. Implement the widget using Qt's `QWidget` or its derivatives.

### Modifying the Build Configuration
To enable AutoMoc, AutoUIC, or AutoRCC, update the `CMakeLists.txt`:
```cmake
set_target_properties(appQtExplorer PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
```

## Notes
- The project is configured for cross-platform compatibility. Platform-specific settings are defined in `CMakeLists.txt`.
- For Linux, a `.desktop` file is generated for application integration.

---

For any unclear or incomplete sections, please provide feedback to refine these instructions.
