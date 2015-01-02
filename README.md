# My first game, made with SDL2, EntityX and C++11

![Screen Shot](screenshot.png?raw=true)

I made this game as a hobby, willing to learn game development basics and improve my C++.

In this game you buy turrets in order to kill creeps, which must not get to the end of the map.
For each creep that reach the end of the map you loose 1 health.
For each creep you kill you get money.
You can buy turrets with money.

The turrets has skills like: 
- slow (slow down creeps speed)
- bleed (creeps loose health gradually)
- stun (stops creep from walking)
- area damage

Besides that, each turret has a different range, cooldown and damage.

The creeps has skills like:
- Health aura (heals creeps in aura range)
- Speed aura (speed up creeps in aura range)
- Bleed resistance (takes less damage from bleed effect)
- Stun resistance (stun effect last less)

[EntityX - A fast, type-safe C++ Entity Component System] (https://github.com/alecthomas/entityx)

[Simple DirectMedia Layer - SDL] (https://www.libsdl.org/index.php)

[Free Tower Defense Graphics] (http://www.hirefreelanceartist.com/free-tower-defense-graphics.html)

## Building

You will need cmake in order to build.

For windows you will need at least Visual Studio 2013.

Example, considering you have SDL2 installed on C:\SDL2:
```
cmake -H. -Bbuild -G "Visual Studio 12" -DSDL2_LIB_DIR=C:\SDL2\lib\X86 -DSDL2_INCLUDE_DIR=C:\SDL2\include
```
This will output a folder 'build' with SpaceTD.sln in it.

Important: In your SDL2 include folder, say C:\SDL2\include, it must have a SDL2 folder with all the .h files in it, 
so that #include "SDL2/SDL.h" can be resolved.

