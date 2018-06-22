log = require "log"

file = io.open("test.lua", "r")
x = 300

log.trace("linghibin"..x)
log.debug("12")
log.info(...)
log.warn(...)
log.error(...)
log.fatal(...)

a={"Hello","World";a=1,b=2,z=3,x=10,y=20;"Good","Bye"}  

for i, v in ipairs(a) do  
    log.error(v..i.."---linghaibin")  
end   

