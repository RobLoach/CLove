
--Joystick is separated lua file ! 

local text = ""
local a = 0

function love.load( ... )
	text = "type smt -- "
end

function love.textinput(t)
	text = text .. t 
end

function love.draw()
	love.graphics.printf(text,100,100,#text + 13)
	love.graphics.print("hey",200,200)
end

function love.update(dt)
	if love.keyboard.isDown("v","e") then 
		print("v or e pressed")
	end
end

function love.mousereleased(x,y,b)
	if b == "l" then 
		print("L has been released " .. x .. " " .. y)
	end
end

function love.mousepressed(x,y,b)
	if b == "l" then 
		print("L has been pressed")
	end
end

function love.keypressed(k)
	if k == "d" then 
		print("Hey!")
	end
	if k == " " or k == "esc" then
		print("QUIT")
	end
end

function love.keyreleased(k)
	if k == "d" then 
		print("bye")
	end
end

function love.wheelmoved(w)
	if w ~= 0 then print("WHEEL HAS BEEN MOVED: " .. w) end
end
