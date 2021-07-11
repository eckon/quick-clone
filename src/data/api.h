#ifndef API_H
#define API_H

#include <string>
#include <vector>

#include "config.h"
#include "repository.h"

std::vector<Repository> getRepoResources(std::string searchValue,
                                         ApiConfig config);

#endif
