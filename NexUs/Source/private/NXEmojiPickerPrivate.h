#ifndef NXEMOJIPICKERPRIVATE_H
#define NXEMOJIPICKERPRIVATE_H

#include <QObject>
#include <QStringList>
#include <QVector>

#include "NXDef.h"

class NXEmojiPicker;
class QGridLayout;
class NXLineEdit;

class NXEmojiPickerPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXEmojiPicker)
  Q_PROPERTY_CREATE_D(int, EmojiSize)
  Q_PROPERTY_CREATE_D(int, Columns)

public:
  struct EmojiCategory
  {
    QString name;
    QString icon;
    QStringList emojis;
  };

  int _currentCategory { 0 };
  NXThemeType::ThemeMode _themeMode;
  QVector<EmojiCategory> _categories;
  QStringList _recentEmojis;

  QGridLayout *_gridLayout { nullptr };
  NXLineEdit *_searchEdit { nullptr };
  QWidget *_gridContainer { nullptr };
  explicit NXEmojiPickerPrivate(QObject *parent = nullptr);
  ~NXEmojiPickerPrivate();


  void initEmojis();
  void buildGrid(int categoryIndex);
  void filterEmojis(const QString &text);
};

#endif // NXEMOJIPICKERPRIVATE_H
