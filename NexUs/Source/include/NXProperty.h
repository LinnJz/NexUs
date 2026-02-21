#ifndef NXPROPERTY_H
#define NXPROPERTY_H
#include <QObject>
#include <QtCore/qglobal.h>
#include <QtCore/qmetaobject.h>
#include <type_traits>

#ifdef NEXUS_LIBRARY
#define NX_EXPORT Q_DECL_EXPORT
#else
#define NX_EXPORT Q_DECL_IMPORT
#endif

#if __cplusplus < 202002L
namespace std
{
  template<typename T>
  struct remove_cvref
  {
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  };

  template<typename T>
  using remove_cvref_t = typename remove_cvref<T>::type;
}
#endif

// PROPERTY: 需要和信号关联
// PRIVATE: 不需要和信号关联
// _H/_CPP: Pimpl版本
//
// 基础版本 - 支持自定义setter和getter类型
#define Q_PROPERTY_CREATE_EX(SET_TYPE, GET_TYPE, M)                                \
  Q_PROPERTY(std::remove_cvref_t<SET_TYPE> p##M MEMBER _p##M NOTIFY p##M##Changed) \
public:                                                                            \
  Q_SIGNAL void p##M##Changed();                                                   \
  void set##M(SET_TYPE M)                                                          \
  {                                                                                \
    _p##M = M;                                                                     \
    Q_EMIT p##M##Changed();                                                        \
  }                                                                                \
  GET_TYPE get##M() const { return _p##M; }                                        \
                                                                                   \
private:                                                                           \
  std::remove_cvref_t<SET_TYPE> _p##M;

#define Q_PRIVATE_CREATE_EX(SET_TYPE, GET_TYPE, M) \
public:                                            \
  void set##M(SET_TYPE M) { _p##M = M; }           \
  GET_TYPE get##M() const { return _p##M; }        \
                                                   \
private:                                           \
  std::remove_cvref_t<SET_TYPE> _p##M;

// Move版本的基础宏
#define Q_PROPERTY_MOVE_CREATE_EX(SET_TYPE, GET_TYPE, M)                                 \
  static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,                               \
                "SET_TYPE must be a plain type (no cv, no reference) for move version"); \
  static_assert(std::is_move_assignable_v<T> && std::is_move_constructible_v<T>,         \
                "SET_TYPE must be move assignable and move constructible");              \
  Q_PROPERTY(SET_TYPE p##M MEMBER _p##M NOTIFY p##M##Changed)                            \
public:                                                                                  \
  Q_SIGNAL void p##M##Changed();                                                         \
  void set##M(SET_TYPE M)                                                                \
  {                                                                                      \
    _p##M = std::move(M);                                                                \
    Q_EMIT p##M##Changed();                                                              \
  }                                                                                      \
  GET_TYPE get##M() const { return _p##M; }                                              \
                                                                                         \
private:                                                                                 \
  SET_TYPE _p##M;

#define Q_PRIVATE_MOVE_CREATE_EX(SET_TYPE, GET_TYPE, M)                                  \
  static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,                               \
                "SET_TYPE must be a plain type (no cv, no reference) for move version"); \
  static_assert(std::is_move_assignable_v<T> && std::is_move_constructible_v<T>,         \
                "SET_TYPE must be move assignable and move constructible");              \
                                                                                         \
public:                                                                                  \
  void set##M(SET_TYPE M) { _p##M = std::move(M); }                                      \
  GET_TYPE get##M() const { return _p##M; }                                              \
                                                                                         \
private:                                                                                 \
  SET_TYPE _p##M;

// 简化版本 - 保持原有接口，自动推导类型
#define Q_PROPERTY_CREATE(TYPE, M) Q_PROPERTY_CREATE_EX(TYPE, TYPE, M)

#define Q_PRIVATE_CREATE(TYPE, M) Q_PRIVATE_CREATE_EX(TYPE, TYPE, M)

#define Q_PROPERTY_MOVE_CREATE(TYPE, M) Q_PROPERTY_MOVE_CREATE_EX(TYPE, TYPE, M)

#define Q_PRIVATE_MOVE_CREATE(TYPE, M) Q_PRIVATE_MOVE_CREATE_EX(TYPE, TYPE, M)

// Pimpl版本 - 支持自定义类型
#define Q_PROPERTY_CREATE_Q_EX_H(SET_TYPE, GET_TYPE, M)                                        \
  Q_PROPERTY(std::remove_cvref_t<SET_TYPE> p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
public:                                                                                        \
  Q_SIGNAL void p##M##Changed();                                                               \
  void set##M(SET_TYPE M);                                                                     \
  GET_TYPE get##M() const;

#define Q_PRIVATE_CREATE_Q_EX_H(SET_TYPE, GET_TYPE, M) \
public:                                                \
  void set##M(SET_TYPE M);                             \
  GET_TYPE get##M() const;

#define Q_PROPERTY_CREATE_Q_EX_CPP(CLASS, SET_TYPE, GET_TYPE, M) \
  void CLASS::set##M(SET_TYPE M)                                 \
  {                                                              \
    Q_D(CLASS);                                                  \
    d->_p##M = M;                                                \
    Q_EMIT p##M##Changed();                                      \
  }                                                              \
  GET_TYPE CLASS::get##M() const { return d_ptr->_p##M; }

#define Q_PRIVATE_CREATE_Q_EX_CPP(CLASS, SET_TYPE, GET_TYPE, M) \
  void CLASS::set##M(SET_TYPE M)                                \
  {                                                             \
    Q_D(CLASS);                                                 \
    d->_p##M = M;                                               \
  }                                                             \
  GET_TYPE CLASS::get##M() const { return d_ptr->_p##M; }

// Pimpl版本 - Move支持
#define Q_PROPERTY_MOVE_CREATE_Q_EX_H(SET_TYPE, GET_TYPE, M)                             \
  static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,                               \
                "SET_TYPE must be a plain type (no cv, no reference) for move version"); \
  static_assert(std::is_move_assignable_v<T> && std::is_move_constructible_v<T>,         \
                "SET_TYPE must be move assignable and move constructible");              \
  Q_PROPERTY(SET_TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed)                \
public:                                                                                  \
  Q_SIGNAL void p##M##Changed();                                                         \
  void set##M(SET_TYPE M);                                                               \
  GET_TYPE get##M() const;

#define Q_PRIVATE_MOVE_CREATE_Q_EX_H(SET_TYPE, GET_TYPE, M)                              \
  static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,                               \
                "SET_TYPE must be a plain type (no cv, no reference) for move version"); \
  static_assert(std::is_move_assignable_v<T> && std::is_move_constructible_v<T>,         \
                "SET_TYPE must be move assignable and move constructible");              \
                                                                                         \
public:                                                                                  \
  void set##M(SET_TYPE M);                                                               \
  GET_TYPE get##M() const;

#define Q_PROPERTY_MOVE_CREATE_Q_EX_CPP(CLASS, SET_TYPE, GET_TYPE, M) \
  void CLASS::set##M(SET_TYPE M)                                      \
  {                                                                   \
    Q_D(CLASS);                                                       \
    d->_p##M = std::move(M);                                          \
    Q_EMIT p##M##Changed();                                           \
  }                                                                   \
  GET_TYPE CLASS::get##M() const { return d_ptr->_p##M; }

#define Q_PRIVATE_MOVE_CREATE_Q_EX_CPP(CLASS, SET_TYPE, GET_TYPE, M) \
  void CLASS::set##M(SET_TYPE M)                                     \
  {                                                                  \
    Q_D(CLASS);                                                      \
    d->_p##M = std::move(M);                                         \
  }                                                                  \
  GET_TYPE CLASS::get##M() const { return d_ptr->_p##M; }

// Pimpl简化版本
#define Q_PROPERTY_CREATE_Q_H(TYPE, M) Q_PROPERTY_CREATE_Q_EX_H(TYPE, TYPE, M)

#define Q_PRIVATE_CREATE_Q_H(TYPE, M) Q_PRIVATE_CREATE_Q_EX_H(TYPE, TYPE, M)

#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M) Q_PROPERTY_CREATE_Q_EX_CPP(CLASS, TYPE, TYPE, M)

#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M) Q_PRIVATE_CREATE_Q_EX_CPP(CLASS, TYPE, TYPE, M)

#define Q_PROPERTY_MOVE_CREATE_Q_H(TYPE, M) Q_PROPERTY_MOVE_CREATE_Q_EX_H(TYPE, TYPE, M)

#define Q_PRIVATE_MOVE_CREATE_Q_H(TYPE, M) Q_PRIVATE_MOVE_CREATE_Q_EX_H(TYPE, TYPE, M)

#define Q_PROPERTY_MOVE_CREATE_Q_CPP(CLASS, TYPE, M) Q_PROPERTY_MOVE_CREATE_Q_EX_CPP(CLASS, TYPE, TYPE, M)

#define Q_PRIVATE_MOVE_CREATE_Q_CPP(CLASS, TYPE, M) Q_PRIVATE_MOVE_CREATE_Q_EX_CPP(CLASS, TYPE, TYPE, M)

#define Q_PROPERTY_CREATE_D(TYPE, M) \
private:                             \
  TYPE _p##M;

#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
  TYPE _p##M;

#define Q_PROPERTY_MOVE_CREATE_D(TYPE, M) \
private:                                  \
  TYPE _p##M;

#define Q_Q_CREATE(CLASS)                                      \
protected:                                                     \
  explicit CLASS(CLASS##Private& dd, CLASS *parent = nullptr); \
  QScopedPointer<CLASS##Private> d_ptr;                        \
                                                               \
private:                                                       \
  Q_DISABLE_COPY(CLASS)                                        \
  Q_DECLARE_PRIVATE(CLASS)

#define Q_D_CREATE(CLASS)  \
protected:                 \
  CLASS *q_ptr;            \
                           \
private:                   \
  Q_DECLARE_PUBLIC(CLASS);

#define Q_BEGIN_ENUM_CREATE_IMPL(_1, _2, NAME, ...) NAME
#define Q_BEGIN_ENUM_CREATE_ARGS(args)              Q_BEGIN_ENUM_CREATE_IMPL args
#define Q_BEGIN_ENUM_CREATE(...)                                                                     \
  Q_BEGIN_ENUM_CREATE_ARGS((__VA_ARGS__, Q_BEGIN_ENUM_CREATE_2, Q_BEGIN_ENUM_CREATE_1))(__VA_ARGS__)

// 枚举类导出  兼容QT5低版本
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE_1(CLASS) \
  namespace CLASS                    \
  {                                  \
    Q_NAMESPACE_EXPORT()

#define Q_BEGIN_ENUM_CREATE_2(CLASS, EXPORT) \
  namespace CLASS                            \
  {                                          \
    Q_NAMESPACE_EXPORT(EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE_1(CLASS) \
  class CLASS : public QObject       \
  {                                  \
    Q_OBJECT                         \
  public:

#define Q_BEGIN_ENUM_CREATE_2(CLASS, EXPORT) \
  class EXPORT CLASS : public QObject        \
  {                                          \
    Q_OBJECT                                 \
  public:

#define Q_END_ENUM_CREATE(CLASS) \
private:                         \
  Q_DISABLE_COPY(CLASS)          \
  }                              \
  ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

template<typename EnumT>
QString qEnumName()
{
  using Enum = std::decay_t<EnumT>;
  static_assert(std::is_enum_v<Enum>, "qEnumName requires an enum type");

  QMetaEnum meta = QMetaEnum::fromType<Enum>();
  if (!meta.isValid()) { return {}; }
  return QString::fromUtf8(meta.name());
}

template<typename EnumT>
QString qEnumName(EnumT value)
{
  using Enum = std::decay_t<EnumT>;
  static_assert(std::is_enum_v<Enum>, "qEnumName requires an enum type");


  const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();
  if (!metaEnum.isValid()) { return {}; }

  using Underlying   = std::underlying_type_t<Enum>;
  const int intValue = static_cast<int>(static_cast<Underlying>(value));
  const char *key    = metaEnum.valueToKey(intValue);
  if (!key) { return QString::number(intValue); }
  return QString::fromUtf8(key);
}

#endif // NXPROPERTY_H
