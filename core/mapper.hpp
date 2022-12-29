/**
 * @file mapper.hpp
 * @brief Definition of the class "Mapper".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CORE_MAPPER_HPP_
#define CORE_MAPPER_HPP_

#include <functional>
#include <future>
#include <string>
#include <thread>
#include <vector>

/** @brief The namespace of the MAP REDUCE project */
namespace yamr {
/** @brief The namespace of the Core */
namespace core {

/** @brief Alias of the pointer on function for "Mapper" class. */
template <class DATA_TYPE, class OUT_TYPE>
using mfunc_ptr_t = std::function<std::vector<OUT_TYPE>(std::vector<DATA_TYPE>&&)>;

/** @brief The mapper class */
template <class DATA_TYPE, class OUT_TYPE>
class mapper {
  using thread_ptr_t = std::unique_ptr<std::thread, std::function<void(std::thread*)>>;

  /** @brief Map function. */
  mfunc_ptr_t<DATA_TYPE, OUT_TYPE> function_;

public:
  explicit mapper(mfunc_ptr_t<DATA_TYPE, OUT_TYPE> map_func) noexcept : function_{map_func} {}

  /**
   * @brief Function to execute.
   * @param [in] input - input data.
   * @return Processed data.
   */
  std::vector<std::vector<OUT_TYPE>> exec(std::vector<std::vector<DATA_TYPE>>&& input) noexcept {
    std::vector<std::vector<OUT_TYPE>> res;

    std::vector<std::promise<std::vector<OUT_TYPE>>> promises(input.size());
    std::vector<std::future<std::vector<OUT_TYPE>>> futures;
    for (size_t i = 0; i != promises.size(); ++i) {
      futures.push_back(promises[i].get_future());
    }

    /* worker function */
    auto worker = [this](std::promise<std::vector<OUT_TYPE>>&& promise,
                         std::vector<DATA_TYPE>&& arg) {
      std::vector<OUT_TYPE> res = function_(std::move(arg));
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

template <class OUT_TYPE>
std::vector<OUT_TYPE> mapper_func(std::vector<std::string>&& lines) {
  std::vector<OUT_TYPE> res;

  for (const std::string& s : lines) {
    for (size_t len = 1; len != s.size() + 1; ++len) {
      res.emplace_back(s.substr(0, len));
    }
  }

  return res;
}

} /* core:: */
} /* yamr:: */

#endif /* CORE_MAPPER_HPP_ */
