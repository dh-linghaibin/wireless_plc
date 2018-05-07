

ltime.add(2000,function ()
    print("time..")
end)

levent.add(device.DI_4,20,0,0,function ()
    print("event..")
end)

while true do
    equipment.equ_delay(1000)
end


