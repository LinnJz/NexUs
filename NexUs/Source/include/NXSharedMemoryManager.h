#ifndef NX_SHARED_MEMORY_MANAGER_H
#define NX_SHARED_MEMORY_MANAGER_H
#include <QSharedMemory>
#include "NXProperty.h"

class NX_EXPORT NXSharedMemoryManager : QObject
{
  Q_OBJECT
public:
  explicit NXSharedMemoryManager(QObject *parent = nullptr);
  ~NXSharedMemoryManager();
  bool writeToSharedMemory(const QJsonDocument& doc, const QString& key);
  void releaseMemory();
  QJsonDocument readFromSharedMemory(const QString& key);
  QString sharedMemoryKey() const;

private:
  QSharedMemory m_sharedMemory;
};
#endif
