local player = require("player")

-- local otherPlayers = {}

local skiman = pesto.graphics.newImage("assets/skiman.png")

local laser = pesto.audio.newSource("assets/laser.ogg", SourceType.Static)
laser:setMaxDistance(400)

global function update(dt: number)
    if pesto.keyboard.isDown(Key.W) then
        player.y = player.y - player.speed * dt
    elseif pesto.keyboard.isDown(Key.S) then
        player.y = player.y + player.speed * dt
    end

    if pesto.keyboard.isDown(Key.A) then
        player.x = player.x - player.speed * dt
    elseif pesto.keyboard.isDown(Key.D) then
        player.x = player.x + player.speed * dt
    end

    if pesto.mouse.isPressed(Button.Left) then
        laser:setVolume(1.0)
        laser:setPan(0.5)
        laser:play()
    end

    if pesto.mouse.isPressed(Button.Right) then
        laser:playSpatial(player.x, player.y)
    end
end

global function draw()
    pesto.graphics.circle(DrawMode.Line, player.x, player.y, 25)
    pesto.graphics.print(player.name, player.x - 15, player.y - 50)

    pesto.graphics.draw(skiman, 100, 100)
end

global function packetreceived(data: Data)
    print(data:type())
end
