var express = require("express");
var app = express();
var type = "rgb";
var cmd = "(255,0,0)"; //Current lighting command
var cmdSeen = false; //Whether the node has seen the latest command
var mq = require("./myMQTT.js");
var m = require("./mosca.js");
var numPole = 0;


app.get("/",function(req,res){
    if(cmdSeen){
        res.send("X: No new data");
    }else{
        cmdSeen = true;
        res.send(type+"."+cmd);
    }
    console.log("Receive request initiated ("+String(numPole)+"), "+(cmdSeen?"seen":"false"));
    numPole++;
    // var newPath = __dirname.split("\\");
    // newPath.pop();
    // newPath = newPath.join("/");
    // newPath += "/";
    // newPath = "."+newPath;
    // var fileOptions = {
    //     root: newPath,
    //     dotfiles: 'deny',
    //     headers: {
    //         'x-timestamp': Date.now(),
    //         'x-sent': true
    //     }
    // }
    // res.sendFile("index.html",fileOptions,function(){
        
    // })
    // console.log("Index loaded");
}).get("/mqtt/:type/:cmd",function(req, res){
    console.log(req.params.type+"."+req.params.cmd);
    if(req.params.cmd !== cmd || req.params.type !== type){
        cmd = req.params.cmd;
        type = req.params.type;
        cmdSeen = false;
        mq.message(type, cmd, res);
    }
    else{
    res.send(req.params.type+"."+req.params.cmd);
    }
}).get("/receive",function(req,res){
    if(cmdSeen){
        res.send("X: No new data");
    }else{
        cmdSeen = true;
        res.send(type+"."+cmd);
    }
    numPole = numPole % 1000;
    console.log("Receive request initiated ("+String(numPole)+"), "+(cmdSeen?"seen":"false"));
    numPole++;
}).get("/falsereceive",function(req,res){
    res.send(type+"."+cmd);

}).get("/log/:message",function(req,res){

});
app.listen(8000);