# Quick Clone

A Small cli tool to search and clone git repositories from the terminal (currently only Gitlab-v4).

This tool was only tested on my machine, but I am open to improve it if people are interested.

If it does not work and/or you want another quick solution, have a look at the [side-node](#side-note) for a bash script with most functionality.


# Usage

Run the `qc` command. This will open up the interactive TUI (The [config file](#setup) needs to be added before first usage).

Views
* Query-Prompt (start position)
  * Select between your different git endpoints with `UP/DOWN` (these were given by the config file)
  * Type in the prompt to request repository name from highlighted endpoint
  * Fire request on `ENTER`
    * Nothing found -> continue with the Query
    * Something found, automatic switch to Filter-Prompt
* Filter-Prompt (`LEFT/RIGHT` let's you switch between the Prompts)
  * Select the wanted repository with `UP/DOWN`
  * Typing will allow basic filtering of the given list
  * Terminate app and clone highlighted repository on `ENTER`
  * Switch back to Query-Prompt with `LEFT/RIGHT`


# Setup

* Currently there is no pre-compiled binaries
  * To install: have the needed [dependencies](#dependencies) and [compile](#commands) the tool on your machine
* Add access token in Gitlab with (at least) Repository-read permissions
  * Can be found in your personal settings under "Access Tokens"
* Create config file at `~/.config/quick-clone/config.json` (_names etc. might change_)
  * An array structure is used, for the purpose of having multiple Gitlab-endpoints
  * Every element is an objcet inside the array `[{...}, {...}, ...]` with the following
    * `name` - Will be shown in the app to identify the selected endpoint
    * `access_token` - Is needed to have access to the endpoint, needs at least read access
    * `url` - URI to the Gitlab endpoint

Example
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


## Side-note

The first version of this tool was a simple bash script, which uses different tools like `fzf`, `jq`, `awk`, `curl` and maybe others.

This should work on most machine that supports bash and the other tools and can be found in my [dotfiles](https://github.com/eckon/dotfiles/blob/master/custom-scripts/gitlab-search-and-clone.sh).


# Dependencies

- ncurses e.g. `apt install libncurses5-dev libncursesw5-dev`
- libcurl e.g. `apt install libcurl4-openssl-dev`
- nlohmann/json e.g. `apt install nlohmann-json3-dev`


## Commands
```sh
# generate Makefile in build/
$ cmake -S . -B build/

# compile executable (binary can be found in ./build/bin/qc)
$ make -C build/

# format repository
$ clang-format -i --style=Google src/**/*.cpp src/**/*.h
```
