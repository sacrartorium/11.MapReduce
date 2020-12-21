#pragma once

#include <fstream>
#include <vector>

/**
 Класс, расчленяющий файл на части.
 */

struct Split {
  using Frames = std::vector<std::iostream::pos_type>;

  Split(std::string fileName, size_t countMaps);
  Frames execute();

private:
  Frames calculateFrames(long fileEnd, long sectionSize);

  std::ifstream mFile;
  size_t mCountMapThreads;
};
