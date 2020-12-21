#include "map.hpp"
#include "reduce.hpp"
#include "sort.hpp"
#include "split.hpp"

#include <iostream>
#include <sstream>

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Error: map_reduce <src> <mnum> <rnum>" << std::endl;
    return -1;
  };

  std::string fileName = argv[1];
  size_t mapThreads = std::stoi(argv[2]);
  size_t reduceThreads = std::stoi(argv[3]);

  if (mapThreads <= 0 || reduceThreads <= 0) {
    std::cout << "need threads > 0" << std::endl;
    return -1;
  }

  // Seek and split
  Split stage1(fileName, mapThreads);
  auto frames = stage1.execute();

  // Map with userFn
  Map stage2(fileName, mapThreads);
  auto mapped = stage2.execute(
      frames, [&map = stage2](
                  std::ifstream &file, std::iostream::pos_type beginSegment,
                  std::iostream::pos_type endSegmant, ContainerPtr container) {
        // Read data to memory and split it to strings.
        size_t length = endSegmant - beginSegment;
        std::string buffer;
        buffer.resize(length);

        try {
          std::lock_guard<std::mutex> lock(map.mMutex);

          file.seekg(beginSegment, std::ios_base::beg);
          file.read(buffer.data(), length);
        } catch (std::exception &e) {
          std::cout << "Error read file. " << e.what() << std::endl;
        }

        // Split read part
        std::stringstream splitted(buffer);
        std::string segment;

        while (std::getline(splitted, segment, '\n')) {
          for (size_t idx = 1; idx < segment.length(); idx++) {
            container->push_back(segment.substr(0, idx));
          }
        }
      });

  // Merge sort
  MergeSort stage3;
  auto sorted = stage3.execute(mapped);

  // Reduce with userFn and output
  Reduce stage4(sorted, reduceThreads);
  stage4.execute([](ContainerPtr &data,
                    [[maybe_unused]] std::shared_ptr<std::ofstream> stream) {
    // Find minimal prefix
    if (data->empty())
      return;

    size_t prefixSize = 1;
    std::string current("");

    for (auto &prefix : *data) {
      *stream << prefix << std::endl;

      if (prefixSize <= prefix.length()) {
        if (current == prefix) {
          prefixSize = prefix.length() + 1;
        } else {
          current = prefix;
        }
      }
    }

    std::string output = "Prefix length = '" + std::to_string(prefixSize) + "'";
    std::cout << output << std::endl;
  });

  return 0;
}
