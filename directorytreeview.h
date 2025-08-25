#ifndef DIRECTORYTREEVIEW_H
#define DIRECTORYTREEVIEW_H

#include <QTreeView>
#include <QFileSystemModel>

class DirectoryTreeView : public QTreeView
{
public:
    explicit DirectoryTreeView(QWidget *parent = nullptr, bool showOnlyDirectories = false);

    void setFileSystemModel(QFileSystemModel *model);
    void navigateToPath(const QString &path);
    QString currentPath() const;

    // Customization
    void setShowOnlyDirectories(bool directoriesOnly);
    bool showOnlyDirectories() const { return m_showOnlyDirectories; }
    void expandToPath(const QString &path);

protected:
    // Override for custom behavior
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupView();
    QFileSystemModel *m_model;
    bool m_showOnlyDirectories;
};

#endif // DIRECTORYTREEVIEW_H
