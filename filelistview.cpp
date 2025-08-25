#include "FileListView.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileInfo>
#include <QDir>
#include <QMimeData>
#include <QUrl>
#include <QApplication>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QDateTime>
#include <QFileIconProvider>

FileListView::FileListView(QWidget *parent)
    : QListView(parent)
    , m_model(nullptr)
    , m_viewMode(IconView)
    , m_currentPath("")
{
    setupView();
}

void FileListView::setupView()
{
    // Basic setup
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setGridSize(QSize(100, 100));
    setMovement(QListView::Static);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionRectVisible(true);

    // Enable drag and drop
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    // Visual improvements
    setAlternatingRowColors(true);
    setUniformItemSizes(false);
    setWordWrap(true);

    // Context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void FileListView::setFileSystemModel(QFileSystemModel *model)
{
    if (m_model != model) {
        m_model = model;
        setModel(model);
        // Set custom delegate so list view shows metadata in rows
        setItemDelegate(new FileItemDelegate(model, this));
    }
}

void FileListView::setCurrentDirectory(const QString &path)
{
    if (m_model && m_currentPath != path) {
        m_currentPath = path;
        QModelIndex index = m_model->index(path);
        if (index.isValid()) {
            setRootIndex(index);
        }
    }
}

QString FileListView::currentDirectory() const
{
    return m_currentPath;
}

void FileListView::setCurrentViewMode(ViewMode mode)
{
    if (m_viewMode != mode) {
        m_viewMode = mode;
        updateViewMode();
    }
}

FileListView::ViewMode FileListView::currentViewMode() const
{
    return m_viewMode;
}

void FileListView::updateViewMode()
{
    switch (m_viewMode) {
    case IconView:
    setViewMode(QListView::IconMode);
    setGridSize(QSize(100, 100));
    setIconSize(QSize(64, 64));
    setUniformItemSizes(false);
    setFlow(QListView::LeftToRight);
    setSpacing(10);
        break;

    case ListView:
    setViewMode(QListView::ListMode);
    setGridSize(QSize());
    setIconSize(QSize(16, 16));
    setUniformItemSizes(true);
    setFlow(QListView::TopToBottom);
    setSpacing(4);
        break;

    case DetailView:
        // For detail view, we might need to switch to QTreeView
        // For now, use list mode with smaller items
    setViewMode(QListView::ListMode);
    setGridSize(QSize(200, 20));
    setIconSize(QSize(16, 16));
    setUniformItemSizes(true);
    setFlow(QListView::TopToBottom);
    setSpacing(2);
        break;
    }
}

QStringList FileListView::selectedFilePaths() const
{
    QStringList paths;
    if (!m_model) return paths;

    QModelIndexList selected = selectionModel()->selectedIndexes();
    for (const QModelIndex &index : selected) {
        paths << m_model->filePath(index);
    }

    return paths;
}

bool FileListView::hasSelection() const
{
    return selectionModel() && selectionModel()->hasSelection();
}

void FileListView::navigateUp()
{
    if (m_currentPath.isEmpty()) return;

    QDir currentDir(m_currentPath);
    if (currentDir.cdUp()) {
        setCurrentDirectory(currentDir.absolutePath());
    }
}

void FileListView::navigateToPath(const QString &path)
{
    setCurrentDirectory(path);
}

void FileListView::refresh()
{
    if (m_model) {
        // Force model refresh
        m_model->setRootPath(m_model->rootPath());
        setCurrentDirectory(m_currentPath);
    }
}

void FileListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (index.isValid() && m_model) {
        QFileInfo info = m_model->fileInfo(index);
        if (info.isDir()) {
            // Navigate to directory
            setCurrentDirectory(info.absoluteFilePath());
        } else {
            // TODO: Open file with default application
            // For now, do nothing
        }
    }

    QListView::mouseDoubleClickEvent(event);
}

void FileListView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter: {
        QModelIndex index = currentIndex();
        if (index.isValid() && m_model) {
            QFileInfo info = m_model->fileInfo(index);
            if (info.isDir()) {
                setCurrentDirectory(info.absoluteFilePath());
            }
        }
        break;
    }

    case Qt::Key_Backspace: {
        navigateUp();
        break;
    }

    case Qt::Key_F5: {
        refresh();
        break;
    }

    default:
        QListView::keyPressEvent(event);
    }
}

void FileListView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void FileListView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void FileListView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        // TODO: Implement file move/copy operations
        // For now, just accept the drop
        event->acceptProposedAction();
    }
}

void FileListView::contextMenuEvent(QContextMenuEvent *event)
{
    createContextMenu(event->pos());
}

void FileListView::createContextMenu(const QPoint &position)
{
    QModelIndex index = indexAt(position);

    QMenu contextMenu(this);

    if (index.isValid()) {
        // File/folder specific actions
        contextMenu.addAction("Open", [this, index]() {
            // TODO: Implement open action
        });

        contextMenu.addSeparator();

        contextMenu.addAction("Copy", [this]() {
            // TODO: Implement copy
        });

        contextMenu.addAction("Cut", [this]() {
            // TODO: Implement cut
        });

        contextMenu.addSeparator();

        contextMenu.addAction("Delete", [this]() {
            // TODO: Implement delete
        });

        contextMenu.addAction("Rename", [this]() {
            // TODO: Implement rename
        });

        contextMenu.addSeparator();

        contextMenu.addAction("Properties", [this, index]() {
            // TODO: Show properties dialog
        });
    } else {
        // Empty space actions
        contextMenu.addAction("New Folder", [this]() {
            // TODO: Implement new folder
        });

        contextMenu.addSeparator();

        contextMenu.addAction("Paste", [this]() {
            // TODO: Implement paste
        });

        contextMenu.addSeparator();

        contextMenu.addAction("Refresh", [this]() {
            refresh();
        });
    }

    contextMenu.exec(mapToGlobal(position));
}

// Custom delegate to show name and metadata on two lines
class FileItemDelegate : public QStyledItemDelegate {
public:
    FileItemDelegate(QFileSystemModel *model, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_model(model) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (!m_model) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        painter->save();

        QRect rect = option.rect;
        // Draw background
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);
        opt.text = ""; // we'll draw text ourselves
        opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        // Get file info
        QFileInfo info = m_model->fileInfo(index);

        // Icon
        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QSize iconSize = QSize(32, 32);
        QRect iconRect = QRect(rect.left() + 4, rect.top() + (rect.height() - iconSize.height())/2, iconSize.width(), iconSize.height());
        icon.paint(painter, iconRect, Qt::AlignCenter);

        // Text area
        int x = iconRect.right() + 8;
        int w = rect.width() - (x - rect.left()) - 8;
        QRect nameRect(x, rect.top() + 4, w, rect.height()/2 - 4);
        QRect metaRect(x, rect.top() + rect.height()/2, w, rect.height()/2 - 6);

        // Filename
        QFont nameFont = option.font;
        nameFont.setBold(true);
        painter->setFont(nameFont);
        painter->setPen(option.palette.color(QPalette::Text));
        QString displayName = info.fileName();
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elideText(painter, displayName, w));

        // Metadata: type • size • modified
        QFont metaFont = option.font;
        metaFont.setPointSize(metaFont.pointSize() - 1);
        painter->setFont(metaFont);
        painter->setPen(option.palette.color(QPalette::Mid));

        QString type = m_model->type(index);
        QString size = info.isDir() ? QStringLiteral("") : humanReadableSize(info.size());
        QString modified = info.lastModified().toString(Qt::DefaultLocaleShortDate);

        QStringList parts;
        if (!type.isEmpty()) parts << type;
        if (!size.isEmpty()) parts << size;
        if (!modified.isEmpty()) parts << modified;

        QString meta = parts.join("  •  ");
        painter->drawText(metaRect, Qt::AlignLeft | Qt::AlignVCenter, elideText(painter, meta, w));

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(index);
        return QSize(option.rect.width(), 48);
    }

private:
    QFileSystemModel *m_model;

    static QString humanReadableSize(qint64 bytes) {
        if (bytes < 1024) return QString::number(bytes) + " B";
        static const char *sizes[] = {"KB", "MB", "GB", "TB"};
        double s = bytes / 1024.0;
        int i = 0;
        while (s >= 1024.0 && i < 3) { s /= 1024.0; ++i; }
        return QString::asprintf("%.1f %s", s, sizes[i]);
    }

    static QString elideText(QPainter *painter, const QString &text, int width) {
        QFontMetrics fm(painter->font());
        return fm.elidedText(text, Qt::ElideRight, width);
    }
};
