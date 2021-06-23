#ifndef RESOURCE_H
#define RESOURCE_H

#include <list>

#include "repository.h"

class Resource {
 public:
  Repository *repository;
  bool hidden;

  Resource(Repository &repository);
};

class ResourceCollection {
 public:
  std::list<Resource> resources;
  int selected;

  ResourceCollection(std::list<Repository> repositories);
};

#endif
