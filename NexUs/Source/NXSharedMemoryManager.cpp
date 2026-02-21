#include "NXSharedMemoryManager.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QTemporaryFile>
#include "NXAESEncryption.h"

NXSharedMemoryManager::NXSharedMemoryManager(QObject *parent)
    : QObject(parent)
    , m_sharedMemory(QCryptographicHash::hash("Re11a_PDFWiz_shared_memory", QCryptographicHash::Md5).toHex())
{
}

NXSharedMemoryManager::~NXSharedMemoryManager() { }

bool NXSharedMemoryManager::writeToSharedMemory(const QJsonDocument& doc, const QString& key)
{
  QByteArray iv(16, 0);
  QRandomGenerator::global()->fillRange(reinterpret_cast<quint32 *>(iv.data()), iv.size() / sizeof(quint32));
  QByteArray hashKey  = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256);
  QByteArray rawText  = doc.toJson(QJsonDocument::Compact);
  QByteArray jsonData = NXAESEncryption::Crypt(
      NXAESEncryption::AES_256, NXAESEncryption::CBC, rawText, hashKey, iv, NXAESEncryption::PKCS7);

  QByteArray finalData;
  finalData.append(iv);
  finalData.append(jsonData);

  // 强制清理旧内存
  if (m_sharedMemory.isAttached()) { m_sharedMemory.detach(); }

  // 计算所需内存大小（数据大小 + 4字节长度头）
  const qint32 dataSize  = finalData.size();
  const qint32 totalSize = sizeof(qint32) + dataSize;
  if (!m_sharedMemory.create(totalSize))
  {
    if (m_sharedMemory.error() == QSharedMemory::AlreadyExists)
    {
      m_sharedMemory.attach();
      m_sharedMemory.detach();
      if (!m_sharedMemory.create(totalSize)) { return false; }
    }
    else
    {
      return false;
    }
    qWarning() << "Create failed:" << m_sharedMemory.errorString();
  }
  m_sharedMemory.lock();
  try
  {
    char *dest = static_cast<char *>(m_sharedMemory.data());
    // 写入长度头
    qToLittleEndian<qint32>(dataSize, dest);
    // 写入实际数据
    memcpy(dest + sizeof(qint32), finalData.constData(), dataSize);
  }
  catch (...)
  {
    m_sharedMemory.unlock();
    qWarning() << "Write faied:" << m_sharedMemory.errorString();
    return false;
  }
  m_sharedMemory.unlock();
  return true;
}

void NXSharedMemoryManager::releaseMemory()
{
  if (m_sharedMemory.isAttached()) { m_sharedMemory.detach(); }
}

QJsonDocument NXSharedMemoryManager::readFromSharedMemory(const QString& key)
{
  if (!m_sharedMemory.isAttached())
  {
    if (!m_sharedMemory.attach())
    {
      qWarning() << "Attach failed:" << m_sharedMemory.errorString() << "Size" << m_sharedMemory.size();
    }
    return QJsonDocument();
  }
  QByteArray encryptedData;
  m_sharedMemory.lock();
  {
    const char *src       = static_cast<const char *>(m_sharedMemory.data());
    const qint32 dataSize = qFromLittleEndian<qint32>(src);
    src += sizeof(qint32);
    encryptedData = QByteArray(src, dataSize); // 包含 IV + 密文
  }
  m_sharedMemory.unlock();
  m_sharedMemory.detach();

  if (encryptedData.size() < 16)
  {
    return QJsonDocument(); // 数据不完整
  }

  // 分离 IV 和密文
  QByteArray iv         = encryptedData.left(16);
  QByteArray cipherText = encryptedData.mid(16);

  QByteArray hashKey       = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256);
  QByteArray decryptedData = NXAESEncryption::Decrypt(
      NXAESEncryption::AES_256, NXAESEncryption::CBC, cipherText, hashKey, iv, NXAESEncryption::PKCS7);
  return QJsonDocument::fromJson(NXAESEncryption::RemovePadding(decryptedData, NXAESEncryption::PKCS7));
}

QString NXSharedMemoryManager::sharedMemoryKey() const { return m_sharedMemory.nativeKey(); }
