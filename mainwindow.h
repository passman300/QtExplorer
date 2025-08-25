#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

// Forward declarations for our custom classes
class FileSystemModel;
class DirectoryTreeView;
class FileListView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Setup methods
    void setupUI();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupComponents();
    void connectSignals();

    // Helper methods
    void navigateToPath(const QString &path);
    void updateStatus(const QString &message = QString());
    void updateAddressBar(const QString &path);

    // UI components - Main layout
    QSplitter *m_centralSplitter;
    QWidget *m_leftPanel;
    QWidget *m_rightPanel;

    // Custom views and models
    FileSystemModel *m_fileSystemModel;
    DirectoryTreeView *m_directoryTree;
    FileListView *m_fileList;

    // UI controls
    QLineEdit *m_addressBar;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;

    // Menus and toolbars
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QToolBar *m_mainToolBar;

    // Actions
    QAction *m_newFolderAction;
    QAction *m_copyAction;
    QAction *m_cutAction;
    QAction *m_pasteAction;
    QAction *m_deleteAction;
    QAction *m_renameAction;
    QAction *m_refreshAction;
    QAction *m_backAction;
    QAction *m_forwardAction;
    QAction *m_upAction;
    QAction *m_homeAction;
    QAction *m_iconViewAction;
    QAction *m_listViewAction;
    QAction *m_showHiddenAction;

    // Application state
    QString m_currentPath;

private slots:
    void onUpActionTriggered();
    void onNewFolderActionTriggered();
    void onRefreshActionTriggered();
};

#endif // MAINWINDOW_H
