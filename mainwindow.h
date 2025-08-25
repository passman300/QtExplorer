#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>

// Forward declarations
QT_BEGIN_NAMESPACE
class QMenu;
class QToolBar;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    // Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation slots
    void onTreeSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onListDoubleClicked(const QModelIndex &index);

    // File operation slots
    void onNewFolder();
    void onCopy();
    void onCut();
    void onPaste();
    void onDelete();
    void onRename();
    void onRefresh();

    // Navigation slots
    void onBack();
    void onForward();
    void onUp();
    void onHome();

    // View slots
    void onIconView();
    void onListView();
    void onToggleHidden(bool show);

private:
    // Setup methods
    void setupUI();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupFileSystem();

    // Helper methods
    void navigateToPath(const QString &path);
    void updateStatus();
    void copySelectedItems(bool cut);

    // UI components - Main layout
    QSplitter *m_centralSplitter;
    QWidget *m_leftPanel;
    QWidget *m_rightPanel;

    // Views and models
    QTreeView *m_treeView;
    QListView *m_listView;
    QFileSystemModel *m_fileSystemModel;

    // UI controls
    QLineEdit *m_addressBar;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;

    // Menus and toolbars
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QToolBar *m_mainToolBar;

    // Application state
    QString m_currentPath;
};

#endif // MAINWINDOW_H
