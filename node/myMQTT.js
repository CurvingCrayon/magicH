//Provides a web interface to use MQTT
var mq = require("mqtt");
var client  = mq.connect('mqtt://broker.hivemq.com:1883');
var connected = false;
client.on('connect', function () {
    connected = true;
    console.log("MQTT connected");
});
exports.message = function(topic, message, res){
    if(connected){
        client.subscribe(topic, function (err) {
            if (!err) {
                client.publish(topic, message,{qos:2},function(err){
                    if(!err){
                        console.log("successful publish! "+topic);
                    }
                    else{
                        console.log("unsuccessful publish");
                        console.log(err);
                    }
                });
                if(topic === "lozrgb"){
                    var newMsg = message.split(",");
                    newMsg.pop();
                    newMsg = newMsg.join(",") + ")";
                    res.send("success: <p style='color:rgba"+newMsg+"'>"+topic+","+newMsg+"</p>");
                    return "success";
                }
                else{
                    res.send("success: "+topic+","+newMsg);
                    return "success";
                }
                
            }
            else{
                console.log("MQTT error");
                res.send("MQTT error "+err);
                return err;
            }
        });
    }
    else{
        console.log("Connection attempted prior to mqtt init");
        res.send("MQTT not initialized");
        return "Not yet connected to mqtt server";
    }
}