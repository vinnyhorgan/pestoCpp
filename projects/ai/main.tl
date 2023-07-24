local worldSize = 50
local startX: number = 0
local startY: number = 0
local endX: number = 0
local endY: number = 0

local path: {Vector} = {}
local collisions: {Vector} = {}

pesto.ai.setWorldSize(worldSize, worldSize)
pesto.ai.setDiagonalMovement(false)

local function pixelToTile(x: number, y: number): number, number
    return math.floor(x / 20), math.floor(y / 20)
end

global function update(dt: number)
    if pesto.mouse.isPressed(Button.Left) then
        local x, y = pixelToTile(pesto.mouse.getX(), pesto.mouse.getY())

        startX = x
        startY = y
    end

    if pesto.mouse.isPressed(Button.Right) then
        local x, y = pixelToTile(pesto.mouse.getX(), pesto.mouse.getY())

        endX = x
        endY = y
    end

    if pesto.mouse.isPressed(Button.Middle) then
        local x, y = pixelToTile(pesto.mouse.getX(), pesto.mouse.getY())

        pesto.ai.addCollision(x, y)

        table.insert(collisions, {x = x, y = y})
    end

    path = pesto.ai.findPath(startX, startY, endX, endY)
end

global function draw()
    for x = 0, worldSize do
        for y = 0, worldSize do
            pesto.graphics.setColor(255, 255, 255)
            pesto.graphics.rectangle(DrawMode.Fill, x * 20, y * 20, 20, 20)

            pesto.graphics.setColor(0, 0, 0)
            pesto.graphics.rectangle(DrawMode.Line, x * 20, y * 20, 20, 20)

            for _, node in ipairs(path) do
                if x == node.x and y == node.y then
                    pesto.graphics.setColor(0, 0, 255)
                    pesto.graphics.rectangle(DrawMode.Fill, x * 20, y * 20, 20, 20)
                end
            end

            for _, node in ipairs(collisions) do
                if x == node.x and y == node.y then
                    pesto.graphics.setColor(255, 0, 255)
                    pesto.graphics.rectangle(DrawMode.Fill, x * 20, y * 20, 20, 20)
                end
            end

            if x == startX and y == startY then
                pesto.graphics.setColor(0, 255, 0)
                pesto.graphics.rectangle(DrawMode.Fill, x * 20, y * 20, 20, 20)
            end

            if x == endX and y == endY then
                pesto.graphics.setColor(255, 0, 0)
                pesto.graphics.rectangle(DrawMode.Fill, x * 20, y * 20, 20, 20)
            end
        end
    end
end
