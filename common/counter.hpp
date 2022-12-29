/**
 * @file counter.hpp
 * @brief Definition of the class "Counter".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef COMMON_COUNTER_HPP_
#define COMMON_COUNTER_HPP_

#include <string>
#include <type_traits>
#include <vector>

/** @brief The namespace of the Common */
namespace common {

/** @brief Internal namespace. */
namespace _detail {

template <typename T, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<
    T, std::enable_if_t<std::is_same<decltype(std::declval<T>().size()), std::size_t>::value>>
  : std::true_type {};

template <typename T>
constexpr bool has_size_v = has_size<T>::value;

} /* _detail:: */

/** @brief Template class "Counter". */
template <class T>
class counter {
  static_assert(_detail::has_size_v<T>, "The specified type is not supported");

  T data_;
  mutable size_t count_;

public:
  counter() = delete;
  explicit counter(T&& data) : data_(data), count_(1) {}

  counter(counter&&) = default;
  counter& operator=(counter&&) = default;

  void add_count(size_t n) const {
    count_ += n;
  }

  size_t count() const {
    return count_;
  }

  size_t strlen() const {
    return data_.size();
  }

  template <class U>
  friend bool operator<(const counter<U>& lhs, const counter<U>& rhs);
  template <class U>
  friend bool operator==(const counter<U>& lhs, const counter<U>& rhs);
  template <class U>
  friend bool operator!=(const counter<U>& lhs, const counter<U>& rhs);
  template <class U>
  friend std::ostream& operator<<(std::ostream& os, const counter<U>& obj);
};

template <class T>
inline bool operator<(const counter<T>& lhs, const counter<T>& rhs) {
  return lhs.data_ < rhs.data_;
}
template <class T>
inline bool operator==(const counter<T>& lhs, const counter<T>& rhs) {
  return lhs.data_ == rhs.data_;
}
template <class T>
inline bool operator!=(const counter<T>& lhs, const counter<T>& rhs) {
  return !(lhs == rhs);
}
template <class T>
inline std::ostream& operator<<(std::ostream& os, const counter<T>& src) {
  os << src.data_;
  return os;
}

} /* common:: */

#endif /* COMMON_COUNTER_HPP_ */
