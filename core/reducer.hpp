/**
 * @file reducer.hpp
 * @brief Definition of the class "Reducer".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CORE_REDUCER_HPP_
#define CORE_REDUCER_HPP_

#include <algorithm>
#include <functional>
#include <future>
#include <set>
#include <thread>
#include <vector>

/** @brief The namespace of the MAP REDUCE project */
namespace yamr {
/** @brief The namespace of the Core */
namespace core {

/** @brief Alias of the pointer on function for "Reducer" class. */
template <class DATA_TYPE, class OUT_TYPE>
using rfunc_ptr_t = std::function<OUT_TYPE(std::vector<DATA_TYPE>&&)>;

/** @brief The reducer class */
template <class DATA_TYPE, class OUT_TYPE>
class reducer {
  using thread_ptr_t = std::unique_ptr<std::thread, std::function<void(std::thread*)>>;

  /** @brief Reduce function. */
  rfunc_ptr_t<DATA_TYPE, OUT_TYPE> function_;

public:
  explicit reducer(rfunc_ptr_t<DATA_TYPE, OUT_TYPE> reduce_func) noexcept
    : function_{reduce_func} {}

  /**
   * @brief Function to execute.
   * @param [in] input - input data.
   * @return Processed data.
   */
  std::vector<OUT_TYPE> exec(std::vector<std::vector<DATA_TYPE>>&& input) noexcept {
    std::vector<OUT_TYPE> res;

    std::vector<std::promise<OUT_TYPE>> promises(input.size());
    std::vector<std::future<OUT_TYPE>> futures;
    for (size_t i = 0; i != promises.size(); ++i) {
      futures.push_back(promises[i].get_future());
    }

    /* worker function */
    auto worker = [this](std::promise<OUT_TYPE>&& promise, std::vector<DATA_TYPE>&& arg) {
      OUT_TYPE res = function_(std::move(arg));
      promise.set_value(std::move(res));
    };

    std::vector<thread_ptr_t> workers;
    for (size_t i = 0; i != promises.size(); ++i) {
      workers.emplace_back(
          thread_ptr_t(new std::thread(worker, std::move(promises[i]), std::move(input[i])),
                       [](std::thread* t) { t->join(); }));
    }

    for (size_t i = 0; i != futures.size(); ++i) {
      res.push_back(futures[i].get());
    }

    return res;
  }
};

/** @brief The namespace to hide the implementation. */
namespace _details {

/**
 * @brief The counting unique prefixes.
 * @tparam DATA_TYPE - Data type used.
 */
template <class DATA_TYPE>
std::set<DATA_TYPE> counting_prefixes(std::vector<DATA_TYPE>&& prefixes) {
  std::set<DATA_TYPE> count_prefixes;

  auto prefixes_it = prefixes.begin();

  while (prefixes_it != prefixes.end()) {
    auto it = count_prefixes.find(*prefixes_it);
    if (it != count_prefixes.end()) {
      it->add_count(prefixes_it->count());
      ++prefixes_it;
    }
    else {
      auto next_it = std::next(prefixes_it);
      count_prefixes.insert(std::move(*prefixes_it));
      prefixes_it = next_it;
    }
  }
  return count_prefixes;
}

/**
 * @brief Sorting data descending by length.
 * @tparam DATA_TYPE - Data type used.
 */
template <class DATA_TYPE>
std::vector<DATA_TYPE> sort_desc_by_len(std::set<DATA_TYPE>&& data) {
  std::vector<DATA_TYPE> res;
  res.reserve(data.size());

  /* move all to vector */
  auto it = data.begin();
  while (it != data.end()) {
    auto next_it = std::next(it);
    res.push_back(std::move(data.extract(it).value()));
    it = next_it;
  }
  std::sort(res.begin(), res.end(),
            [](const DATA_TYPE& lhs, const DATA_TYPE& rhs) { return lhs.strlen() > rhs.strlen(); });

  return res;
}

} /* _details:: */

/**
 * @brief The reducer of function.
 * @tparam DATA_TYPE - Data type used.
 */
template <class DATA_TYPE>
DATA_TYPE reducer_func(std::vector<DATA_TYPE>&& prefixes) {
  std::set<DATA_TYPE> count_prefixes = _details::counting_prefixes(std::move(prefixes));
  std::vector<DATA_TYPE> desc_by_len = _details::sort_desc_by_len(std::move(count_prefixes));

  auto longest_not_uniq = std::find_if(desc_by_len.begin(), desc_by_len.end(),
                                       [](const DATA_TYPE& obj) { return obj.count() > 1; });

  if (longest_not_uniq == desc_by_len.end() || longest_not_uniq->count() == 1) {
    return std::move(desc_by_len[0]);
  }
  else {
    return std::move(std::move(*longest_not_uniq));
  }
}

} /* core:: */
} /* yamr:: */

#endif /* CORE_REDUCER_HPP_ */
