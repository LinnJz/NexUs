#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "NXWindow.h"
class QToolBar;
class NXRibbonBar;
class T_Home;
class T_Icon;
class T_NXScreen;
class T_BaseComponents;
class T_Graphics;
class T_Navigation;
class T_Popup;
class T_Card;
class T_ListView;
class T_TableView;
class T_TableWidget;
class T_TreeView;
class T_Router;
class T_About;
class T_Setting;
class T_NewComponents;
class T_NewComponents2;
class T_CodeEditor;
class NXContentDialog;

class MainWindow : public NXWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void initWindow();
  void initEdgeLayout();
  void initRibbon();
  void initContent();

  void setRibbonMode(bool enabled);

  bool isRibbonMode() const { return _isRibbonMode; }

protected:
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  NXContentDialog *_closeDialog { nullptr };
  NXSuggestBox *_windowSuggestBox { nullptr };
  T_Home *_homePage { nullptr };
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  T_NXScreen *_screenPage { nullptr };
#endif
  T_Icon *_iconPage { nullptr };
  T_BaseComponents *_baseComponentsPage { nullptr };
  T_Graphics *_graphicsPage { nullptr };
  T_Navigation *_navigationPage { nullptr };
  T_Popup *_popupPage { nullptr };
  T_Card *_cardPage { nullptr };
  T_ListView *_listViewPage { nullptr };
  T_TableView *_tableViewPage { nullptr };
  T_TableWidget *_tableWidgetPage { nullptr };
  T_TreeView *_treeViewPage { nullptr };
  T_About *_aboutPage { nullptr };
  T_NewComponents *_newComponentsPage { nullptr };
  T_NewComponents2 *_newComponents2Page { nullptr };
  T_CodeEditor *_codeEditorPage { nullptr };
  T_Router *_routerPage { nullptr };
  T_Setting *_settingPage { nullptr };
  QString _dxgiKey { QStringLiteral("") };
  QString _viewKey { QStringLiteral("") };
  QString _aboutKey { QStringLiteral("") };
  QString _settingKey { QStringLiteral("") };

  QWidget *_menuBarWrapper { nullptr };
  QWidget *_ribbonTabBarWrapper { nullptr };
  QToolBar *_ribbonToolBar { nullptr };
  NXRibbonBar *_ribbonBar { nullptr };
  bool _isRibbonMode { false };
};
#endif // MAINWINDOW_H
