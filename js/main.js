var horizon, compass, map;

function Horizon(element) {
    this.height = 0.0;
    this.rotation = 0.0;
    this._element = element;
    this.setHeight = function (height) {
        this.height = height;
    };
    this.setRotation = function (rotation) {
        this.rotation = rotation;
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
    this.mapOptions = {
        zoom: 17,
        center: new google.maps.LatLng(43.362138, -8.412518),
        mapTypeId: google.maps.MapTypeId.HYBRID
    };
    this.map = new google.maps.Map(element, this.mapOptions);
}

function getData(data) {
    if (data.type == 'command_reply')
    {
        $('#orugas_terminal').terminal().echo('[' + get_hour() + '] ' + data.text);
        return;
    }
    
    if (data.type == 'error')
    {
        $('#orugas_terminal').terminal().error('[' + get_hour() + '] ' + data.text);
        return;
    }
    
    if (data.type == 'data')
    {
        // TODO: pharse data string
    }
}


function main()
{
    window.onload = function() {
        horizon = new Horizon(document.getElementById('horizon'));
        compass = new Compass(document.getElementById('compass'));
        map = new Map(document.getElementById('map-canvas'));
        
        // TODO: remove debug code
        setInterval("test_horizon()", 80);
        setInterval("test_compass()", 80);
    };
    
    jQuery(function($, undefined) {
        $('#orugas_terminal').terminal(function(command, term) {
            if (command != '')
                term.echo('Recibido ' + command);
            if (command == 'error')
                term.error('test ');
        }, {
            greetings: 'Bienvenido al sistema de orugas versión 0.1.3.',
            name: 'Orugas terminal',
            height: 200,
            prompt: '> '});
    });
    
    // open a connection to the serial server:
	var socket = io.connect('http://localhost:8080');

	 // when you get a serialdata event, do this:
	socket.on('serialEvent', getData(data));
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



