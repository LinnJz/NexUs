#ifndef NXWIZARD_H
#define NXWIZARD_H

#include <QWidget>

#include "NXProperty.h"

class NXWizardPrivate;
class NX_EXPORT NXWizard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXWizard)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, CurrentStep)

public:
    explicit NXWizard(QWidget* parent = nullptr);
    ~NXWizard() override;

    void addStep(const QString& title, QWidget *page) noexcept;
    void next() noexcept;
    void previous() noexcept;
    void finish() noexcept;
    int getStepCount() const noexcept;

Q_SIGNALS:
  void currentStepChanged(int step);
  void finished();
  void cancelled();

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
};

#endif // NXWIZARD_H
