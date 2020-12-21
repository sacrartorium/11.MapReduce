#include "split.hpp"

Split::Split(std::string fileName, size_t countMaps)
    : mFile(fileName), mCountMapThreads(countMaps) {}

Split::Frames Split::calculateFrames(long fileEnd, long sectionSize) {
  Frames frames;
  frames.reserve(mCountMapThreads);

  long currentSectionEnd = sectionSize;

  while ((currentSectionEnd < fileEnd) && (currentSectionEnd >= 0)) {
    auto byte = '\0';

    mFile.seekg(currentSectionEnd, std::ios_base::beg);

    while (byte != '\n' && !mFile.fail())
      mFile.read(&byte, 1);

    currentSectionEnd = mFile.tellg();

    if (currentSectionEnd >= 0) {
      frames.push_back(currentSectionEnd);
      currentSectionEnd += sectionSize;
    }
  }

  frames.push_back(fileEnd);

  return frames;
}

Split::Frames Split::execute() {
  if (!mFile.good())
    throw std::ifstream::failure("Error read file.");

  mFile.seekg(0, std::ios_base::end);

  long fileEnd = mFile.tellg(), section = fileEnd / mCountMapThreads;

  mFile.seekg(section, std::ios_base::beg);

  return calculateFrames(fileEnd, section);
}
