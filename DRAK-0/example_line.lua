pi8 = math.pi / 8
pi2 = math.pi * 2

t = 0
function update()
    cls()

    for i = t%8, 135, 8 do
        line(i, 0, 0, 135-i, rgb(1,2,3))
        line(i, 135, 135, 135-i, rgb(3,1,1))
        t = t + 0.01
    end

    for i = t / 16 % pi8, pi2, pi8 do
        x = 68 + 32 * math.cos(i)
        y = 68 + 32 * math.cos(i)
        line(135, 0, x, y, rgb(3, 3, 3))
        line(0, 135, x, y, rgb(3, 3, 3))
    end

    line(0, 0, 135, 0, rgb(1, 2, 3))
    line(0, 0, 0, 135, rgb(1, 2, 3))
    line(135, 0, 135, 135, rgb(3, 1, 1))
    line(0, 135, 135, 135, rgb(3, 1, 1))
end
