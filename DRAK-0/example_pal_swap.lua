for j=0, 239, 1 do
    for i=0, 319, 1 do
        local c = random(0, 64)
        pix(i, j, c)
    end
end

pal = 0x12C00
pal_count = 64
red = 0
green = 1
blue = 2
offset = 3

t = time()

palette = { }

for i=1, pal_count, 1 do
    local r = peek(pal + (offset * i) + red)
    local g = peek(pal + (offset * i) + green)
    local b = peek(pal + (offset * i) + blue)
    palette[i] = { r, g, b }
end

function update()
    --trace("0")
    t = time()
    --trace("0.5")
    for i=0, pal_count - 1, 1 do
        --trace("1")
        local r = palette[i+1][1] --* math.floor(((math.sin(((math.pi() * 2) / 1000) * t) + 1) / 2))
        local g = palette[i+1][2] --* math.floor(((math.sin(((math.pi() * 1.9) / 1000) * t) + 1) / 2))
        local b = palette[i+1][3] --* math.floor(((math.sin(((math.pi() * 1.8) / 1000) * t) + 1) / 2))
        --trace("2")
        local vr = (math.sin(((math.pi * 2) / 1000) * t) + 1) / 2
        local vg = (math.sin(((math.pi * 2) / 1333) * t) + 1) / 2
        local vb = (math.sin(((math.pi * 2) / 1476) * t) + 1) / 2
        --trace("4")
        r = math.floor(r * vr)
        g = math.floor(g * vg)
        b = math.floor(b * vb)
        --if i == 3 then
        --    trace("("..tostring(vr)..", "..tostring(vg)..", "..tostring(vb)..") ("..tostring(r)..", "..tostring(g)..", "..tostring(b)..")")
        --end
        --trace("5")
        poke(pal + (offset * i) + red, r)
        poke(pal + (offset * i) + green, g)
        poke(pal + (offset * i) + blue, b)
    end
end
