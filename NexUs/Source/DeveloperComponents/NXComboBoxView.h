#ifndef NXCOMBOBOXVIEW_H
#define NXCOMBOBOXVIEW_H

#include <QListView>

class NXComboBoxView : public QListView
{
  Q_OBJECT
public:
  explicit NXComboBoxView(QWidget *parent = nullptr);
  ~NXComboBoxView();
Q_SIGNALS:
  Q_SIGNAL void itemPressed(const QModelIndex& index);

protected:
  void mousePressEvent(QMouseEvent *event) override;
};

#endif // NXCOMBOBOXVIEW_H
