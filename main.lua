
print('start') --只执行一次


ltime.add(2000,function ()
    print("年"..time.year.."月"..time.month.."日"..time.date.."时"..time.hour.."分"..time.min.."秒"..time.sec)
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


while true do --循环
    equipment.delay(1000) --延时1s   输入参数单位为ms
    -- equipment.coils_set(7,8,0) --设置DO-8 地址7 所有的继电器关闭
    -- equipment.coils_set(7,8,1) --设置DO-8 地址7 所有的继电器打开
    -- equipment.coils_set(7,1,1) --设置DO-8 地址7 第二个继电器打开
    -- cal = equipment.coils_get(7,1) --获取DO-8 地址7 的第一个IO口状态
    -- time = ltime.time() --获取系统时间 time 是一个table表
    -- print("年"..time.year.."月"..time.month.."日"..time.date.."时"..time.hour.."分"..time.min.."秒"..time.sec)
    -- if( equipment.input_get(1,0) == 1 ) then --判断DI 地址1 的第一个按钮是否按下
    --     equipment.coils_set(7,8,1)
    -- else
    --     equipment.coils_set(7,8,1)
    -- end
end


