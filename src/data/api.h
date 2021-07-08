#ifndef API_H
#define API_H

#include <string>
#include <vector>

#include "repository.h"

class ApiConfig {
 public:
  std::string name;
  std::string accessToken;
  std::string url;

  ApiConfig();
  ApiConfig(std::string name, std::string accessToken, std::string url);
};

class ApiConfigCollection {
 public:
  std::vector<ApiConfig> apiConfigurations;
  int selected;

  ApiConfigCollection();
};

ApiConfigCollection TMPcreateConfigCollection();

std::vector<Repository> getRepoResources(std::string searchValue,
                                         ApiConfig config);

#endif
