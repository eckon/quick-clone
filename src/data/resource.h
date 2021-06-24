#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>

#include "repository.h"

// TODO: make repository/hidden private
class Resource {
 public:
  Repository repository;
  bool hidden;
  int index;

  Resource();
  Resource(Repository repository);
};

// TODO: make resources/selected private
class ResourceCollection {
 public:
  std::vector<Resource> resources;
  int selected;

  ResourceCollection(std::vector<Repository> repositories);

  bool next();
  bool previous();
  void applyFilter(std::string filter);
  std::vector<Resource> getFilteredResources(std::string filter);
};

#endif
