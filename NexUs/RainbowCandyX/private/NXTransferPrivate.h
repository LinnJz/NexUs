#ifndef NXTRANSFERPRIVATE_H
#define NXTRANSFERPRIVATE_H

#include <QObject>
#include <QStringList>

#include "NXDef.h"
class QHBoxLayout;
class QVBoxLayout;
class NXCheckBox;
class NXLineEdit;
class NXIconButton;
class NXText;
class NXBaseListView;
class QStandardItemModel;
class NXTransfer;
class NXTransferPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTransfer)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(QString, SourceTitle)
    Q_PROPERTY_CREATE_D(QString, TargetTitle)
    Q_PROPERTY_CREATE_D(bool, IsSearchVisible)
public:
    explicit NXTransferPrivate(QObject* parent = nullptr);
    ~NXTransferPrivate() override;

    Q_SLOT void onMoveToTarget();
    Q_SLOT void onMoveToSource();
    Q_SLOT void onSourceSearchTextChanged(const QString& text);
    Q_SLOT void onTargetSearchTextChanged(const QString& text);
    Q_SLOT void onSourceSelectAllChanged(int state);
    Q_SLOT void onTargetSelectAllChanged(int state);

    void _refreshCountText();

private:
    NXThemeType::ThemeMode _themeMode;

    NXCheckBox* _sourceSelectAll{nullptr};
    NXText* _sourceTitleText{nullptr};
    NXText* _sourceCountText{nullptr};
    NXLineEdit* _sourceSearchEdit{nullptr};
    NXBaseListView* _sourceListView{nullptr};
    QStandardItemModel* _sourceModel{nullptr};

    NXCheckBox* _targetSelectAll{nullptr};
    NXText* _targetTitleText{nullptr};
    NXText* _targetCountText{nullptr};
    NXLineEdit* _targetSearchEdit{nullptr};
    NXBaseListView* _targetListView{nullptr};
    QStandardItemModel* _targetModel{nullptr};

    NXIconButton* _moveToTargetButton{nullptr};
    NXIconButton* _moveToSourceButton{nullptr};

    friend class NXTransfer;
};

#endif // NXTRANSFERPRIVATE_H
