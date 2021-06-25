#include "resource.h"

Resource::Resource(Repository repository) {
  this->repository = repository;
  this->hidden = false;
  this->index = 0;
}

ResourceCollection::ResourceCollection(std::vector<Repository> repositories) {
  std::vector<Resource> resources = {};
  int index = 0;
  for (auto repository : repositories) {
    Resource resource(repository);
    resource.index = index;
    resources.push_back(resource);
    index++;
  }

  this->resources = resources;
  this->selected = 0;
}

std::vector<Resource> ResourceCollection::getFilteredResources(
    std::string filter) {
  this->applyFilter(filter);
  std::vector<Resource> resources = {};
  for (auto &resource : this->resources) {
    if (resource.hidden) continue;
    resources.push_back(resource);
  }
  return resources;
}

void ResourceCollection::applyFilter(std::string filter) {
  // set hidden flag of resource, when it does not fit filter
  for (auto &resource : this->resources) {
    bool hasSubString =
        resource.repository.ssh_url_to_repo.find(filter) != std::string::npos;
    resource.hidden = !hasSubString;
  }
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

int ResourceCollection::getFilteredSelectedIndex(
    std::vector<Resource> filteredResources) {
  int selectedIndex = 0;
  for (auto const &r : filteredResources) {
    if (r.index == this->selected) break;

    selectedIndex++;
  }

  return selectedIndex;
}
