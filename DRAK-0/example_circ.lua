balls = {}
d = 1
for i = 0, 50 do
    ball = {
        x = random(10, 220),
        y = random(10, 126),
        dx = random(1, 2) * d,
        dy = random(1, 2) * d,
        r = random(6, 12),
        c = math.random(1, 6)
    }
    balls[i] = ball
    d = d * -1
end

function update()
    cls()
    for k, b in pairs(balls) do
        b.x = b.x + b.dx
        b.y = b.y + b.dy

        if b.x >= 240 - b.r then
            b.x = 240 - b.r - 1
            b.dx = -b.dx
        elseif b.x < b.r then
            b.x = b.r
            b.dx = -b.dx
        end

        if b.y >= 136 - b.r then
            b.y = 136 - b.r - 1
            b.dy = -b.dy
        elseif b.y < b.r then
            b.y = b.r
            b.dy = -b.dy
        end

        circ(b.x, b.y, b.r, b.c)
        circ(b.x + b.r / 4, b.y - b.r / 4, b.r / 4, b.c + 7)
    end
end
