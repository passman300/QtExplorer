#include "MainWindow.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralSplitter(nullptr)
    , m_leftPanel(nullptr)
    , m_rightPanel(nullptr)
    , m_treeView(nullptr)
    , m_listView(nullptr)
    , m_fileSystemModel(nullptr)
    , m_addressBar(nullptr)
    , m_statusLabel(nullptr)
    , m_progressBar(nullptr)
    , m_currentPath("")
{
    setupUI();
    setupFileSystem();

    // Set initial window properties
    setWindowTitle("File Manager");
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

    QLabel *treeLabel = new QLabel("Directories");
    treeLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    leftLayout->addWidget(treeLabel);

    m_treeView = new QTreeView();
    m_treeView->setHeaderHidden(true);
    m_treeView->setRootIsDecorated(true);
    leftLayout->addWidget(m_treeView);

    // Create right panel (file list)
    m_rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(m_rightPanel);
    rightLayout->setContentsMargins(5, 5, 5, 5);

    // Address bar
    QHBoxLayout *addressLayout = new QHBoxLayout();
    QLabel *addressLabel = new QLabel("Location:");
    m_addressBar = new QLineEdit();
    m_addressBar->setReadOnly(true);

    addressLayout->addWidget(addressLabel);
    addressLayout->addWidget(m_addressBar);
    rightLayout->addLayout(addressLayout);

    // File list view
    m_listView = new QListView();
    m_listView->setViewMode(QListView::IconMode);
    m_listView->setResizeMode(QListView::Adjust);
    m_listView->setGridSize(QSize(100, 100));
    m_listView->setMovement(QListView::Static);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    rightLayout->addWidget(m_listView);

    // Add panels to splitter
    m_centralSplitter->addWidget(m_leftPanel);
    m_centralSplitter->addWidget(m_rightPanel);

    // Set splitter proportions (30% left, 70% right)
    m_centralSplitter->setSizes(QList<int>{300, 700});
}

void MainWindow::setupFileSystem()
{
    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setRootPath(QDir::rootPath());

    // Setup tree view (directories only)
    QFileSystemModel *treeModel = new QFileSystemModel(this);
    treeModel->setRootPath(QDir::rootPath());
    treeModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    m_treeView->setModel(treeModel);
    m_treeView->setRootIndex(treeModel->index(QDir::rootPath()));

    // Hide all columns except name in tree view
    for (int i = 1; i < treeModel->columnCount(); ++i) {
        m_treeView->hideColumn(i);
    }

    // Setup list view (files and folders)
    m_listView->setModel(m_fileSystemModel);

    // Note: Without MOC, signal-slot connections won't work
    // We'll add them back when we enable MOC
}

void MainWindow::navigateToPath(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return; // Can't show error dialog without signals/slots
    }

    m_currentPath = QDir::toNativeSeparators(dir.absolutePath());
    m_addressBar->setText(m_currentPath);

    // Update list view
    QModelIndex index = m_fileSystemModel->index(m_currentPath);
    m_listView->setRootIndex(index);

    // Update tree view selection
    QFileSystemModel *treeModel = qobject_cast<QFileSystemModel*>(m_treeView->model());
    if (treeModel) {
        QModelIndex treeIndex = treeModel->index(m_currentPath);
        if (treeIndex.isValid()) {
            m_treeView->setCurrentIndex(treeIndex);
            m_treeView->expand(treeIndex);
        }
    }
}
