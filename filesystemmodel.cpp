#include "FileSystemModel.h"
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

FileSystemModel::FileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
    , m_showHiddenFiles(false)
{
    setRootPath(QDir::rootPath());
    setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
}

void FileSystemModel::setShowHiddenFiles(bool show)
{
    if (m_showHiddenFiles != show) {
        m_showHiddenFiles = show;

        QDir::Filters filters = QDir::NoDotAndDotDot | QDir::AllEntries;
        if (show) {
            filters |= QDir::Hidden;
        }
        setFilter(filters);
    }
}

bool FileSystemModel::showHiddenFiles() const
{
    return m_showHiddenFiles;
}

bool FileSystemModel::createDirectory(const QString &path, const QString &name)
{
    QDir dir(path);
    return dir.mkdir(name);
}

bool FileSystemModel::removeFile(const QString &filePath)
{
    QFileInfo info(filePath);
    if (info.isFile()) {
        return QFile::remove(filePath);
    }
    return false;
}

bool FileSystemModel::removeDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    return dir.removeRecursively();
}

bool FileSystemModel::renameFile(const QString &oldPath, const QString &newName)
{
    QFileInfo info(oldPath);
    QDir dir = info.dir();
    QString newPath = dir.absoluteFilePath(newName);
    return dir.rename(info.fileName(), newName);
}

int FileSystemModel::getFileCount(const QString &dirPath) const
{
    QDir dir(dirPath);
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    return entries.count();
}

int FileSystemModel::getDirectoryCount(const QString &dirPath) const
{
    QDir dir(dirPath);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    return entries.count();
}

qint64 FileSystemModel::getDirectorySize(const QString &dirPath) const
{
    return calculateDirectorySize(dirPath);
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ToolTipRole) {
        QFileInfo info = fileInfo(index);
        QString tooltip = QString("Name: %1\nSize: %2\nModified: %3")
                              .arg(info.fileName())
                              .arg(info.size())
                              .arg(info.lastModified().toString());
        return tooltip;
    }

    return QFileSystemModel::data(index, role);
}

qint64 FileSystemModel::calculateDirectorySize(const QString &dirPath) const
{
    qint64 size = 0;
    QDirIterator it(dirPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        size += it.fileInfo().size();
    }

    return size;
}
