x = 120
y = 68
dx = 7
dy = 4
col = 1

cls()
function update()
    x = x + dx
    y = y + dy
    if x > 240 - 6 or x < 0 then
        dx = -dx
        col = col % 63 + 1
    end
    if y > 136 - 6 or y < 0 then
        dy = -dy
        col = col % 63 + 1
    end
    rect(x, y, 6, 6, col) 
end
