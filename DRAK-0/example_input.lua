function update()
  cls(0)
  for i=0, 19, 1 do
    if btn(i) then pix(1 + (2 * i), 1, rgb(3, 3, 0))
    else pix(1 + (2 * i), 1, rgb(1, 1, 0)) end
  end
  for i=0, 3, 1 do
    if mbtn(i) then pix(1 + (2 * i), 3, rgb(3, 3, 0))
    else pix(1 + (2 * i), 3, rgb(1, 1, 0)) end
  end

  pix(mpos(0), mpos(1), rgb(3, 3, 3))
end
