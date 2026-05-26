#ifndef T_NEWCOMPONENTS_H
#define T_NEWCOMPONENTS_H

#include "T_BasePage.h"

class NXInfoBadge;
class NXExpander;
class NXTeachingTip;
class NXPushButton;
class NXPasswordBox;
class NXTag;
class NXSplitButton;
class NXPersonPicture;
class NXPagination;
class NXFlyout;
class NXDivider;
class NXSkeleton;
class NXSteps;
class NXTimeline;
class NXCommandBar;
class NXRatingControl;
class NXNumberBox;
class NXWizard;
class NXNotificationCenter;
class NXMarkdownViewer;
class NXCaptcha;
class NXStatCard;
class NXDropDownButton;
class NXSelectorBar;
class NXVirtualList;
class NXSplashScreen;
class NXSheetPanel;
class NXTransfer;
class NXSpotlight;
class NXCountdown;
class NXPopconfirm;
class NXWatermark;
class NXSplitter;
class NXQRCode;
class NXFloatButton;
class NXEmojiPicker;
class NXInfoBar;
class NXAutoComplete;
class NXTreeSelect;
class NXUploadArea;
class NXCopyButton;

class T_NewComponents : public T_BasePage
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit T_NewComponents(QWidget *parent = nullptr);
  ~T_NewComponents();

private:
  // InfoBadge
  NXInfoBadge *_dotBadge { nullptr };
  NXInfoBadge *_valueBadge { nullptr };
  NXInfoBadge *_iconBadge { nullptr };
  // Expander
  NXExpander *_expander { nullptr };
  NXExpander *_expanderUp { nullptr };
  // TeachingTip
  NXTeachingTip *_teachingTip { nullptr };
  NXPushButton *_teachingTipTarget { nullptr };
  // PasswordBox
  NXPasswordBox *_passwordBox { nullptr };
  // Tag
  NXTag *_closableTag { nullptr };
  NXTag *_checkableTag { nullptr };
  // SplitButton
  NXSplitButton *_splitButton { nullptr };
  // PersonPicture
  NXPersonPicture *_personPicture { nullptr };
  NXPersonPicture *_personPictureInitials { nullptr };
  NXPersonPicture *_personPictureDefault { nullptr };
  // Pagination
  NXPagination *_pagination { nullptr };
  // Flyout
  NXFlyout *_flyout { nullptr };
  // Steps
  NXSteps *_steps { nullptr };
  // Timeline
  NXTimeline *_timeline { nullptr };
  // CommandBar
  NXCommandBar *_commandBar { nullptr };
  // RatingControl
  NXRatingControl *_ratingControl { nullptr };
  // NumberBox
  NXNumberBox *_numberBox { nullptr };
  // Wizard
  NXWizard *_wizard { nullptr };
  // NotificationCenter
  NXNotificationCenter *_notificationCenter { nullptr };
  // MarkdownViewer
  NXMarkdownViewer *_markdownViewer { nullptr };
  // Captcha
  NXCaptcha *_captcha { nullptr };
  // DropDownButton
  NXDropDownButton *_dropDownButton { nullptr };
  // SelectorBar
  NXSelectorBar *_selectorBar { nullptr };
  // VirtualList
  NXVirtualList *_virtualList { nullptr };
  // SplashScreen
  NXSplashScreen *_splashScreen { nullptr };
  // SheetPanel
  NXSheetPanel *_sheetPanel { nullptr };
  // Transfer
  NXTransfer *_transfer { nullptr };
  // Spotlight
  NXSpotlight *_spotlight { nullptr };
  NXCountdown *_countdown { nullptr };
  NXPopconfirm *_popconfirm { nullptr };
  // Watermark
  NXWatermark *_watermark { nullptr };
  // Splitter
  NXSplitter *_splitter { nullptr };
  // QRCode
  NXQRCode *_qrCode { nullptr };
  // FloatButton
  NXFloatButton *_floatButton { nullptr };
  NXEmojiPicker *_emojiPicker { nullptr };
  // InfoBar
  NXInfoBar *_infoBarInfo { nullptr };
  NXInfoBar *_infoBarSuccess { nullptr };
  NXInfoBar *_infoBarWarning { nullptr };
  NXInfoBar *_infoBarError { nullptr };
  // AutoComplete
  NXAutoComplete *_autoComplete { nullptr };
  // TreeSelect
  NXTreeSelect *_treeSelect { nullptr };
  // UploadArea
  NXUploadArea *_uploadArea { nullptr };
  // CopyButton
  NXCopyButton *_copyButton { nullptr };
};

#endif // T_NEWCOMPONENTS_H
