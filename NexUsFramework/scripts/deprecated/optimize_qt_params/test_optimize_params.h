#ifndef TEST_PARAMS_H
#define TEST_PARAMS_H

#include <QFlags>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QUrl>
#include <QVariant>

class TestParams
{
public:
  // ===== Phase 1: Remove const & from small types =====

  void takePoint(QPoint pt);
  void takePointF(QPointF pt);
  void takeRect(QRect r);
  void takeSize(QSize sz);
  void takeDate(QDate d);
  void takeTime(QTime t);
  void takeChar(QChar c);
  void takeUuid(QUuid id);
  void takeFlags(QFlags<Qt::Alignment> flags);

  // const& without space
  void takePoint2(QPoint pt);

  // Type const & (east const)
  void takePoint3(QPointF pt);

  // with default value
  void takeRect2(QRect r = QRect());

  // pointer - should NOT change
  void takePointPtr(const QPoint *pt);
  void takePointPtr2(QPoint *pt);

  // non-const reference - should NOT change
  void takePointRef(QPoint &pt);

  // ===== Phase 2: Add const & to COW types =====

  // Pass by value - should add const &
  void setName(const QString &name);
  void setData(const QByteArray &data);
  void setList(const QList<int> &items);
  void setMap(const QMap<QString, int> &mapping);
  void setImage(const QImage &img);
  void setPixmap(const QPixmap &pm);
  void setVariant(const QVariant &val);
  void setUrl(const QUrl &url);

  // Already const & - should NOT change
  void setName2(const QString &name);
  void setData2(const QByteArray &data);

  // Non-const reference - should NOT change
  void setName3(QString &name);

  // Pointer - should NOT change
  void setNamePtr(QString *name);
  void setNameConstPtr(const QString *name);

  // East const reference - should NOT change
  void setName4(QString const &name);

  // With default value
  void setName5(const QString &name = QString());
  void setFont(const QFont &font = QFont());

  // Multiple params mixing both phases
  void complexFunc(QPoint pt, const QString &text, int value);
  void complexFunc2(QSize sz, const QString &name, const QList<int> &items);

  // No-param function - should NOT change
  void emptyFunc();

  // Non-Qt types - should NOT change
  void takeInt(int x);
  void takeString(const std::string &s);
};

#endif
