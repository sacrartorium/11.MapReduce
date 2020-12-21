#pragma once

#include "task.hpp"

/**
    Класс, выполняющий свёртку.
 */
struct Reduce {
  using ReduceFn = std::function<void(ContainerPtr &data,
                                      std::shared_ptr<std::ofstream> stream)>;

  Reduce(Container data, size_t countReducers);
  void execute(ReduceFn reducer);

private:
  Container mData;
  size_t mCountReduceThreads;
};
