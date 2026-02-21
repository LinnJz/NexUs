#ifndef NXSUGGESTBOX_H
#define NXSUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "NXDef.h"

class NXSuggestBoxPrivate;

class NX_EXPORT NXSuggestBox : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXSuggestBox)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
  explicit NXSuggestBox(QWidget *parent = nullptr);
  ~NXSuggestBox() override;
  void setPlaceholderText(const QString& placeholderText);
  void setFixedSize(const QSize& size);
  void setFixedSize(int w, int h);
  void setFixedHeight(int h);

  struct NX_EXPORT SuggestData
  {
    Q_PRIVATE_CREATE(NXIconType::IconName, NXIcon)
    Q_PRIVATE_CREATE(QString, SuggestText)
    Q_PRIVATE_CREATE(QString, SuggestKey)
    Q_PRIVATE_CREATE(QVariantMap, SuggestData)
  public:
    explicit SuggestData();
    explicit SuggestData(NXIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    ~SuggestData();
  };

  QString addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
  QString addSuggestion(NXIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
  QString addSuggestion(const NXSuggestBox::SuggestData& suggestData);
  QStringList addSuggestion(const QList<NXSuggestBox::SuggestData>& suggestDataList);

  void removeSuggestion(const QString& suggestKey);
  void removeSuggestion(int index);
  void clearSuggestion();
Q_SIGNALS:
  Q_SIGNAL void suggestionClicked(const NXSuggestBox::SuggestData& suggestData);
};

#endif // NXSUGGESTBOX_H
