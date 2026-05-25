#ifndef MULTI_LINE_H
#define MULTI_LINE_H

class MultiLineTest
{
public:
  virtual void longFunctionName(int param1, const QString &param2) override;

  virtual int anotherVirtual(QWidget *w) const = 0;

  virtual void deeplyIndented(const QString &text);

  virtual const QMap<QString, QVariant> &getDataMap() const;
};

#endif
