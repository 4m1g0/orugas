var horizon, compass, map, orugas, joystick, buttons, lastPulse;

var global = new function () {
    this.debug = 1;
    
    this.isDebug = function () {
        return this.debug;
    }
}

function getLeter(x, on) {
    switch (x) {
        case 1: return on?'':'';
        case 2: return on?'':'';
        case 4: return on?'':'';
        case 8: return on?'':'';
        case 16: return on?'':'';
        case 32: return on?'':'';
        case 64: return on?'':'';
        case 128: return on?'':'';
        case 512: return on?'':'';
        case 1024: return on?'':'';
        //TODO: Esta funcion debe devolver la letra asignada al boton X (ver codigo de arduino Serie.ino:8)
    
    }
} 

function Horizon(element) {
    this.height = 0.0;
    this.rotation = 0.0;
    this._element = element;
    this.setHeight = function (height) {
        this.height = -height;
    };
    this.setRotation = function (rotation) {
        this.rotation = -rotation;
    };
    this.update = function () {
        this._element.style.webkitTransform= "rotate(" + this.rotation + "deg)";
        this._element.style.transform= "rotate(" + this.rotation + "deg)";
        this._element.style.backgroundPosition=" -65px "+ (-65-this.height) +"px";
    };
}

function Compass(element) {
    this.rotation = 0.0;
    this._element = element;
    this.setRotation = function (rotation) {
        this.rotation = rotation;
    };
    this.update = function () {
        this._element.style.transform= "rotate(" + this.rotation + "deg)";
        this._element.style.webkitTransform= "rotate(" + this.rotation + "deg)";
    };
}

function Map(element) {
    this.x = 43.362138;
    this.y = -8.412518;
    this.centerX = this.x;
    this.centerY = this.x;
    
    this.mapOptions = {
        zoom: 17,
        center: new google.maps.LatLng(this.x, this.y),
        mapTypeId: google.maps.MapTypeId.HYBRID
    };
    this.map = new google.maps.Map(element, this.mapOptions);
    
    this.image = 'images/centro_brujula.png';
    this.marker = new google.maps.Marker({
      position: new google.maps.LatLng(this.x, this.y),
      map: this.map,
      title: 'Orugas',
      /*icon: this.image*/
    });
    this.move2Marker = function() {
        this.map.panTo(new google.maps.LatLng(this.x, this.y));
        this.centerX= this.x;
        this.centerY= this.y;
    }
    this.moveMarker = function (x, y) {
        this.x = x;
        this.y = y;
        this.marker.setPosition(new google.maps.LatLng(x, y));
        
        // si el marcador se esta saliendo del mapa desplazamos el mapa
        if (Math.abs(this.x - this.centerX) > 0.0015 || Math.abs(this.y - this.centerY) > 0.0015)
            this.move2Marker();
    };
}

function Light(element) {
    this._element = element;
    this.light = 0;
    
    this.setLight = function(value) {
        this.light = value;
    }
    
    this.update = function() {
        // TODO:
    }
}

function Temperature(element) {
    this._element = element;
    this.temperature = 0;
    
    this.setTemperature = function(value) {
        this.temperature = value;
    }
    
    this.update = function() {
        // TODO:
    }
}

function Pan(element) {
    this._element = element;
    this.pan = 0;
    this.distance = 0;
    
    this.setPan = function(value) {
        this.pan = value;
    }
    
    this.setDistance = function(value) {
        this.distance = value;
    }
    
    this.update = function() {
        // TODO:
    }
}

function Tilt(element) {
    this._element = element;
    this.tilt = 0;
    this.distance = 0;
    
    this.setTilt = function(value) {
        this.tilt = value;
    }
    
    this.setDistance = function(value) {
        this.distance = value;
    }
    
    this.update = function() {
        // TODO:
    }
}

function getOrugasData(data) {
    try { // enviroment data
        light.setLight(data.e[0]);
        temperature.setTemperature(data.e[1]);
        map.moveMarker(data.e[2], data.e[3]);
        light.update();
        temperature.update();
        if (global.isDebug())
            $('#orugas_terminal').terminal().echo('[Orugas] ' + data.e[0] + ',' + data.e[1] + ',' + data.e[2] + ',' + data.e[3]);
        return;
    } catch (e) {}
    
    try { // position data
        horizon.setHeight(data.p[1]);
        horizon.setRotation(data.p[0]);
        pan.setDistance(data.p[2]);
        tilt.setDistance(data.p[2]);
	    compass.setRotation(data.p[3]);
	    horizon.update();
	    compass.update();
	    pan.update();
	    tilt.update();
	    if (global.isDebug())
	        $('#orugas_terminal').terminal().echo('[Orugas] ' + data.p[0] + ',' + data.p[1] + ',' + data.p[2] + ',' + data.p[3]);
	    return;
    } catch (e) {}
}

function getJoystickData(data) {
    if (global.isDebug())
        $('#orugas_terminal').terminal().echo('[Joystick]: x = ' + data[0] + '  y = ' + data[1] + '  b = ' + data[2]);
    
    orugas.emit("sendData", 'm' + data[0] + ',' + data[1] + '\n');
    
    // parse botons
    var changed = lastPulse ^ data[2]; // obtenemos un mapa con los bits que han cambiado
    var onOff;
    lastPulse = data[2];
    
    for (i = 1; i < 1024; i*=2) {
        if (i & changed && i & data[2]) { // si el bit i ha cambiado y vale 1
            if (buttons & i) { // si ya estaba encendido lo apago
                onOff = 0;
                buttons &= ~i;
            } else { // si estaba apagado lo enciendo
                onOff = 1;
                buttons |= i;
            }
            orugas.emit("sendData", 'b' + getLeter(i, onOff) + '\n');
        }
    }
}


function main()
{
    window.onload = function() {
        horizon = new Horizon(document.getElementById('horizon'));
        compass = new Compass(document.getElementById('compass'));
        map = new Map(document.getElementById('map-canvas'));
        
        // TODO: remove debug code
        /*setInterval("test_horizon()", 80);
        setInterval("test_compass()", 80);
        map.moveMarker(map.x+0.0011, map.y);
        map.move2Marker();*/
    };
    
    
    
    
    jQuery(function($, undefined) {
        $('#orugas_terminal').terminal({
                send: function(arg1) {
                    orugas.emit("sendData", arg1);
                },
                add: function(a, b) {
                    this.echo(parseInt(a)+parseInt(b));
                }
            }, {
            greetings: 'Bienvenido al sistema de orugas versión 0.1.3.',
            name: 'Orugas terminal',
            height: 200,
            prompt: '> '});
    });
    
    // open a connection to the serial server:
	orugas = io.connect('http://localhost:8080');
	joystick = io.connect('http://localhost:8081');

	 // when you get a serialdata event, do this:
	orugas.on('serialEvent', getOrugasData);
	joystick.on('serialEvent', getJoystickData);
}


/* A partir de aqui es codigo de debug */

var giro=0,
gstep=1,
altura=0,
astep=8,
giro2 = 0;

function test_horizon()
{
	

	giro += gstep;

    if (giro >= 360)
		giro = 0;

	if (giro <= -360)
		giro = -0;
	

	altura += astep;
	if (altura >= 380)
		altura = 0;

	if (altura <= -378)
		altura = 0;

    horizon.setHeight(altura);
    horizon.setRotation(giro);
	horizon.update();

}

function test_compass()
{
	if (giro2 >= 360)
	    giro2 = 0;
	    
	compass.setRotation(giro2);
	compass.update();
	
	giro2 += 2;
}

function get_hour()
{
    var todayDate=new Date();
    var hours=todayDate.getHours();
    var minutes=todayDate.getMinutes();
    var seconds=todayDate.getSeconds();
    
    return hours + ':' + minutes + ':' + seconds;
}



