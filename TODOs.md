# Features

## General
- Draw additional information of highlighted entry on screen
- Add way to change parameter inside of application
  - this could also include selection of different git endpoints
  - this should allow to add multiple endpoints as well
- Add way to add access token etc in a better way
  - maybe a config file?
- Add loading animation while waiting for curl
  - Might have to look into different modes (halfdelay/timeout)
- **Use optional or something similar to wrap non existing objects**

## Typing
- Allow better typing
  - correctly show cursor
  - enable cursor movement with arrow keys or similar
  - general things, like copy/paste, word deletion etc (what is expected of an editor)
- Update the allowed typed characters, umlaute etc do not work currently

## Filter
- Add fuzzy search
  - with this also split filtering and output, so that these do not need to know each other
- Allow multiple filter words (probably divided by space)
- Handle show of filtered resources differently (keep highlight centered)
  - currently we go all the way to the bottom which is can be annoying (you can not see the next entry when on bottom)
