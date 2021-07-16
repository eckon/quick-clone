# Features

## Needed
- Add version number
- Add license
- Add screenshots/gifs
- Redo readme (and other meta data of repo) for final update
- Look maybe into distributing the binary (maybe cpp has an easy way?)

## Wanted
- Use optional or something similar to wrap non existing objects
- Make Generic which will handle collections (for lists) and input (for data input) (with selected propterty)
  - so that i do not need to always wrap my classes in collection wrappers who just add basic things like a selection entry for the highlight position

## Nice to have
- Draw additional information of highlighted entry on screen
- Add way to change parameter inside of application
  - ordering amount of returned elements etc.
- Add loading animation while waiting for curl
  - Instead of just showing the modal window
- Add some tutorial, maybe add "Filtere here" on empty prompt etc
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
