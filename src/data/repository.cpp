#include "repository.h"

Repository::Repository(std::string ssh_url_to_repo, std::string name,
                       std::string name_with_namespace,
                       std::string http_url_to_repo, std::string web_url) {
  this->ssh_url_to_repo = ssh_url_to_repo;
  this->name = name;
  this->name_with_namespace = name_with_namespace;
  this->http_url_to_repo = http_url_to_repo;
  this->web_url = web_url;
}
