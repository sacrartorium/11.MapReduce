#include "sort.hpp"

#include <algorithm>

Container MergeSort::execute(VectorContainers data) {
  Container container;

  for (auto &part : data) {
    Container temp;

    std::merge(part->begin(), part->end(), container.begin(), container.end(),
               std::back_inserter(temp));

    container = temp;
  }

  return container;
}
