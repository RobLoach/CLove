![Alt text](CLoveLogo.png?raw=true "CLove")

CLove
=====
CLove is a simple, easy to learn and use 2D game framework tested on
Mac/Linux/PC, made in C with OpenGLES 2 which uses Lua as scripting langauge.

How to build
============
On Windows: 
- Download and install mingw and let the setup configure the path for you. 
 Open up the CMD and type gcc if you get an error then type this: setx PATH "%PATH%;C:\MinGW\bin;" . That command will add 
 the bin folder to the path.
 After that check if you got gcc & g++ installed.
- Download and install CMake and let the setup configure the path for you.
- Make a new directory called build inside CLove and call: 
 cmake ../ -DCMAKE_C_COMPILER:PATH=C:/MinGW/bin/gcc.exe -DCMAKE_CXX_COMPILER:PATH=C:/MinGW/bin/g++.exe . If this command 
 does not work then go in C:\Program Files\CMake\bin and open up cmake-gui.exe. Tell it where is Clove and where you want to build the   project. Also do not forget to use custom compiler that is set to GCC as C compiler and G++ as the C++ compiler. 
 After that use the make command and that's it( if you get errors that make does not exist then paste this into CMD terminal: 
  copy c:\MinGW\bin\mingw32-make.exe c:\MinGW\bin\make.exe )
- Download DX SDK 2010 only if you get errors from SDL when building: http://www.microsoft.com/en-us/download/details.aspx?id=6812
- After you have succesfuly build CLove go into src/3rdparty directory and copy: SDL2, openal .dll in the same directory where clove.exe is.

On Linux and OS X:
- Download and install cmake,gcc,g++(optional git)(on OSX you can install these using brew)
- Make a new directory inside Clove, eg: build. Go there and call: cmake ../ &&
  make 
- If you have problems with building on Linux(Debian based) install: 
sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev 
libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libasound2-dev 
libaudio-dev libesd0-dev libpulse-dev libroar-dev

Features
========
- Lua scripting language or native C/C++ support.
- 3D camera functions.
- Easy to learn and use api.
- Cross Platform. Android is planned. Web needs a new build system.
- Custom package format.
- Powerful Batch system.
- Image loading and drawing.
- Image creation from scratch or from a template & save(png,bmp,tga).
- Meshes.
- Sound loading and playing (Vorbis and Wav).
- Streaming support for Vorbis files.
- Primitive drawing.
- Filesystem functions.
- OpenGL ES 2.0.
- Networking (TCP,IPv4 or IPv6)
- Powerful font loading and drawing using batch system.
- Support for image fonts. 
- Keyboard, mouse and joystick support.
- Threads.
- Error handling.

Contributing
========
If you want to contribute to the project, feel free to work on what you please. I want to do a lot of the other stuff myself too.
Please try to replicate the code style of the existing code by using tab width of 4 spaces and expand tabs to spaces, I will refuse to pull your changes otherwise.

Stuff needed: 
    - Physics, love.physics
	- Networking 
	- Web build system 

Examples (see examples folder)
--------
```lua
-- Example of drawing an image
local image = love.graphics.newImage("image.png")

function love.draw()
	love.graphics.draw(image, 200, 200)
end
```

```lua
-- Example of drawing some primitives
function love.draw()
	love.graphics.rectangle("fill", 100, 100, 32, 16)
	love.graphics.rectangle("line", 200, 200, 32, 32)
	love.graphics.circle("fill", 270, 200, 32, 16)
	love.graphics.circle("line", 300, 100, 32, 8)
end
```

```lua
-- Example of playing music
local ogg_music = love.graphics.newSource("music.ogg")
function love.load()
	ogg_music:play()
end
```

CLove pictures
-----------------------------
![Image 1:](data/4.png?raw=true "See examples folder")
![Image 2:](data/1.png?raw=true "Web")
![Image 3:](data/2.png?raw=true "Linux")
![Image 4:](data/3.png?raw=true "Os X")


