cmake_minimum_required(VERSION 3.16)

# Project configuration
project(QtExplorer VERSION 0.1 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components - Changed from Quick to Widgets!
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Qt standard project setup
qt_standard_project_setup(REQUIRES 6.5)

# Create executable with source files
qt_add_executable(appQtExplorer
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

# Optional: Add resources (icons, images, etc.) - uncomment when needed
# qt_add_resources(appQtExplorer "resources"
#     PREFIX "/"
#     FILES
#         icons/folder.png
#         icons/file.png
#         # Add more resource files here
# )

# Set target properties
set_target_properties(appQtExplorer PROPERTIES
    # Application metadata
    OUTPUT_NAME "QtExplorer"

    # macOS bundle configuration
    MACOSX_BUNDLE_GUI_IDENTIFIER com.yourcompany.qtexplorer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_BUNDLE_NAME "Qt Explorer"
    MACOSX_BUNDLE_INFO_STRING "Cross-platform File Manager"
    MACOSX_BUNDLE TRUE

    # Windows executable configuration
    WIN32_EXECUTABLE TRUE
)

# Link Qt libraries - Changed from Quick to Widgets!
target_link_libraries(appQtExplorer
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

# Compiler-specific options
target_compile_features(appQtExplorer PRIVATE cxx_std_17)

# Platform-specific configurations
if(WIN32)
    # Windows specific settings
    set_target_properties(appQtExplorer PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    # Optional: Set Windows application icon
    # set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
    # target_sources(appQtExplorer PRIVATE ${app_icon_resource_windows})

elseif(APPLE)
    # macOS specific settings
    set_target_properties(appQtExplorer PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.yourcompany.qtexplorer
    )

    # Optional: Set macOS application icon
    # set(MACOSX_BUNDLE_ICON_FILE app.icns)
    # set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.icns")
    # set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    # target_sources(appQtExplorer PRIVATE ${app_icon_macos})

elseif(UNIX)
    # Linux specific settings
    set_target_properties(appQtExplorer PROPERTIES
        OUTPUT_NAME "qtexplorer"
    )
endif()

# Installation configuration
include(GNUInstallDirs)

install(TARGETS appQtExplorer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Optional: Install desktop file on Linux
if(UNIX AND NOT APPLE)
    # Create desktop entry for Linux
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/QtExplorer.desktop.in"
        "${CMAKE_CURRENT_BINARY_DIR}/QtExplorer.desktop"
        @ONLY
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QtExplorer.desktop"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
    )
endif()

# Development configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(appQtExplorer PRIVATE QT_QML_DEBUG)
endif()

# Disable AutoMoc for now - will add back later when needed
set_target_properties(appQtExplorer PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
