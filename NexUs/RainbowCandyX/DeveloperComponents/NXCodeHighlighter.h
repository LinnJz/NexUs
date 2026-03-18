#ifndef NXCODEHIGHLIGHTER_H
#define NXCODEHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QList>

#include "NXTheme.h"

class NXCodeHighlighter : public QSyntaxHighlighter
{
public:
  explicit NXCodeHighlighter(QTextDocument *parent, NXThemeType::ThemeMode themeMode, int language = 0);
  ~NXCodeHighlighter();
  void setThemeMode(NXThemeType::ThemeMode themeMode) noexcept;
  void setLanguage(int language) noexcept;

protected:
  void highlightBlock(const QString& text) override;

private:
  struct HighlightRule
  {
    QRegularExpression pattern;
    QTextCharFormat format;
  };

  QStringList _getKeywords() const noexcept;
  QString _getSingleLineComment() const noexcept;
  bool _hasMultiLineComment() const noexcept;
  bool _hasPreprocessor() const noexcept;
  void setupRules() noexcept;

  QList<HighlightRule> _rules;
  NXThemeType::ThemeMode _themeMode;
  int _language { 0 };
};

#endif // NXCODEHIGHLIGHTER_H
