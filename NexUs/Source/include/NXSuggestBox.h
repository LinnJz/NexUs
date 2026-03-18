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
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(Qt::CaseSensitivity, CaseSensitivity)

public:
  explicit NXSuggestBox(QWidget *parent = nullptr);
  ~NXSuggestBox() override;
  void setPlaceholderText(const QString& placeholderText) noexcept;
  void setFixedSize(const QSize& size) noexcept;
  void setFixedSize(int w, int h) noexcept;
  void setFixedHeight(int h) noexcept;

  struct NX_EXPORT SuggestData
  {
    Q_PRIVATE_CREATE(NXIconType::IconName, NXIcon)
    Q_PRIVATE_CREATE_2(const QString&, QString, SuggestText)
    Q_PRIVATE_CREATE_2(const QString&, QString, SuggestKey)
    Q_PRIVATE_CREATE_2(const QVariantMap&, QVariantMap, SuggestData)

  public:
    explicit SuggestData();
    explicit SuggestData(NXIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    ~SuggestData();
  };

  QString addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {}) noexcept;
  QString
  addSuggestion(NXIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {}) noexcept;
  QString addSuggestion(const NXSuggestBox::SuggestData& suggestData) noexcept;
  QStringList addSuggestion(const QList<NXSuggestBox::SuggestData>& suggestDataList) noexcept;

  void removeSuggestion(const QString& suggestKey) noexcept;
  void removeSuggestion(int index) noexcept;
  void clearSuggestion() noexcept;

Q_SIGNALS:
  void suggestionClicked(const NXSuggestBox::SuggestData& suggestData);
};

#endif // NXSUGGESTBOX_H
