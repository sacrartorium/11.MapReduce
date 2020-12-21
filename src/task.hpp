#pragma once

#include <fstream>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

using Container = std::vector<std::string>;
using ContainerPtr = std::shared_ptr<Container>;
using VectorContainers = std::vector<ContainerPtr>;
using File = std::shared_ptr<std::ofstream>;
using Thread = std::shared_ptr<std::thread>;
using Frames = std::vector<std::iostream::pos_type>;

/**
  Класс, описывающий тип, хранящий задачи по свёртке и отображению.
 */
template <typename T> struct Task {
  Thread task;
  T resultOutput;
};
