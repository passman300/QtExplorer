#include "DirectoryTreeView.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHeaderView>
#include <QDir>

DirectoryTreeView::DirectoryTreeView(QWidget *parent, bool showOnlyDirectories)
    : QTreeView(parent)
    , m_model(nullptr)
    , m_showOnlyDirectories(showOnlyDirectories)
{
    setupView();
}

void DirectoryTreeView::setupView()
{
    setHeaderHidden(true);
    setRootIsDecorated(true);
    setAnimated(true);
    setIndentation(20);
    setExpandsOnDoubleClick(true);

    // Selection behavior
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    // Visual improvements
    setAlternatingRowColors(false);
    setUniformRowHeights(true);
}

void DirectoryTreeView::setFileSystemModel(QFileSystemModel *model)
{
    if (m_model != model) {
        m_model = model;

        if (model) {
            setModel(model);
            setRootIndex(model->index(QDir::rootPath()));

            // Hide all columns except name for tree view
            for (int i = 1; i < model->columnCount(); ++i) {
                hideColumn(i);
            }

            setShowOnlyDirectories(m_showOnlyDirectories);
        }
    }
}

void DirectoryTreeView::navigateToPath(const QString &path)
{
    if (!m_model) return;

    QModelIndex index = m_model->index(path);
    if (index.isValid()) {
        setCurrentIndex(index);
        expandToPath(path);
        scrollTo(index, QAbstractItemView::PositionAtCenter);
    }
}

QString DirectoryTreeView::currentPath() const
{
    if (!m_model) return QString();

    QModelIndex current = currentIndex();
    if (current.isValid()) {
        return m_model->filePath(current);
    }

    return QString();
}

void DirectoryTreeView::setShowOnlyDirectories(bool directoriesOnly)
{
    m_showOnlyDirectories = directoriesOnly;

    if (m_model) {
        if (directoriesOnly) {
            m_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
        } else {
            m_model->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
        }
    }
}

void DirectoryTreeView::expandToPath(const QString &path)
{
    if (!m_model) return;

    QDir dir(path);
    QString currentPath = dir.absolutePath();

    // Expand all parent directories
    while (!currentPath.isEmpty() && currentPath != QDir::rootPath()) {
        QModelIndex index = m_model->index(currentPath);
        if (index.isValid()) {
            expand(index);
        }

        dir.cdUp();
        currentPath = dir.absolutePath();
    }
}

void DirectoryTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (index.isValid() && m_model) {
        QFileInfo info = m_model->fileInfo(index);
        if (info.isDir()) {
            // Expand/collapse directory
            if (isExpanded(index)) {
                collapse(index);
            } else {
                expand(index);
            }
        }
    }

    QTreeView::mouseDoubleClickEvent(event);
}

void DirectoryTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QModelIndex index = currentIndex();
        if (index.isValid()) {
            if (isExpanded(index)) {
                collapse(index);
            } else {
                expand(index);
            }
        }
    }

    QTreeView::keyPressEvent(event);
}
