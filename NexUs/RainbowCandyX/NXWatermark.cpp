#include "NXWatermark.h"

#include <QEvent>
#include <QPainter>
#include <QtMath>

#include "NXTheme.h"
#include "private/NXWatermarkPrivate.h"

Q_PROPERTY_CREATE_CPP(NXWatermark, int, FontPixelSize)
Q_PROPERTY_CREATE_CPP(NXWatermark, qreal, Opacity)
Q_PROPERTY_CREATE_CPP(NXWatermark, qreal, Rotation)
Q_PROPERTY_CREATE_CPP(NXWatermark, int, GapX)
Q_PROPERTY_CREATE_CPP(NXWatermark, int, GapY)
Q_PROPERTY_CREATE_CPP(NXWatermark, int, ImageWidth)
Q_PROPERTY_CREATE_CPP(NXWatermark, int, ImageHeight)
Q_PROPERTY_CREATE_2_CPP(NXWatermark, const QColor&, QColor, TextColor)
Q_PROPERTY_CREATE_2_CPP(NXWatermark, const QImage&, QImage, Image)
Q_PROPERTY_CREATE_2_CPP(NXWatermark, const QString&, QString, Text)

NXWatermark::NXWatermark(QWidget *parent)
	: QWidget{parent}, d_ptr(new NXWatermarkPrivate())
{
	Q_D(NXWatermark);
	d->q_ptr = this;
	d->_pFontPixelSize = 14;
	d->_pOpacity = 0.12;
	d->_pRotation = -22;
	d->_pGapX = 80;
  d->_pGapY          = 60;
  d->_pText          = {};
	d->_pTextColor = QColor();
	d->_pImage = QImage();
	d->_pImageWidth = 0;
	d->_pImageHeight = 0;
	setObjectName("NXWatermark");

	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
	setFocusPolicy(Qt::NoFocus);

	if (parent)
	{
		parent->installEventFilter(this);
		resize(parent->size());
		raise();
	}

	d->_themeMode = nxTheme->getThemeMode();
	connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		d->_patternDirty = true;
		update();
	});

	auto markDirty = [=]()
	{
		d->_patternDirty = true;
		update();
	};
	connect(this, &NXWatermark::pTextChanged, this, markDirty);
	connect(this, &NXWatermark::pFontPixelSizeChanged, this, markDirty);
	connect(this, &NXWatermark::pOpacityChanged, this, markDirty);
	connect(this, &NXWatermark::pRotationChanged, this, markDirty);
	connect(this, &NXWatermark::pGapXChanged, this, markDirty);
	connect(this, &NXWatermark::pGapYChanged, this, markDirty);
	connect(this, &NXWatermark::pTextColorChanged, this, markDirty);
	connect(this, &NXWatermark::pImageChanged, this, markDirty);
	connect(this, &NXWatermark::pImageWidthChanged, this, markDirty);
	connect(this, &NXWatermark::pImageHeightChanged, this, markDirty);
}

NXWatermark::NXWatermark(const QString &text, QWidget *parent)
	: NXWatermark(parent)
{
	Q_D(NXWatermark);
	d->_pText = text;
	d->_patternDirty = true;
}

NXWatermark::~NXWatermark()
{
}

bool NXWatermark::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == parent() && event->type() == QEvent::Resize)
	{
		resize(static_cast<QWidget *>(parent())->size());
		raise();
	}
	return QWidget::eventFilter(watched, event);
}

void NXWatermark::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(NXWatermark);

	if (d->_patternDirty)
	{
		d->rebuildPattern();
	}

	if (d->_patternPixmap.isNull())
	{
		return;
	}

	QPainter painter(this);
	painter.drawTiledPixmap(rect(), d->_patternPixmap);
}