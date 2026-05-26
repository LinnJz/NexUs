#ifndef TEST_OVERRIDE_PARAMS_H
#define TEST_OVERRIDE_PARAMS_H

#include <QFont>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QString>
#include <QUrl>
#include <QVariant>

class BaseStyle
{
public:
  virtual ~BaseStyle() = default;
  virtual void
  drawPrimitive(int element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
  virtual QSize sizeFromContents(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
  virtual void
  drawControl(int element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};

class TestOverrideStyle : public BaseStyle
{
public:
  // Single-line override - Phase 1 types: should NOT change when SKIP_OVERRIDE=True
  QSize sizeFromContents(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;

  // Single-line override - Phase 2 types: should NOT change when SKIP_OVERRIDE=True
  void setName(const QString &name) override;
  void setPixmap(const QPixmap &pm) override;

  // Single-line override with QRect: should NOT change when SKIP_OVERRIDE=True
  void drawRect(const QRect &r) const override;

  // Single-line override with QPointF: should NOT change when SKIP_OVERRIDE=True
  void moveTo(const QPointF &pt) override;

  // East const override: should NOT change when SKIP_OVERRIDE=True
  void eastConstOverride(QSize const &sz) override;

  // Override with mixed Phase 1 and Phase 2 types: should NOT change when SKIP_OVERRIDE=True
  void mixedOverride(const QSize &sz, const QString &name, const QPixmap &pm) override;

  // Override with QFont (Phase 2): should NOT change when SKIP_OVERRIDE=True
  void setFont(const QFont &font) override;

  // Multi-line override - Phase 1 type split across lines
  QSize multiLineSize(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;

  // Multi-line override - Phase 2 type split across lines
  void multiLineComplex(const QSize &sz, const QString &name, const QPixmap &pm) override;

  // ===== Non-override functions: should ALWAYS be changed =====

  QSize nonOverrideSize(QSize size);
  void nonOverrideName(const QString &name);
  void nonOverridePixmap(const QPixmap &pm);
  void nonOverrideRect(QRect r);
  void nonOverrideMixed(QSize sz, const QString &name, const QPixmap &pm);
};

#endif
