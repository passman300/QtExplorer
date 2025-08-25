#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QListView>
#include <QFileSystemModel>

class FileListView : public QListView
{
public:
    enum ViewMode {
        IconView,
        ListView,
        DetailView
    };

    explicit FileListView(QWidget *parent = nullptr);

    void setFileSystemModel(QFileSystemModel *model);
    void setCurrentDirectory(const QString &path);
    QString currentDirectory() const;

    // View modes
    void setCurrentViewMode(ViewMode mode);
    ViewMode currentViewMode() const;

    // Selection
    QStringList selectedFilePaths() const;
    bool hasSelection() const;

    // Navigation
    void navigateUp();
    void navigateToPath(const QString &path);
    void refresh();

protected:
    // Override for custom behavior
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void setupView();
    void updateViewMode();
    void createContextMenu(const QPoint &position);

    QFileSystemModel *m_model;
    ViewMode m_viewMode;
    QString m_currentPath;
};

#endif // FILELISTVIEW_H
