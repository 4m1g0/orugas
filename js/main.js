var horizon, compass, map, orugas, joystick, buttons, lastPulse, temperature, luminosity, pan, tilt;

var global = new function () {
    this.debug = 1;
    
    this.isDebug = function () {
        return this.debug;
    }
}

function getLeter(x, on) {
    switch (x) {
        case 1: return on?'L':'l';
        case 2: return on?'F':'f';
        case 4: return on?'P':'P';
        case 8: return on?'p':'p';
        case 16: return on?'T':'T';
        case 32: return on?'t':'t';
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

function Luminosity(element, luminosityV) {
    this._element = element;
    this._luminosityV = luminosityV
    this.luminosity = 0;
    
    this.setLuminosity = function(value) {
        this.luminosity = value;
    }
    
    this.update = function() {
        this._element.style.backgroundPosition="0px "+ (200-this.luminosity*2) +"px";
        this._luminosityV.innerHTML = this.luminosity;
    }
}

function Temperature(element, temperatureV) {
    this._element = element;
    this._temperatureV = temperatureV;
    this.temperature = 0;
    
    this.setTemperature = function(value) {
        if (value > 40) value = 40;
        if (value < -5) value = -5;
        this.temperature = value;
    }
    
    this.update = function() {
        this._element.style.backgroundPosition="0px "+ (130-this.temperature*2.6) +"px";
        this._temperatureV.innerHTML = this.temperature;
    }
}

function Pan(element, distanceV) {
    this._element = element;
    this._distanceV = distanceV;
    this.pan = 0;
    this.distance = 0;
    
    this.setPan = function(value) {
        if (value > 90) value = 90;
        if (value < -90) value = -90;
        this.pan = value;
    }
    
    this.setDistance = function(value) {
        if (value > 1000) value = 1000;
        if (value < 0) value = 0;
        this.distance = value;
    }
    
    this.update = function() {
        this._element.style.webkitTransform= "rotate(" + this.pan + "deg)";
        this._element.style.backgroundPosition="0px "+ -(this.distance/10) +"px";
        this._distanceV.innerHTML = this.distance;
    }
}

function Tilt(element) {
    this._element = element;
    this.tilt = 0;
    this.distance = 0;
    
    this.setTilt = function(value) {
        if (value > 90) value = 90;
        if (value < -90) value = -90;
        this.tilt = value;
    }
    
    this.setDistance = function(value) {
        if (value > 1000) value = 1000;
        if (value < 0) value = 0;
        this.distance = value;
    }
    
    this.update = function() {
        this._element.style.backgroundPosition=-100+(this.distance/10) +"px 0px";
        this._element.style.webkitTransform= "rotate(" + this.tilt + "deg)";
    }
}

function getData(data) {
   if (data.e != undefined) {
        luminosity.setLuminosity(data.e[0]);
        temperature.setTemperature(data.e[1]);
        map.moveMarker(data.e[2], data.e[3]);
        luminosity.update();
        temperature.update();
        if (global.isDebug())
            $('#orugas_terminal').terminal().echo('[Orugas] ' + data.e[0] + ',' + data.e[1] + ',' + data.e[2] + ',' + data.e[3]);
        return;
    } else if (data.p != undefined) {
        pan.setPan(data.p[0]);
        tilt.setTilt(data.p[1])
        horizon.setHeight(data.p[2]);
        horizon.setRotation(data.p[3]);
        pan.setDistance(data.p[4]);
        tilt.setDistance(data.p[5]);
	    compass.setRotation(data.p[6]);
	    horizon.update();
	    compass.update();
	    pan.update();
	    tilt.update();
	    if (global.isDebug())
	        $('#orugas_terminal').terminal().echo('[Orugas] ' + data.p[0] + ',' + data.p[1] + ',' + data.p[2] + ',' + data.p[3]);
	    return;
    }
}

function main()
{
    window.onload = function() {
        horizon = new Horizon(document.getElementById('horizon'));
        compass = new Compass(document.getElementById('compass'));
        map = new Map(document.getElementById('map-canvas'));
        temperature = new Temperature(document.getElementById('termometer'), document.getElementById('temperature_value'));
        luminosity = new Luminosity(document.getElementById('luminosity'), document.getElementById('luminosity_value'));
        pan = new Pan(document.getElementById('pan'), document.getElementById('distance_value'));
        tilt = new Tilt(document.getElementById('tilt'));
        
        
        /*pan.setDistance(400);
        pan.setPan(45);
        pan.update();
        /*temperature.setTemperature(30);
        temperature.update();
        
        luminosity.setLuminosity(30);
        luminosity.update();
        /*
        // TODO: remove debug code
        setInterval("test_horizon()", 80);
        setInterval("test_compass()", 80);
        map.moveMarker(map.x+0.0011, map.y);
        map.move2Marker();*/
    };
    
    
    
    
    jQuery(function($, undefined) {
        $('#orugas_terminal').terminal({
                send: function(arg1) {
                    joystick.emit("sendData", arg1);
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
	joystick = io.connect('http://localhost:8081');

	 // when you get a serialdata event, do this:
	joystick.on('serialEvent', getData);
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



