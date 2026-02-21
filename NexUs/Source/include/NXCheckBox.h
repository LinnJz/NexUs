#ifndef NXCHECKBOX_H
#define NXCHECKBOX_H

#include <QCheckBox>

#include "NXDef.h"
class NXCheckBoxPrivate;

class NX_EXPORT NXCheckBox : public QCheckBox
{
  Q_OBJECT
  Q_Q_CREATE(NXCheckBox)
public:
  explicit NXCheckBox(QWidget *parent = nullptr);
  explicit NXCheckBox(const QString& text, QWidget *parent = nullptr);
  ~NXCheckBox() override;
  void setTextStyle(NXTextType::TextStyle textStyle,
                    std::optional<int> pixelSize        = std::nullopt,
                    std::optional<QFont::Weight> weight = std::nullopt);
  NXTextType::TextStyle getTextStyle() const;

  void setCheckBorderRadius(int borderRadius);
  int getCheckBorderRadius() const;

  void setCheckIndicatorWidth(int indicatorWidth);
  int getCheckIndicatorWidth() const;
};

#endif // NXCHECKBOX_H
