#include "config.h"

#include <fstream>
#include <nlohmann/json.hpp>

ApiConfig::ApiConfig() {
  this->accessToken = "";
  this->name = "";
  this->url = "";
}

ApiConfig::ApiConfig(std::string name, std::string accessToken,
                     std::string url) {
  this->accessToken = accessToken;
  this->name = name;
  this->url = url;
}

ApiConfigCollection::ApiConfigCollection() {
  std::vector<ApiConfig> configs = {};

  std::string homeDir = std::getenv("HOME");
  std::ifstream file(homeDir + "/.config/quick-clone/config.json");

  auto json = nlohmann::json::parse(file);
  for (auto &el : json.items()) {
    auto val = el.value();
    ApiConfig config = ApiConfig(val["name"], val["access_token"], val["url"]);
    configs.push_back(config);
  }
  file.close();

  this->apiConfigurations = configs;
  this->selected = 0;
}
