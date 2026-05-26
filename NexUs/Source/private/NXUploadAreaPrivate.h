#ifndef NXUPLOADAREAPRIVATE_H
#define NXUPLOADAREAPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXUploadArea;

class NXUploadAreaPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXUploadArea)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QString, DialogTitle)
  Q_PROPERTY_CREATE_D(QStringList, AcceptedSuffixes)
  Q_PROPERTY_CREATE_D(qint64, MaxFileSize)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, MaxFileCount)
  Q_PROPERTY_CREATE_D(bool, IsMultiple)

public:
  explicit NXUploadAreaPrivate(QObject *parent = nullptr);
  ~NXUploadAreaPrivate();

  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  friend class NXUploadArea;
  bool _isDragOver { false };
  bool _isHover { false };
  bool _isPressed { false };
  NXThemeType::ThemeMode _themeMode;
  QString _mimeFilter;
  QStringList _filePaths;
  void _openFileDialog();
  bool _validateFile(const QString &filePath, QString &reason) const;
  void _addFiles(const QStringList &filePaths);
  void _removeFile(int index);
};

#endif // NXUPLOADAREAPRIVATE_H
