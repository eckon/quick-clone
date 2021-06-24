#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>

#include "repository.h"

class Resource {
 public:
  Repository repository;
  bool hidden;

  Resource();
  Resource(Repository repository);
};

class ResourceCollection {
 public:
  std::vector<Resource> resources;
  int selected;

  ResourceCollection(std::vector<Repository> repositories);
};

#endif
