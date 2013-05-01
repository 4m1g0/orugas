var port_name = process.argv[2];

if (port_name == undefined) {
    console.log("Serial port name is required.");
    process.exit(1);
}

var serial = require("serialport"),
    port = new serial.SerialPort(port_name, {
        parser: serial.parsers.readline("\r\n"),
        baudrate: 115200
    });
console.log("Opening serial port " + port_name);

var app = require("express")(),
    server = require("http").createServer(app),
    io = require("socket.io").listen(server);

server.listen(8081);
console.log("Listening for new clients on http://localhost:8081");

io.sockets.on("connection", function(socket) {
    port.on("data", function(data) {
        try {
            var serial_data = JSON.parse(data);
        } catch(e) {
            return; // Malformed JSON string. Ignore.
        }
        socket.emit("serialEvent", serial_data);
    });
});
