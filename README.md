# What is it?

This is a little Minecraft-style thing.

# What does it do?

You can place blocks, and remove them. You can edit the `blocks` file in a hex editor and edit `texture.png` to add new block types to the game. I'm planning on building an editor for this purpose as well. Right now, the only thing you can define for a block is its name, a nice name and the index into the texture for it to use for each side, check the struct in `BlockData.h` for details.

# What doesn't it do?

Everything else.

# What are the controls?

WASD - Move  
QE - Move directly up or down in fly mode  
Mouse - Look  
LMB - Remove block  
RMB - Place block  
MMB - Select block that you're looking at (for placement)  
G - Disable gravity  
ESC - [Finish It](https://f4.bcbits.com/img/a2856518997_16.jpg)

# Feature Hitlist

Normally they call this a roadmap, but a hitlist sounds way cooler, right now my priorities are:

* Text and UI rendering
* Block types that are not cubes, e.g. flowers
* Some kind of optimization for the chunk mesher, right now it causes a tiny hitch as it regenerates the entire mesh at once for every change made (per frame, if multiple changes are made in one frame, only one update is performed), this could be optimized but I'm not too worried about it because it's beyond unnoticeable in an optimized Release build. **This could easily be removed by parallelizing the mesh generation, which would cut the time in half at least, however, I'd like to find a better solution**

# What does it depend upon?

* glfw3
* libpng (by extension zlib)
* openal (isn't actually used right now, but will be)
* freetype2
* glm

# Where can I build it?

The CMAKE build MAY work on Visual Studio with some configuration, however I haven't tested it. I know it builds on Linux with `clang++`, `g++` not tested. All my code should be portable between Linux and Windows (MacOS not supported as of now).

# Thanks to...

Mojang for the textures I stole (they're placeholders!!!)  
[This guy](http://noonat.github.io/intersect/) who wrote the 2D AABB tests I based my collision off  
Everyone who gave me the validation I needed to be motivated to continue with this, as long as I do  
My parents for being cool

<sup><sup>ahoy spongebob, i have broken several copyright and trademark laws and am going to be sentenced to life in prison agagagagagagagag</sup></sup>
