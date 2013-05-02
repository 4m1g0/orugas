var horizon, compass, map, orugas, joystick;

var global = new function () {
    this.debug = 1;
    
    this.isDebug = function () {
        return this.debug;
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

function getOrugasData(data) {
    if (data.t == 0) // command response
    {
        // "{\"t\":0,\"m\":\"Pin 3 = HIGH\"}"
        $('#orugas_terminal').terminal().echo('[' + get_hour() + '] ' + data.m);
        return;
    }
    
    if (data.t == 1) // error
    {
        // "{\"t\":1,\"m\":\"Error numero 23\"}"
        $('#orugas_terminal').terminal().error('[' + get_hour() + '] ' + data.m);
        return;
    }
    
    if (data.t == 2) // position data 
    {
        // "{\"t\":2,\"p\":[120,45],\"l\":[134.543,-8.432,134],\"d\":23}" -> p:[x,y] ll[latitud,longitud,orientacion]
        // "{"t":2,"o":[120,45],"g":[134.543,-8.432,134],"d":23}"
        horizon.setHeight(data.o[1]);
        horizon.setRotation(data.o[0]);
	    horizon.update();
	    
	    compass.setRotation(data.g[2]);
	    compass.update();
	    
	    map.moveMarker(data.g[0], data.g[1]);
	    if (global.isDebug())
	        $('#orugas_terminal').terminal().echo(
	            '[orugas]: p = ' + data.o[1] + ',' + data.o[0] + ' l = ' + data.g[0] + ',' + data.g[1] + ',' + data.g[2] + ' d = ' + data.d);
    }
    
    if (data.t == 3) // enviroment data 
    {
        // "{\"t\":3,\"l\":80,\"C\":23}" l = luz, tem = temperatura
        if (global.isDebug())
            $('#orugas_terminal').terminal().echo('[orugas]: luz = ' + data.l + ' temp = ' + data.C);
    }
    
    if (data.t == 4) // ack
    {
        // TODO:
    }
}

function getJoystickData(data) {
    if (global.isDebug())
        $('#orugas_terminal').terminal().echo('[Joystick]: x = ' + data.x + '  y = ' + data.y + '  b = ' + data.b);
    
    
    
    orugas.emit("sendData", data.x + ',' + data.y + '\n');
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



