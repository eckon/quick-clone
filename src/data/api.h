#ifndef API_H
#define API_H

#include <string>
#include <vector>

#include "repository.h"

std::vector<Repository> getRepoResources(std::string searchValue);

#endif
