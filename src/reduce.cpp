#include "reduce.hpp"

#include <algorithm>

Reduce::Reduce(Container data, size_t countReducers)
    : mData(data), mCountReduceThreads(countReducers) {}

void Reduce::execute(ReduceFn reduceFn) {
  VectorContainers containers(mCountReduceThreads);
  std::generate(containers.begin(), containers.end(),
                []() { return std::make_shared<Container>(); });

  // Generate keys
  {
    for (auto &part : mData) {
      std::hash<std::string> hash;
      uint8_t key = (part.empty()) ? 0 : (hash(part) % mCountReduceThreads);

      containers[key]->push_back(part);
    }
  }

  // Start reduce procedure
  {
    std::vector<Task<File>> reducers;

    for (size_t taskIndex = 0; taskIndex < mCountReduceThreads; taskIndex++) {
      std::string fileName("result_" + std::to_string(taskIndex) + ".txt");

      // Start task
      Task<File> task;
      task.resultOutput = std::make_shared<std::ofstream>(fileName);
      task.task = std::make_shared<std::thread>(
          reduceFn, std::ref(containers[taskIndex]), task.resultOutput);
      reducers.push_back(task);
    }

    // Wait reduce
    {
      for (auto &reducer : reducers)
        reducer.task->join();

      reducers.clear();
    }
  }
}
