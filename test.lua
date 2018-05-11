
print("123")

ii = 0;

ltime.add(2000,function () --的速度得到
    print("\f21time.."..ii)
    ii = ii+1
end)

levent.add(device.DI_4,20,0,0,function ()
    print("event..1")
    equipment.coils_set(7,8,0)
end)

levent.add(device.DI_4,20,0,1,function ()
    print("event..2")
    equipment.coils_set(7,8,255)
end)

levent.add(device.DI_4,20,1,0,function ()
    print("event..3")
    equipment.coils_set(7,0,0)
end)

levent.add(device.DI_4,20,1,1,function ()
    print("event..4")
    equipment.coils_set(7,0,1)
end)

while true do
    equipment.delay(1000)
end


