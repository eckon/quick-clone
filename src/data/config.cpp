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

void ApiConfigCollection::generatePlaceholderConfigFile(std::string path, std::string filename) {
  std::string createFolder = "mkdir -p " + path;
  std::system(createFolder.c_str());

  std::string configFilePath = path + "/" + filename;
  std::ofstream outfile(configFilePath);

  std::string configExampleString =
    "[\n"
    "  {\n"
    "    \"name\": \"Official Gitlab\",\n"
    "    \"access_token\": \"\",\n"
    "    \"url\": \"https://gitlab.com/api/v4/projects\"\n"
    "  },\n"
    "  {\n"
    "    \"name\": \"Your Work Repository Manager     (see README for configuration)\",\n"
    "    \"access_token\": \"replace-with-token-see-README\",\n"
    "    \"url\": \"https://your-work-endpoint.com/path/to/api\"\n"
    "  },\n"
    "  {\n"
    "    \"name\": \"Your Private Repository Manager  (config at: '" + configFilePath + "')\",\n"
    "    \"access_token\": \"replace-with-token-see-README\",\n"
    "    \"url\": \"https://your-private-endpoint.com/path/to/api\"\n"
    "  }\n"
    "]";

  outfile << configExampleString << std::endl;
  outfile.close();
}

void ApiConfigCollection::init() {
  std::string homeDir = std::getenv("HOME");
  std::string configDir = homeDir + "/.config/quick-clone";
  std::string configFileName = "config.json";
  std::string configFilePath = configDir + "/" + configFileName;
  std::ifstream file;
  file.open(configFilePath);

  // user could have forgotten the config file
  // generate a basic one so that it at least shows the working app
  if (!file) {
    file.close();
    this->generatePlaceholderConfigFile(configDir, configFileName);

    // reload the file
    file.open(configFilePath);
  }

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
