local ballPositionX = -100.0
local ballRadius = 20.0
local ballAlpha = 0.0

local state = 0
local timer = 0.0

global function update(dt: number)
    timer = timer + dt

    if state == 0 then
        ballPositionX = pesto.timer.tween(timer, -100, 400, 2, TweenType.ElasticOut)

        if timer > 2 then
            timer = 0
            state = 1
        end
    elseif state == 1 then
        ballRadius = pesto.timer.tween(timer, 20, 520, 3.3, TweenType.ElasticIn)

        if timer > 3.3 then
            timer = 0
            state = 2
        end
    elseif state == 2 then
        ballAlpha = pesto.timer.tween(timer, 0, 1, 3.3, TweenType.CubicOut)

        if timer > 3.3 then
            timer = 0
            state = 3
        end
    elseif state == 3 then
        if pesto.keyboard.isPressed(Key.Space) then
            timer = 0;
            ballPositionX = -100;
            ballRadius = 20;
            ballAlpha = 0;
            state = 0;
        end
    end
end

global function draw()
    pesto.graphics.clear(255, 255, 255)

    pesto.graphics.print("Ball position: " .. ballPositionX, 10, 10)
    pesto.graphics.print("Ball radius: " .. ballRadius, 10, 40)
    pesto.graphics.print("Ball alpha: " .. ballAlpha, 10, 70)
    pesto.graphics.print("Timer: " .. timer, 10, 100)

    if state >= 2 then
        pesto.graphics.setColor(0, 255, 0)
        pesto.graphics.rectangle(DrawMode.Fill, 0, 0, 800, 600)
        pesto.graphics.setColor(0, 0, 0)
    end

    pesto.graphics.circle(DrawMode.Fill, ballPositionX, 300, ballRadius)

    if state == 3 then
        pesto.graphics.setColor(255, 255, 255)
        pesto.graphics.print("Press space to restart", 10, 10)
        pesto.graphics.setColor(0, 0, 0)
    end

    pesto.graphics.setColor(0, 0, 0)
end

