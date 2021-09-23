# Features

## Needed
- better error output in case config values are missing (currently only generic json3 error that helps with nothing)
  - nlohmann::detail::type_error ... type must be string, but is null

## Wanted
- remove dependencies (mainly nlohmann/json3)
  - only things that are really needed is ncurses for the ui and curl for http requests
  - the return of curl can be parsed manually
  - the config file should probably change format as well (maybe somehitng like yaml or a key value pair or env vars)
    - so that i can remove the json3 dependency completly
  - after that remove it from readme etc.
- Have a look into [libraries](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/) which might abstract some things
- Use optional or something similar to wrap non existing objects
- Make Generic which will handle collections (for lists) and input (for data input) (with selected property)
  - so that i do not need to always wrap my classes in collection wrappers who just add basic things like a selection entry for the highlight position

## Nice to have
- Draw additional information of highlighted entry on screen
- Add way to change parameter inside of application
  - ordering amount of returned elements etc.
- Add loading animation while waiting for curl
  - Instead of just showing the modal window
- Add some tutorial, maybe add "Filter here" on empty prompt etc
  - or use the modal to show basic instructions on first open
- Make a grid for easier window position
- Rename different classes, methods etc. currently some have really bad names
- Allow better typing
  - enable cursor movement with arrow keys or similar
  - general things, like copy/paste, word deletion etc (what is expected of an editor)
- Update the allowed typed characters, umlaute etc do not work currently
- Add fuzzy search
  - with this also split filtering and output, so that these do not need to know each other
- Allow multiple filter words (probably divided by space)
- Handle show of filtered resources differently (keep highlight centered)
  - currently we go all the way to the bottom which is can be annoying (you can not see the next entry when on bottom)
