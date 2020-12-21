#pragma once

#include "task.hpp"

#include <mutex>

/**
  Класс, реализующий концепцию отображения, на основе файла, разбитого на
  группы.
 */
struct Map {
  using MapFn = std::function<void(
      std::ifstream &file, std::iostream::pos_type beginSegmant,
      std::iostream::pos_type endSegmant, ContainerPtr container)>;

  Map(std::string fileName, size_t countMaps);

  VectorContainers execute(Frames frames, MapFn mapFn);

  std::mutex mMutex;

private:
  std::ifstream mFile;
  size_t mCountMapThreads;
};
