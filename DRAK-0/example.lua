trace("example.lua")
trace("Starting Up...")

function update()
  for j=0, 239, 1 do
    for i=0, 319, 1 do
      --local c = random(0, 64)
      pix(i, j, random(0, 64))
      --pix(i, j, rgb(random(0, 3), random(0, 3), random(0, 3)))
    end
  end
end
