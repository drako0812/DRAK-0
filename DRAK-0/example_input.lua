function update()
  cls(0)
  for i=0, 19, 1 do
    if btn(i) then pix(1 + (2 * i), 1, 3)
    else pix(1 + (2 * i), 1, 1) end
  end
  for i=0, 3, 1 do
    if mbtn(i) then pix(1 + (2 * i), 3, 3)
    else pix(1 + (2 * i), 3, 1) end
  end

  pix(mpos(0), mpos(1), 3)
end
