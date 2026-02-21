#ifndef NXCHECKBOXPRIVATE_H
#define NXCHECKBOXPRIVATE_H

#include <QObject>
#include "NXDef.h"
class NXCheckBox;
class NXCheckBoxStyle;

class NXCheckBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXCheckBox)
public:
  explicit NXCheckBoxPrivate(QObject *parent = nullptr);
  ~NXCheckBoxPrivate();

private:
  NXTextType::TextStyle _textStyle { NXTextType::NoStyle };
  NXCheckBoxStyle *_checkBoxStyle { nullptr };
};

#endif // NXCHECKBOXPRIVATE_H
