

-- we define our player as a table
player = {}
player.x = 280 
player.y = 20 -- you can also get the height by using : love.window.getHeight()
player.width = 32
player.height = 64
player.rotation = 0
player.speed = 100
player.canFall = true

-- our map blocks
block = {}

function spawnblocks(x,y)
    table.insert(block,{x=x,y=y,size=32})
end

function love.load()
    
    -- set window size (width and height)
    love.window.setMode(640, 480)
    -- set a title for our demo 
    love.window.setTitle("Rectangle little demo")
    
    -- here we define the shape of our map
    -- 1 means blocks , 0 means air 
    local map = {
{1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1}
    }
    
    for y=1,#map do 
        for x=1,#map[y] do
                if map[y][x] == 1 then
                    spawnblocks(x * 32 - 32, y * 32 - 32)
                end
            end
        end

end 

function checkCollision(x1,y1,w1,h1, x2, y2, w2, h2) 
    if 
        x1 < x2 + w2 and 
        y1 < y2 + h2 and 
        x2 < x1 + w1 and 
        y2 < y1 + h2 then
        return true;
    end
    return false
end

local timer = 0

function love.update(dt)

    -- silly trick of 'collision detection' 
    for i,v in ipairs(block) do 
       if checkCollision(player.x,player.y + 2,player.width,player.height + 2, v.x, v.y - 34, 32, 32) then
            
           if love.keyboard.isDown("space") then
            player.y = player.y - 2040 * dt
           end

            player.canFall = false
            timer = 0
        else 
            timer = timer + dt 
            if timer > 10 then
                player.canFall = true
                timer = 0
            end
       end
    end


    updateplayer(dt)
end

function updateplayer(dt)
   -- detect user input 
   -- check whether key d or right arrow has been pressed
   if love.keyboard.isDown("d","right") then 
        player.x = player.x + player.speed * dt
   end
   if love.keyboard.isDown("a","left") then
        player.x = player.x - player.speed * dt
   end
    
   -- respawn the player
    if player.y > love.window.getHeight() then player.y = 20 end

   -- make the player fall when there is no collision with the ground
   if player.canFall then
        player.y = player.y + 150 * dt
    end
end

function love.draw()
    for i,v in ipairs(block) do 
        love.graphics.setColor(50,205,100)
        love.graphics.rectangle("fill",v.x,v.y,32,32) 
        love.graphics.setColor(255,255,255)
    end

    drawplayer()
end

function drawplayer()
    
    -- set a colour for our player 
    love.graphics.setColor(255,50,100)
    -- you can choose between "fill" and "line"
    love.graphics.rectangle("fill", player.x, player.y, player.width, player.height)
    -- be sure that only our player is coloured by the parameters put above 
    love.graphics.setColor(255,255,255)
end



function love.keypressed(key) 
    if key == "esc" then
        -- if we pressed the key ESC then close the demo
        love.event.quit()
    end
end

