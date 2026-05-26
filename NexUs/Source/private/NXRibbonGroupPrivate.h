#ifndef NXRIBBONGROUPPRIVATE_H
#define NXRIBBONGROUPPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"

class QHBoxLayout;
class NXRibbonGroup;

class NXRibbonGroupPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXRibbonGroup)

public:
  explicit NXRibbonGroupPrivate(QObject *parent = nullptr);
  ~NXRibbonGroupPrivate();

private:
  friend class NXRibbonGroup;
  QString _title;
  QHBoxLayout *_contentLayout { nullptr };
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXRIBBONGROUPPRIVATE_H
