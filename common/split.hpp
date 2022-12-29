/**
 * @file split.hpp
 * @brief Implementation of various split functions.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef COMMON_SPLIT_HPP_
#define COMMON_SPLIT_HPP_

#include <regex>
#include <string>
#include <vector>

/** @brief The namespace of the Common */
namespace common {

/**
 * @brief The function slices a string into substrings.
 *
 * @details
 * This function slices a given string into substrings and writes them to a
 * string vector. The line feed character ("<space>") is used as a separator.
 *
 * @note If the delimiter is at the beginning of the line, then the first
 * substring will be empty. That is, the line is empty before the separator.
 *
 * @param [in] str - input of data string.
 * @return Vector of substrings.
 */
inline std::vector<std::string> split(const std::string& str) {
  std::regex ws_re("\\s+");
  return std::vector<std::string>{std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
                                  std::sregex_token_iterator()};
}

/**
 * @brief Cutting the original data into a specified number of parts.
 *
 * @details
 * The function slices the input data vector into the specified number of data
 * vectors.
 *
 * @param [in] input - input data vector.
 * @param [in] parts - a given number of parts.
 * @return Vector of sliced data vectors.
 */
template <class T>
std::vector<std::vector<T>> split(std::vector<T>&& input, std::size_t parts) {
  std::vector<std::vector<T>> res;

  std::size_t size = input.size();
  std::size_t idx = 0;
  for (std::size_t i = 0; i < size; ++i) {
    if (res.size() < idx + 1)
      res.push_back(std::vector<T>{});

    res[idx].push_back(std::move(input[i]));
    idx = (idx + 1) % parts;
  }

  return res;
}

/**
 * @brief Split long vector to reduce threads.
 * @param [in] vec - is a long vector of sorted elements.
 * @param [in] parts -  a given number of parts.
 * @return Vector of vectors, contain all unique elements.
 */
template <class T>
std::vector<std::vector<T>> split_reduce(std::vector<T>&& vec, std::size_t parts) {
  std::vector<std::vector<T>> res;

  std::size_t num_cluster = (vec.size() / parts) + (vec.size() % parts ? 1 : 0);

  auto it = vec.begin();
  auto it_prev = it;

  while (it_prev != vec.end()) {
    res.emplace_back();
    std::vector<T>& cluster = res.back();

    /* fill this cluster */
    std::size_t inserted_count{0};
    while (inserted_count < num_cluster) {
      if (it == vec.end() || *it != *it_prev) {
        std::copy(std::make_move_iterator(it_prev), std::make_move_iterator(it),
                  std::back_inserter(cluster));
        std::size_t moved_count = std::distance(it_prev, it);
        inserted_count += moved_count;

        it_prev = it;
        if (it_prev == vec.end())
          break;

        ++it;
      }
      else
        ++it;
    }

    if (it_prev == vec.end())
      break;
  }

  return res;
}

} /* common:: */

#endif /* COMMON_SPLIT_HPP_ */
