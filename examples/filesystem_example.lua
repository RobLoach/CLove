
local dir = "" -- put a dir
local files = love.filesystem.enumerate(dir)
for k, file in ipairs(files) do
    print(k .. ". " .. file) --outputs something like "1. main.lua"
end

local a = "vlad"
local b = "vlad"
print("\n")
print("is " .. a .. " the same as " .. b .. "?")
print(love.filesystem.compare(a,b,#b))

local a = "Murii"
local b = "vlad"
print("\n")
print("is " .. a .. " the same as " .. b .. "?")
print(love.filesystem.compare(a,b,#b))

v = {0,0,1,0,0,0,1}
e = {1}
print("\n")
print("1)String test: ") 
for r = 0, #v do
    print(love.filesystem.contain(tostring(v[r]),tostring(1)))
end

v = "0,0,0,0,0,0,1"
e = "12"
print("\n")
print("2)String test: ")
print(love.filesystem.contain(v,e)) -- e false 

v = "0,0,0,0,0,0,1"
e = "1"
print("\n")
print("3)String test: ")
print(love.filesystem.contain(v,e)) -- e true

print(love.filesystem.getCurrentDirectory())
-- e = exists, default 
-- w = can write to that file
-- r = can read from that file
-- wr = can read and can write to that file
print(love.filesystem.isFile("test.txt", "r"))



--create a file 
--love.filesystem.load works like in love2d
--love.filesystem.remove works like in love2d
--love.filesystem.exists works like in love2d
--love.filesystem.append works like in love2d

local list_of_lines = love.filesystem.lines("text.txt")
for i=1,#list_of_lines do
	if (love.filesystem.equals("LOOK_FOR_SOMETHING", list_of_lines[i])) then
		print(list_of_lines[i])	
	end	
end

print("\n")
print("Does TEST.txt exist?")
print(love.filesystem.exists("TEST.txt"))
love.filesystem.write("TEST.txt","Hello World!\n")
print("Now it should have been created succesfully")
print(love.filesystem.exists("TEST.txt"))
love.filesystem.append("TEST.txt", "Hey from append!")
