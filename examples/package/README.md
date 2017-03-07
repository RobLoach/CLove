
# Usage 
./clove game.clove.tar N C1, C2
where
    N: number of classes to be run. eg: 2
    C1,C2: classes to load and run. eg: test.lua test2.lua

If you want to pack your game you have to:
tar -cvf X.clove.name C1, C2 ... CN 
where 
	X: name of your game, no "" required. eg: game.clove.tar 
	C1,C2 ... CN: files that make your game, no "" require. eg: test.lua test2.lua
Note: 
	Do not pack main.lua nor the assets!

# Important
   I enourage you to use love.filesystem.require because 
   this function takes into account wether or not your
   game is in packaging mode or not. 
    
   NOTE: main.lua can't be put inside the package (fixme?)

   NOTE: You can stil use "require" but 
   when you package your game you have to get ride of all 
   instance of "require"

As of current version (0.7.0) assets can't be put inside the package but
I'm planning in changeing that by 1.0.0. Maybe ^_^
