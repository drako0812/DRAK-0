a = 0
space = 10
function update()
    cls()
    for i = 0, 200, space do
        circb(120 + 80 * math.sin(a),
            68 + 40 * math.cos(a),
            i + time() / 40 % space,
            43)
        circb(120 + 80 * math.sin(a / 2),
            68 + 40 * math.cos(a / 2),
            i + time() / 40 % space,
            43)
    end
    a = a + math.pi / 240
end
