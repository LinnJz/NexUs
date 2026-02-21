#ifndef T_ABOUT_H
#define T_ABOUT_H

#include <NXDialog.h>

class T_About : public NXDialog
{
  Q_OBJECT
public:
  explicit T_About(QWidget *parent = nullptr);
  ~T_About();
};

#endif // T_ABOUT_H
