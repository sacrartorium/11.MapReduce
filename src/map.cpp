#include "map.hpp"

Map::Map(std::string fileName, size_t countMaps)
    : mFile(fileName), mCountMapThreads(countMaps) {}

VectorContainers Map::execute(Frames frames, MapFn mapFn) {
  if (!mFile.good()) {
    throw std::ifstream::failure("Error read file.");
  }

  std::vector<Task<ContainerPtr>> maps;

  std::iostream::pos_type begin(0), end(0);
  auto frame = frames.begin();

  // Start map procedure
  {
    for (size_t taskIndex = 0;
         (taskIndex < mCountMapThreads) && (frame != frames.end());
         taskIndex++, frame++) {
      end = *frame;

      // Start task
      Task<ContainerPtr> task;
      task.resultOutput = std::make_shared<Container>();
      task.task = std::make_shared<std::thread>(mapFn, std::ref(mFile), begin,
                                                end, task.resultOutput);
      maps.push_back(task);
      begin = end;
    }
  }

  // Wait map and sort
  {
    VectorContainers containers;
    for (auto &task : maps) {
      task.task->join();
      ContainerPtr container(task.resultOutput);
      std::sort(container->begin(), container->end());
      containers.push_back(container);
    }

    return containers;
  }
}
