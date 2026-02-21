#ifndef NXRADIOBUTTON_H
#define NXRADIOBUTTON_H

#include <QRadioButton>

#include "NXDef.h"
class NXRadioButtonPrivate;

class NX_EXPORT NXRadioButton : public QRadioButton
{
  Q_OBJECT
  Q_Q_CREATE(NXRadioButton)
public:
  explicit NXRadioButton(QWidget *parent = nullptr);
  explicit NXRadioButton(const QString& text, QWidget *parent = nullptr);
  ~NXRadioButton() override;

  void setTextStyle(NXTextType::TextStyle textStyle,
                    std::optional<int> pixelSize        = std::nullopt,
                    std::optional<QFont::Weight> weight = std::nullopt);
  NXTextType::TextStyle getTextStyle() const;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXRADIOBUTTON_H
