#ifndef NXSPLITTERHANDLE_H
#define NXSPLITTERHANDLE_H

#include <QSplitterHandle>

#include "NXDef.h"

class NXSplitterHandle : public QSplitterHandle
{
  Q_OBJECT

public:
  explicit NXSplitterHandle(Qt::Orientation orientation, QSplitter *parent = nullptr);
  ~NXSplitterHandle();

  void setGripLength(int length);
  int getGripLength() const;

protected:
  void paintEvent(QPaintEvent *event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  void enterEvent(QEnterEvent *event) override;
#else
  void enterEvent(QEvent *event) override;
#endif
  void leaveEvent(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
  bool _isHover { false };
  bool _isPressed { false };
  int _gripLength { 36 };
};

#endif // NXSPLITTERHANDLE_H
