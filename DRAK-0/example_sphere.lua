a={}
v={}
x={}
--c={}
r=118
number_of_points = 3600
for i=1,number_of_points do a[i]=random(360) end
for i=1,number_of_points do v[i]=random((-1*r),r)end
for i=1,number_of_points do x[i]=math.sqrt(r*r-v[i]*v[i])end
--for i=1,number_of_points do c[i]=rgb(3, 3, 3) end

function update()
    cls(0)
    for i=1,number_of_points do
        f=math.cos(a[i]*math.pi/180)
        if a[i]>180 then
            --pix((x[i]*f)+160,120+v[i],3)
            local c = rgb(3, 3, 3)
            if((a[i] > 180) and (a[i] <= 180 + 36)) then
                c = rgb(1, 1, 1)
            elseif((a[i] > 180 + 36) and (a[i] <= 180 + 36 * 2)) then
                c = rgb(2, 2, 2)
            elseif((a[i] > 180 + 36 * 2) and (a[i] <= 180 + 36 * 3)) then
                c = rgb(3, 3, 3)
            elseif((a[i] > 180 + 36 * 3) and (a[i] <= 180 + 36 * 4))  then
                c = rgb(2, 2, 2)
            elseif((a[i] > 180 + 36 * 4) and (a[i] <= 180 + 36 * 5)) then
                c = rgb(1, 1, 1)
            end
            --circb((x[i]*f)+160,120+v[i],4,c)
            pix((x[i]*f)+160,120+v[i],c)
        end
        a[i]=a[i]+1
        if a[i]>360 then a[i]=a[i] % 360 end
    end

end
