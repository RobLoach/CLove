
--[[ 
   I enourage you to use love.filesystem.require because 
   this function takes into account wether or not your
   game is in packaging more or not. 
    
   NOTE: main.lua can't be put inside the package (fixme?)

   NOTE: You can steel use "require" but then 
   when you package your game you have to get ride of all 
   instance of "require"
--]]

love.filesystem.require("test.lua")
love.filesystem.require("test2.lua")

function love.load( ... )
  test.load()
  test2.load()
end

function love.draw( ... )
  test.draw()
  test2.draw()
end
