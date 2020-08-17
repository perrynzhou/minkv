local socket = require("socket")
local host,port = "127.0.0.1",9090
local tcp = assert(socket.tcp())

for i=1,10000 do
    consumer = coroutine.create(function () 
        tcp::connect(host,port)
        local message = "thread "..i
        tcp:send(message)
        tcp:close()
    end)
    coroutine.resume(consumer)
end
