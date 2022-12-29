/**
 * @file extractor.hpp
 * @brief Definition of the class "Extractor".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef COMMON_EXTRACTOR_HPP_
#define COMMON_EXTRACTOR_HPP_

#include <string>
#include <vector>

/** @brief The namespace of the Common */
namespace common {

/** @brief Template class "Extractor". */
template <class T>
class extractor {
  /** @brief Data container. */
  std::vector<T> data_;
  /** @brief Pointer to element. */
  std::move_iterator<typename std::vector<T>::iterator> next_;

public:
  /**
   * @brief Constructor with param.
   * @param [in] data - data container.
   */
  explicit extractor(std::vector<T>&& data) noexcept
    : data_(std::move(data)), next_(std::make_move_iterator(data_.begin())) {}

  /** @brief Move constructor */
  extractor(extractor&& src) noexcept {
    data_.swap(src.data_);
    next_ = src.next_;
  }

  /** @brief Move operator */
  extractor& operator=(extractor&& src) noexcept {
    if (this != &src) {
      data_.swap(src.data_);
      next_ = src.next_;
    }
    return *this;
  }

  /**
   * @brief Is there any next item.
   * @return "True" - is has next item, otherwise - "False".
   */
  bool has_next() const noexcept {
    return next_.base() != data_.end();
  }

  /**
   * @brief Extract elem.
   * @return Element.
   */
  T extract() noexcept {
    T tmp = *next_;
    ++next_;
    return tmp;
  }

  /**
   * @brief Get element.
   * @return Element.
   */
  const T& val() const noexcept {
    return *(next_.base());
  }
};

template <class T>
bool operator<(const common::extractor<T>& lhs, const common::extractor<T>& rhs) {
  return lhs.val() < rhs.val();
}

template <class T>
bool operator>(const common::extractor<T>& lhs, const common::extractor<T>& rhs) {
  return rhs.val() < lhs.val();
}

template <class T>
bool operator==(const common::extractor<T>& lhs, const common::extractor<T>& rhs) {
  return lhs.val() == rhs.val();
}

template <class T>
bool operator!=(const common::extractor<T>& lhs, const common::extractor<T>& rhs) {
  return !(lhs < rhs);
}

} /* common:: */

#endif /* COMMON_EXTRACTOR_HPP_ */
