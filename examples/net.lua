

--[[
	
  Usage:
  Put "is_client" to false when you first run the test.
  This will create the server which will be waiting for a connection.
  Open up another teminal and run clove but first put "is_client" to true.
  This will make the program to connect to the server.
  
 After everything is done you should get e message from the server in the termianl
 also the server should have outputed something in the console :).
 
 Enjoy!     
--]]

local is_client = true

local port = 4935

function love.load()
	
	local socket = 0

	if is_client then
		print("Starting client...")

		love.net.load("localhost", port, "ipv4")
		socket = love.net.createSocket()
		love.net.connect()

		recieve_msg = "";
		love.net.recieve(recieve_msg, 5)
		
		print(recieve_msg)

		love.net.close()
	else
		print("Starting server...")

		-- server 
		love.net.load("localhost", port, "ipv4")
		socket = love.net.createSocket()
		love.net.bind()
		love.net.listen(2)

		local temp_socket = love.net.accept()
		--while (temp_socket) do 
			print("Incomming connection from ip: TODO")
			size = love.net.send(temp_socket, "Hello", 5)
			temp_socket = love.net.accept()
		--end
		love.net.close()
	end

end
