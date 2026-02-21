#ifndef T_BASEPAGE_H
#define T_BASEPAGE_H

#include <NXScrollPage.h>

class QVBoxLayout;

class T_BasePage : public NXScrollPage
{
  Q_OBJECT
public:
  explicit T_BasePage(QWidget *parent = nullptr);
  ~T_BasePage() override;

protected:
  void createCustomWidget(QString desText);
};

#endif // T_BASEPAGE_H
