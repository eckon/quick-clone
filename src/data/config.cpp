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

  // build array of missing config parts, to display to the user
  std::vector<std::string> errorList;

  // TODO: handle other exceptions as well
  // this will still throw exceptions on wrong json format
  auto json = nlohmann::json::parse(file);
  int index = 0;

  for (auto &el : json.items()) {
    auto val = el.value();

    auto name = val["name"];
    if (name == nlohmann::detail::value_t::null) {
      name = "";
      errorList.push_back(std::to_string(index) + ": name");
    }

    auto accessToken = val["access_token"];
    if (accessToken == nlohmann::detail::value_t::null) {
      accessToken = "";
      errorList.push_back(std::to_string(index) + ": accessToken");
    }

    auto url = val["url"];
    if (url == nlohmann::detail::value_t::null) {
      url = "";
      errorList.push_back(std::to_string(index) + ": url");
    }

    ApiConfig config = ApiConfig(name, accessToken, url);
    this->apiConfigurations.push_back(config);
    index++;
  }

  if (errorList.size() > 0) {
    std::string errorMessage = "Following configurations are missing, with array index:";
    for (auto const error : errorList) {
      errorMessage = errorMessage + "\n" + error;
    }
    throw errorMessage;
  }

  file.close();
}
