//Provides a web interface to use MQTT
var mq = require("mqtt");
var client  = mq.connect('mqtt://127.0.0.1:1883');
var connected = false;
client.on('connect', function () {
    connected = true;
});
exports.message = function(topic, message, res){
    if(connected){
        client.subscribe(topic, function (err) {
            if (!err) {
                client.publish(topic, message);
                if(topic === "instant"){
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