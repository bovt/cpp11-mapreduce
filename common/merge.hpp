/**
 * @file merge.hpp
 * @brief Implementation of various merge functions.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef COMMON_MERGE_HPP_
#define COMMON_MERGE_HPP_

#include <queue>

#include "extractor.hpp"

/** @brief The namespace of the Common */
namespace common {

/**
 * @brief Combine sorted vectors into one big one.
 *
 * @param [in] data - input data.
 * @return Vector.
 */
template <class T>
std::vector<T> merge(std::vector<std::vector<T>>&& data) {
  using pqueue_t = std::priority_queue<common::extractor<T>, std::vector<common::extractor<T>>,
                                       std::greater<common::extractor<T>>>;

  pqueue_t pq;
  std::size_t count = 0;
  std::for_each(data.begin(), data.end(), [&pq, &count](std::vector<T>& vec) {
    count += vec.size();
    pq.emplace(std::move(vec));
  });

  std::vector<T> res;
  res.reserve(count);

  while (!pq.empty()) {
    common::extractor<T> elem = std::move(const_cast<common::extractor<T>&>(pq.top()));
    pq.pop();

    res.push_back(std::move(elem.extract()));
    if (elem.has_next()) {
      pq.push(std::move(elem));
    }
  }

  return res;
}

} /* common:: */

#endif /* COMMON_MERGE_HPP_ */
