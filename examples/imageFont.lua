text = " abcdefghijklmnopqrstuvwxyz" ..
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ0" ..
    "123456789.,!?-+/():;%&`'*#=[]"
font = nil
font2 = nil
function love.load()	
	font = love.graphics.newImageFont("font.png", text, 11.5)
	font:setFilter("nearest","nearest")
	font:setOffsetX(0, "l")
	font:setOffsetY(0, "l")
	font2 = love.graphics.newImageFont("font2.png", text, 11.5)
end

function love.draw()
	love.graphics.print("fps: " .. love.timer.getFPS(), 50, 60)
	love.graphics.setImageFont(font2)	
	love.graphics.printb("hello Clove", 100, 100, 0, 4, 4)

	love.graphics.setImageFont(font)	
	love.graphics.printb("hey", 100, 200, 0, 4, 4)
end
