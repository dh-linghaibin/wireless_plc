
while ture do
    
    local num = tonumber("0x" .. string.sub(equipment.re485_read(), 0, 1)) -- 获取第1个hex 转化为 数字 第2个就是（2 - 3）以此类推
    equipment.holeing_set(0,num) -- 保存到地址0
    
    -- 更简洁的写法
    equipment.holeing_set(0,tonumber("0x" .. string.sub(equipment.re485_read(), 0, 1)))
    
end 