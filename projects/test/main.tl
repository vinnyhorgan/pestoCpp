pesto.networking.connect("127.0.0.1")

local toSend = pesto.data.newData({ 1004, 0, 4 })

global function update(dt: number)
    dt = dt + 1

    if pesto.keyboard.isPressed(Key.Space) then
        pesto.networking.sendReliable(toSend)
    end
end

global function draw()
    pesto.graphics.print("Finally, hello graphics...", 100, 100)
end

global function packetreceived(data: Data)
    print(data:getData()[3])
end

