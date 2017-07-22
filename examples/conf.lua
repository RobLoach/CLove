
function love.conf(t)
   	t.window.identity = "."
	t.window.version = "0.7.0"
    t.window.width = 640
    t.window.height = 480
    t.window.x = -1
    t.window.y = -1
    t.window.window = true
    t.window.vsync = true
    t.window.resizable = false
    t.window.title = "No title in conf.lua"
    t.window.stats = true
end
