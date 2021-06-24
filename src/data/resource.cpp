#include "resource.h"

Resource::Resource(Repository repository) {
  this->repository = repository;
  this->hidden = false;
}

ResourceCollection::ResourceCollection(std::vector<Repository> repositories) {
  std::vector<Resource> resources = {};
  for (auto repository : repositories) {
    Resource resource(repository);
    resources.push_back(resource);
  }

  this->resources = resources;
  this->selected = 0;
}
