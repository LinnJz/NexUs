## 二

帮我添加一个开关限制，开关不打卡则对于写了声明为 override的函数，我们不进行修改，.cpp等实现文件也不能修改，意味着我们需要一个全局set记录函数完整签名，开关打开就是当前py 的内容
 `\optimize_qt_params.py` `\test_optimize_params.h` ，默认开关不打开。注意的是函数签名可能被换行也可能是单独一行，需要处理这种情况，
例如，开关不打卡
.h等header文件
  QSize
  sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;
不替换
.cpp等实现文件
QSize
ElaNavigationStyle::sizeFromContents(ContentsType type,
  const QStyleOption* option,
  const QSize& size,
  const QWidget* widget) const
{
不替换
示例：
开关打开
h等header文件
  QSize
  sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;
替换
  QSize
  sizeFromContents(ContentsType type, const QStyleOption *option, QSize size, const QWidget *widget) const override;
.cpp等实现文件
QSize
ElaNavigationStyle::sizeFromContents(ContentsType type,
  const QStyleOption* option,
  const QSize& size,
  const QWidget* widget) const
{
替换
QSize
ElaNavigationStyle::sizeFromContents(ContentsType type,
  const QStyleOption* option,
  QSize size,
  const QWidget* widget) const
{
示例：不是override函数
h等header文件
  QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
替换
QSize  sizeFromContents(ContentsType type, const QStyleOption *option, QSize size, const QWidget *widget) const;
进行tdd测试