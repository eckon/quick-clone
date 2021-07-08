# Quick Clone

Small cli tool to search and clone git repositories in one step.

Just add your gitlab config and be able to quickly clone any repository of that endpoint, without having to search through a web ui.


# Usage

Run the `qc` command. This will open up the interactive TUI.

The first view (query) can be used to select a gitlab endpoint and to query that endpoint with a search parameter. On enter you will get the search results.

The Search results are in the Filter tab and can be filtered by typing. On Enter the highlighted repository will be cloned and the program will be exited.


## Example

```sh
$ qc
```


# Setup

Currently there is no precompiled binaries, so to install it, the [dependencies](#dependencies) are needed. After that have a look into the compilation part (bottom).

To be able to access a private repository, the app needs an endpoint and an access token which has read access.

An array structure is used, so that it is possible to have multiple gitlab endpoints.

Add config file in "some path that i need to decide on" with a json array

* Config every element is inside an array `[{...},{...},...]`
  * name - Will be shown in the app to identify the selected enpoint
  * access_token - Is needed to have access to the endpoint, needs at least read access
  * url - URI to the gitlab endpoint

```json
[
  {
    "name": "Work",
    "access_token": "123",
    "url": "https://gitlab.my-work-site.com/api/v4/projects"
  },
  {
    "name": "Home",
    "access_token": "987",
    "url": "https://gitlab.example.com/api/v4/projects"
  }
]
```


## Sidenote

A bash like script that does a search & clone still better than this, can be found in my [dotfiles/custom-scripts](https://github.com/eckon/dotfiles/blob/master/custom-scripts/gitlab-search-and-clone.sh).

It uses multiple dependencies (jq, fzf, etc), but might be a better option for others (until this projects progressed further).


# Dependencies

- ncurses e.g. `apt install libncurses5-dev libncursesw5-dev`
- libcurl e.g. `apt install libcurl4-openssl-dev`
- nlohmann/json e.g. `apt install nlohmann-json3-dev`


## Commands for compilation etc.
```sh
# generate Makefile in build/
$ cmake -S . -B build/

# compile executable (binary can be found in ./build/bin/qc)
$ make -C build/

# format repository
$ clang-format -i --style=Google src/**/*.cpp src/**/*.h
```
