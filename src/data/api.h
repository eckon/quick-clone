#ifndef API_H
#define API_H

#include <list>
#include <string>

#include "repository.h"

std::list<Repository> getRepoResources(std::string searchValue);

#endif
