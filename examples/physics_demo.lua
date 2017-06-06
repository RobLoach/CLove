
local x, y = 150, 100
local w,h,r = 16, 16, 0

local x2, y2 = 100, 200
local w2, h2 = 256, 64
local r2 = 0

local x3, y3, rad, r3 = 200, 100, 12, 0

function love.load()
    
    space = love.physics.newSpace(0, 100)

    body = love.physics.newBoxBody(space, 10, w, h, 0, "dynamic")
    body:setPosition(x, y)
    shape = love.physics.newBoxShape(space, body, w, h, 0, 0, -7)
	shape:setFriction(80)

	body2 = love.physics.newBoxBody(space, 1, w2, h2, 0, "static")
	body2:setPosition(x2,y2)
	shape2 = love.physics.newBoxShape(space, body2, w2, h2, 0)
	shape2:setFriction(2)
	
	
	body3 = love.physics.newCircleBody(space, 1, rad, 1, 0, 0, "dynamic")
	body3:setPosition(x3, y3)
	shape3 = love.physics.newCircleShape(space, body3, rad, 0, -12)
	shape3:setFriction(1)
	
	
	end

function love.update(dt)
	x, y = body:getPosition(space)
		
	if love.keyboard.isDown("d") then
		body:setVelocity(100, 0)
	end
	
	if love.keyboard.isDown("a") then
		body:setVelocity(-100, 0)
	end
		
	if love.keyboard.isDown("w") then
		body:setVelocity(0, -100)
	end
		
	if love.keyboard.isDown("s") then
		body:setVelocity(0, 100)
	end

	r = body:getAngle(space)
	
	x2, y2 = body2:getPosition(space)
	r2 = body2:getAngle(space)
	
	x3, y3 = body3:getPosition(space)
	r3 = body3:getAngle(space)

    love.physics.update(space, dt)
	
	if love.keyboard.isDown("esc") then
		love.event.quit()
	end
	
end

function love.draw()
    love.graphics.rectangle("fill", x, y, w, h, r)
	love.graphics.rectangle("fill", x2, y2, w2, h2)
	love.graphics.circle("fill", x3, y3, 12, rad, r)
end


