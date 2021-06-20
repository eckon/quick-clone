# Quick Clone

Small cli tool to search and clone git repositories in one step.

Just add your gitlab config and be able to quickly clone any repository of that endpoint, without having to search through a web ui.


# Usage

Run the command `qc` with a search term of the repository (currently only the name of the git repository).

## Example

```sh
# Results in a searchable buffer with all entries which include the word "quick"
$ qc quick
```


# Setup

Currently there is no precompiled binaries, so to install it, the [dependencies](#dependencies) are needed.

To be able to access a private repository, the app needs an endpoint and an access token which has read access.

For now these are used by having them as ENV-Variables (similar to the $PATH etc.). Needed are:

- GITLAB_ACCESS_TOKEN="YOUR-TOKEN"
- GITLAB_PROJECTS_URL="YOUR-REPOSITORY-API-ENDPOINT"


## Sidenote

A bash like script that does a search & clone still better than this, can be found in my [dotfiles/custom-scripts](https://github.com/eckon/dotfiles/blob/master/custom-scripts/gitlab-search-and-clone.sh).

It uses multiple dependencies (jq, fzf, etc), but might be a better option for others (until this projects progressed further).


# Dependencies

- ncurses
- libcurl
- nlohmann/json


## Commands
```sh
# compile and run
$ make && ./bin/main
```
