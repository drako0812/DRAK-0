x = 320 / 2
y = 240 / 2

function update()
    if btn(0) then y = y - 5 end
    if btn(1) then y = y + 5 end
    if btn(2) then x = x - 5 end
    if btn(3) then x = x + 5 end

    cls()
    spr(sidx(3, 0, 1), x, y, 0)
end
