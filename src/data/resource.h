#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>

#include "repository.h"

class Resource {
 public:
  Repository repository;
  bool hidden;
  int index;

  // the pair shows the beginning and the end of matching pairs
  std::vector<std::pair<int, int>> filterMatchRanges;

  Resource();
  Resource(Repository repository);
};

class ResourceCollection {
 public:
  std::vector<Resource> resources;
  int selected;

  ResourceCollection(std::vector<Repository> repositories);
  ResourceCollection();

  bool next();
  bool previous();
  void applyFilter(std::string filter);
  std::vector<Resource> getFilteredResources(std::string filter);
  int getFilteredSelectedIndex(std::vector<Resource> filteredResources);
};

#endif
