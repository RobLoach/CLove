
math.randomseed(os.time())

image = love.graphics.newImage("hey.png")
function love.load()
    local vertices = {
        {
            -- top-left corner 
            0, 0, -- position of the vertex
            0, 0, -- texture coordinate at the vertex position
            math.random(0,255), math.random(0,255), math.random(0,255), 255 -- color of the vertex
        },
        {
            -- top-right corner 
            image:getWidth(), 0,
            1, 0, -- texture coordinates are in the range of [0, 1]
            math.random(0,255), math.random(0,255), math.random(0,255), 255
        },
        {
            -- bottom-right corner 
            image:getWidth(), image:getHeight(),
            1, 1,
            math.random(0,255), math.random(0,155), math.random(0,255), 255
        },
        {
            -- bottom-left corner 
            0, image:getHeight(),
            0, 1,
            math.random(0,255), math.random(0,255), math.random(0,255), 255
        },
    }
    local indices = 
    {
        {
            0, 1, 2, 3
        },
    }
    mesh = love.graphics.newMesh(vertices, 4, indices, "fan")
end

function love.update(dt) 
    if love.keyboard.isDown("esc") then love.event.quit() end
    if love.keyboard.isDown("a") then 	mesh:setTexture(image) end
    if love.keyboard.isDown("d") then 	mesh:setTexture(nil) end
    if love.keyboard.isDown("e") then 
        newVertices = {
            {
                -- top-left corner 
                love.math.random(-200,200), love.math.random(-200,200), -- position of the vertex
                0, 0, -- texture coordinate at the vertex position
                155, 45, 5, 255 -- color of the vertex
            },
            {
                -- top-right corner 
                image:getWidth(), love.math.random(0,200),
                1, 0, -- texture coordinates are in the range of [0, 1]
                55, 5, 155, 255
            },
            {
                -- bottom-right corner
                image:getWidth(), image:getHeight(),
                1, 1,
                255, 255, 255, 255
            },
            {
                -- bottom-left corner 
                love.math.random(-200,200), image:getHeight(),
                0, 1,
                255, 255, 255, 255
            },
        }
        mesh:setVertices(newVertices) 
    end 
end

function love.draw()
    love.graphics.draw(mesh, 200, 300)
end
