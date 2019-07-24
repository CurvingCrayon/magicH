var m = require("mosca");

  
  var settings = {
    port: 1883,
  };
  
  var server = new m.Server(settings);
  
  server.on('clientConnected', function(client) {
      console.log('client connected', client.id);
  });
  
  // fired when a message is received
  server.on('published', function(packet, client) {
      var pck = packet.payload;
      if(typeof(pck)==="object"){
        console.log('Published', pck.toString());
      }
      else{
        console.log(pck);
      }
    
  });
  
  server.on('ready', setup);
  
  // fired when the mqtt server is ready
  function setup() {
    console.log('Mosca server is up and running');
  }