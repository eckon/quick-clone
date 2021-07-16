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
  this->apiConfigurations = configs;
  this->selected = 0;
}

void ApiConfigCollection::init() {
  std::string homeDir = std::getenv("HOME");
  std::string configDir = homeDir + "/.config/quick-clone/config.json";
  std::ifstream file(configDir);

  // user could have forgotten the config file
  if (!file) throw std::string("Config file is missing: \"" + configDir + "\"");

  // TODO this can throw different errors -> handle it in exceptions
  auto json = nlohmann::json::parse(file);
  for (auto &el : json.items()) {
    auto val = el.value();
    ApiConfig config = ApiConfig(val["name"], val["access_token"], val["url"]);
    this->apiConfigurations.push_back(config);
  }

  file.close();
}
