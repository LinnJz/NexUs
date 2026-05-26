#include "test_override_params.h"

#include <QPainter>
#include <QStyleOption>
#include <QWidget>

// ===== Non-override base implementations (should ALWAYS be changed) =====

void
BaseStyle::drawPrimitive(int element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
}

QSize
BaseStyle::sizeFromContents(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
  return QSize();
}

void
BaseStyle::drawControl(int element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
}

// ===== Override implementations: should NOT change when SKIP_OVERRIDE=True =====

QSize
TestOverrideStyle::sizeFromContents(int type,
                                    const QStyleOption *option,
                                    const QSize &size,
                                    const QWidget *widget) const
{
  return QSize();
}

void
TestOverrideStyle::setName(const QString &name)
{
}

void
TestOverrideStyle::setPixmap(const QPixmap &pm)
{
}

void
TestOverrideStyle::drawRect(const QRect &r) const
{
}

void
TestOverrideStyle::moveTo(const QPointF &pt)
{
}

void
TestOverrideStyle::eastConstOverride(QSize const &sz)
{
}

void
TestOverrideStyle::mixedOverride(const QSize &sz, const QString &name, const QPixmap &pm)
{
}

void
TestOverrideStyle::setFont(const QFont &font)
{
}

QSize
TestOverrideStyle::multiLineSize(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
  return QSize();
}

void
TestOverrideStyle::multiLineComplex(const QSize &sz, const QString &name, const QPixmap &pm)
{
}

// ===== Non-override implementations: should ALWAYS be changed =====

QSize
TestOverrideStyle::nonOverrideSize(QSize size)
{
  return QSize();
}

void
TestOverrideStyle::nonOverrideName(const QString &name)
{
}

void
TestOverrideStyle::nonOverridePixmap(const QPixmap &pm)
{
}

void
TestOverrideStyle::nonOverrideRect(QRect r)
{
}

void
TestOverrideStyle::nonOverrideMixed(QSize sz, const QString &name, const QPixmap &pm)
{
}
