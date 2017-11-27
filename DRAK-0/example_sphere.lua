a={}
v={}
x={}
c={}
r=118
number_of_points = 3600
for i=1,number_of_points do a[i]=random(360) end
for i=1,number_of_points do v[i]=random((-1*r),r)end
for i=1,number_of_points do x[i]=math.sqrt(r*r-v[i]*v[i])end
for i=1,number_of_points do c[i]=random(1,64) end

function update()
    cls(0)
    for i=1,number_of_points do
        f=math.cos(a[i]*math.pi/180)
        if a[i]>180 then
            --pix((x[i]*f)+160,120+v[i],3)
            circb((x[i]*f)+160,120+v[i],4,c[i])
        end
        a[i]=a[i]+1
        if a[i]>360 then a[i]=a[i] % 360 end
    end

end
