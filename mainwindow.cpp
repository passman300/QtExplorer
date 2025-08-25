#include "MainWindow.h"
#include "FileSystemModel.h"
#include "DirectoryTreeView.h"
#include "FileListView.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QStatusBar>
#include <QMenuBar>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralSplitter(nullptr)
    , m_leftPanel(nullptr)
    , m_rightPanel(nullptr)
    , m_fileSystemModel(nullptr)
    , m_directoryTree(nullptr)
    , m_fileList(nullptr)
    , m_addressBar(nullptr)
    , m_statusLabel(nullptr)
    , m_progressBar(nullptr)
    , m_fileMenu(nullptr)
    , m_editMenu(nullptr)
    , m_viewMenu(nullptr)
    , m_mainToolBar(nullptr)
    , m_newFolderAction(nullptr)
    , m_copyAction(nullptr)
    , m_cutAction(nullptr)
    , m_pasteAction(nullptr)
    , m_deleteAction(nullptr)
    , m_renameAction(nullptr)
    , m_refreshAction(nullptr)
    , m_backAction(nullptr)
    , m_forwardAction(nullptr)
    , m_upAction(nullptr)
    , m_homeAction(nullptr)
    , m_iconViewAction(nullptr)
    , m_listViewAction(nullptr)
    , m_showHiddenAction(nullptr)
    , m_currentPath("")
{
    setupUI();
    createMenus();
    createToolBars();
    createStatusBar();
    setupComponents();
    connectSignals();

    // Set initial window properties
    setWindowTitle("Qt File Explorer");
    setMinimumSize(800, 600);
    resize(1200, 800);

    // Navigate to home directory initially
    navigateToPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
}

MainWindow::~MainWindow()
{
    // Qt handles cleanup of child widgets automatically
}

void MainWindow::setupUI()
{
    // Create central splitter
    m_centralSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(m_centralSplitter);

    // Create left panel (directory tree)
    m_leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    leftLayout->setSpacing(5);

    QLabel *treeLabel = new QLabel("Directories");
    treeLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    leftLayout->addWidget(treeLabel);

    // Directory tree will be created in setupComponents()

    // Create right panel (file list)
    m_rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(m_rightPanel);
    rightLayout->setContentsMargins(5, 5, 5, 5);
    rightLayout->setSpacing(5);

    // Address bar
    QHBoxLayout *addressLayout = new QHBoxLayout();
    addressLayout->setSpacing(5);

    QLabel *addressLabel = new QLabel("Location:");
    m_addressBar = new QLineEdit();
    m_addressBar->setReadOnly(true);
    // m_addressBar->setStyleSheet("QLineEdit { background-color: #f8f8f8; }");

    addressLayout->addWidget(addressLabel);
    addressLayout->addWidget(m_addressBar);
    rightLayout->addLayout(addressLayout);

    // File list will be created in setupComponents()

    // Add panels to splitter
    m_centralSplitter->addWidget(m_leftPanel);
    m_centralSplitter->addWidget(m_rightPanel);

    // Set splitter proportions (25% left, 75% right)
    m_centralSplitter->setSizes(QList<int>{250, 750});
    m_centralSplitter->setStretchFactor(0, 0);
    m_centralSplitter->setStretchFactor(1, 1);
}

void MainWindow::createMenus()
{
    // File Menu
    m_fileMenu = menuBar()->addMenu("&File");

    m_newFolderAction = m_fileMenu->addAction("&New Folder");
    m_newFolderAction->setShortcut(QKeySequence::New);
    m_newFolderAction->setStatusTip("Create a new folder");

    m_fileMenu->addSeparator();

    m_refreshAction = m_fileMenu->addAction("&Refresh");
    m_refreshAction->setShortcut(QKeySequence::Refresh);
    m_refreshAction->setStatusTip("Refresh current directory");

    m_fileMenu->addSeparator();

    QAction *exitAction = m_fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");

    // Edit Menu
    m_editMenu = menuBar()->addMenu("&Edit");

    m_copyAction = m_editMenu->addAction("&Copy");
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setStatusTip("Copy selected items");

    m_cutAction = m_editMenu->addAction("Cu&t");
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_cutAction->setStatusTip("Cut selected items");

    m_pasteAction = m_editMenu->addAction("&Paste");
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_pasteAction->setStatusTip("Paste items from clipboard");

    m_editMenu->addSeparator();

    m_deleteAction = m_editMenu->addAction("&Delete");
    m_deleteAction->setShortcut(QKeySequence::Delete);
    m_deleteAction->setStatusTip("Delete selected items");

    m_renameAction = m_editMenu->addAction("&Rename");
    m_renameAction->setShortcut(Qt::Key_F2);
    m_renameAction->setStatusTip("Rename selected item");

    // View Menu
    m_viewMenu = menuBar()->addMenu("&View");

    m_iconViewAction = m_viewMenu->addAction("&Icon View");
    m_iconViewAction->setCheckable(true);
    m_iconViewAction->setChecked(true);
    m_iconViewAction->setStatusTip("Show files as icons");

    m_listViewAction = m_viewMenu->addAction("&List View");
    m_listViewAction->setCheckable(true);
    m_listViewAction->setStatusTip("Show files as a list");

    // Create view mode group
    QActionGroup *viewModeGroup = new QActionGroup(this);
    viewModeGroup->addAction(m_iconViewAction);
    viewModeGroup->addAction(m_listViewAction);

    m_viewMenu->addSeparator();

    m_showHiddenAction = m_viewMenu->addAction("Show &Hidden Files");
    m_showHiddenAction->setCheckable(true);
    m_showHiddenAction->setStatusTip("Show hidden files and folders");
}

void MainWindow::createToolBars()
{
    m_mainToolBar = addToolBar("Main");
    m_mainToolBar->setMovable(false);

    // Navigation buttons
    m_backAction = m_mainToolBar->addAction("◀ Back");
    m_backAction->setStatusTip("Go back to previous directory");

    m_forwardAction = m_mainToolBar->addAction("▶ Forward");
    m_forwardAction->setStatusTip("Go forward to next directory");

    m_upAction = m_mainToolBar->addAction("▲ Up");
    m_upAction->setStatusTip("Go to parent directory");

    m_mainToolBar->addSeparator();

    m_homeAction = m_mainToolBar->addAction("🏠 Home");
    m_homeAction->setStatusTip("Go to home directory");

    m_mainToolBar->addSeparator();

    // File operations
    m_mainToolBar->addAction(m_newFolderAction);
    m_mainToolBar->addAction(m_refreshAction);
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel);

    // Add a progress bar for future operations
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);
    statusBar()->addPermanentWidget(m_progressBar);
}

void MainWindow::setupComponents()
{
    // Create and setup file system model
    m_fileSystemModel = new FileSystemModel(this);

    // Create directory tree
    m_directoryTree = new DirectoryTreeView(m_leftPanel, false); // show files as well
    m_directoryTree->setFileSystemModel(m_fileSystemModel);
    m_directoryTree->setShowOnlyDirectories(false); // show files as well as folders

    // Add tree to left panel layout
    QVBoxLayout *leftLayout = qobject_cast<QVBoxLayout*>(m_leftPanel->layout());
    if (leftLayout) {
        leftLayout->addWidget(m_directoryTree);
    }

    // Create file list
    m_fileList = new FileListView(m_rightPanel);
    m_fileList->setFileSystemModel(m_fileSystemModel);
    m_fileList->setCurrentViewMode(FileListView::IconView);

    // Add file list to right panel layout
    QVBoxLayout *rightLayout = qobject_cast<QVBoxLayout*>(m_rightPanel->layout());
    if (rightLayout) {
        rightLayout->addWidget(m_fileList);
    }
}

void MainWindow::connectSignals()
{
    // Note: Since we disabled MOC, these connections won't work yet
    // We'll enable them when we turn MOC back on

    // For now, we can connect to basic Qt signals that don't need MOC
    // Most custom signals will need MOC to be enabled

    // Connect basic widget signals (these work without MOC in our custom classes)
    // The custom signal-slot connections will be added when MOC is enabled

    connect(m_upAction, &QAction::triggered, this, &MainWindow::onUpActionTriggered);
    connect(m_newFolderAction, &QAction::triggered, this, &MainWindow::onNewFolderActionTriggered);
    connect(m_refreshAction, &QAction::triggered, this, &MainWindow::onRefreshActionTriggered);

    // Open files/folders from the directory tree
    if (m_directoryTree && m_fileSystemModel) {
        auto openFromIndex = [this](const QModelIndex &index) {
            if (!index.isValid() || !m_fileSystemModel) return;
            QFileInfo info = m_fileSystemModel->fileInfo(index);
            QString path = info.absoluteFilePath();
            if (info.isDir()) {
                navigateToPath(path);
            } else {
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            }
        };

        connect(m_directoryTree, &QTreeView::doubleClicked, this, openFromIndex);
        connect(m_directoryTree, &QTreeView::activated, this, openFromIndex);
    }

    // Open files/folders from the file list (activated/double-click)
    if (m_fileList && m_fileSystemModel) {
        connect(m_fileList, &QListView::activated, this, [this](const QModelIndex &index) {
            if (!index.isValid() || !m_fileSystemModel) return;
            QFileInfo info = m_fileSystemModel->fileInfo(index);
            QString path = info.absoluteFilePath();
            if (info.isDir()) {
                navigateToPath(path);
            } else {
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            }
        });
    }

    // View mode actions
    if (m_iconViewAction && m_listViewAction && m_fileList) {
        connect(m_iconViewAction, &QAction::triggered, this, [this]() {
            if (m_fileList) m_fileList->setCurrentViewMode(FileListView::IconView);
        });

        connect(m_listViewAction, &QAction::triggered, this, [this]() {
            if (m_fileList) m_fileList->setCurrentViewMode(FileListView::ListView);
        });
    }

    // Show hidden files toggle
    if (m_showHiddenAction && m_fileSystemModel) {
        connect(m_showHiddenAction, &QAction::toggled, this, [this](bool checked) {
            if (m_fileSystemModel) m_fileSystemModel->setShowHiddenFiles(checked);
        });
    }

    // End of connectSignals
}

void MainWindow::navigateToPath(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return; // Can't show error without signal/slot
    }

    m_currentPath = QDir::toNativeSeparators(dir.absolutePath());
    updateAddressBar(m_currentPath);

    // Update both views
    if (m_directoryTree) {
        m_directoryTree->navigateToPath(m_currentPath);
    }

    if (m_fileList) {
        m_fileList->setCurrentDirectory(m_currentPath);
    }

    updateStatus();
}

void MainWindow::updateStatus(const QString &message)
{
    if (!message.isEmpty()) {
        m_statusLabel->setText(message);
        return;
    }

    if (!m_fileSystemModel || m_currentPath.isEmpty()) {
        m_statusLabel->setText("Ready");
        return;
    }

    int fileCount = m_fileSystemModel->getFileCount(m_currentPath);
    int dirCount = m_fileSystemModel->getDirectoryCount(m_currentPath);
    int totalItems = fileCount + dirCount;

    QString status = QString("%1 items (%2 folders, %3 files)")
                         .arg(totalItems)
                         .arg(dirCount)
                         .arg(fileCount);

    m_statusLabel->setText(status);
}

void MainWindow::updateAddressBar(const QString &path)
{
    if (m_addressBar) {
        m_addressBar->setText(path);
    }
}

void MainWindow::onUpActionTriggered()
{
    // Navigate to the parent directory
    QDir dir(m_currentPath);
    if (dir.cdUp()) {
        navigateToPath(dir.absolutePath());
    }
}

void MainWindow::onNewFolderActionTriggered()
{
    // Create a new folder in the current directory
    QString newFolderPath = QDir(m_currentPath).filePath("New Folder");
    if (QDir().mkdir(newFolderPath)) {
        updateStatus("Folder created: " + newFolderPath);
    } else {
        updateStatus("Failed to create folder.");
    }
}

void MainWindow::onRefreshActionTriggered()
{
    // Refresh the current directory view
    navigateToPath(m_currentPath);
    updateStatus("Directory refreshed.");
}
