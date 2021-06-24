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

bool ResourceCollection::previous() {
  if (this->selected - 1 < 0) return false;

  // iterate from the selection and go back to get the previous visible element
  for (int i = this->selected; i >= 0; i--) {
    if (this->selected == i) continue;
    if (this->resources[i].hidden) continue;

    this->selected = i;
    return true;
  }

  return false;
}

bool ResourceCollection::next() {
  int amount = this->resources.size();
  if (this->selected + 1 >= amount) return false;

  // iterate from the selection and go forward to get the next visible element
  for (int i = this->selected; i < amount; i++) {
    if (this->selected == i) continue;
    if (this->resources[i].hidden) continue;

    this->selected = i;
    return true;
  }

  return false;
}
