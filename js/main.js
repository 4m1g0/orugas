function horizon_move(giro, altura)
{
   document.getElementById('horizon').style.webkitTransform= "rotate("+giro+"deg)";
   document.getElementById('horizon').style.transform= "rotate("+giro+"deg)";
   document.getElementById('horizon').style.backgroundPosition=" -65px "+ (-65-altura) +"px";
}

function compass_move(giro)
{
    document.getElementById('compass').style.transform= "rotate("+giro+"deg)";
    document.getElementById('compass').style.webkitTransform= "rotate("+giro+"deg)";
}

giro=0;
gstep=1;
altura=0;
astep=8;
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

	horizon_move(giro, altura);

}

function test_compass()
{
	if (giro2 >= 360)
	    giro2 = 0;
	    
	compass_move(giro2);
	giro2 += 2;
}


function main()
{
    setInterval("test_horizon()", 40);
    setInterval("test_compass()", 40);
}
