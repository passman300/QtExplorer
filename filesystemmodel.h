#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QFileInfo>
#include <QDir>

class FileSystemModel : public QFileSystemModel
{
public:
    explicit FileSystemModel(QObject *parent = nullptr);

    // Enhanced functionality
    void setShowHiddenFiles(bool show);
    bool showHiddenFiles() const;

    // File operations
    bool createDirectory(const QString &path, const QString &name);
    bool removeFile(const QString &filePath);
    bool removeDirectory(const QString &dirPath);
    bool renameFile(const QString &oldPath, const QString &newName);

    // Information methods
    int getFileCount(const QString &dirPath) const;
    int getDirectoryCount(const QString &dirPath) const;
    qint64 getDirectorySize(const QString &dirPath) const;

protected:
    // Override for custom behavior
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    bool m_showHiddenFiles;
    qint64 calculateDirectorySize(const QString &dirPath) const;
};

#endif // FILESYSTEMMODEL_H
