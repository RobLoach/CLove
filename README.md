CLove
=====
CLove is a simple and easy to learn and use game framework tested on
Mac/Linux/PC, made in C with OpenGLES 2 which uses Lua as a scripting langauge.

How to build
============
In order to build CLove you'll need CMake and GCC. Download or clone CLove 
and call "cmake ../ && make" from a new folder called "build". If you are on
Windows then you'll need CMake GUI.

Features
========
- Lua scripting language 
- Cross Platform 
- Powerful Batch system
- Image loader & drawing
- Sound loader & playing (Vorbis and Wav)
- Primitive drawing
- Filsystem functions
- OpenGL ES 2.0 
- Font loading & drawing
- Almost the same functions names as in Love2D
- Keybaord & Mouse functions
- Physics system thanks to ChipMunk

Examples
--------
```lua
local image = love.graphics.newImage("image.png")

function love.draw()
   --draw the loaded image named "image" at the postion X:200, Y:200
	love.graphics.draw(image, 200, 200)
	--draw a circle at X:100 Y:100 with the radius of 32 and number of
	--segments(points) of 24
	love.graphics.circle("fill", 100, 100, 32, 24)
end
```

Images of running CLove demos
-----------------------------
![Image 1:](data/1.png?raw=true "Web")
![Image 2:](data/2.png?raw=true "Linux")
![Image 2:](data/3.png?raw=true "Os X")


