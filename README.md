This is a simplified SMB clone, made with [pntr](https://github.com/robloach/pntr) & [pntr_app](https://github.com/robloach/pntr_app).

I used [mapgen](https://github.com/konsumer/mapgen) to convert screen-caps into editable levels, added collision-layer, then used [pntr_tiled](https://github.com/RobLoach/pntr_tiled) to load them.

You can use [Tiled](https://www.mapeditor.org/) to edit maps!


```
# start local, live-reloading web-server for web-version
npm start

# build native version
npm run build:raylib

# build libretro core version
npm run build:retro
```