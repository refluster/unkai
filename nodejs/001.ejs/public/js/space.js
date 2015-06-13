var brightness = [];

for (var i = 0; i < 15; i++) {
	brightness[i] = 0;
}

socket.on('connect', function(msg) {
	console.log('connect');
	tlc5940_update();
});

function tlc5940_update() {
	socket.emit('led/set', {brightness: brightness});
	console.log('led/set ' + brightness);
}

Star = function(elem) {
	this.elem = elem;
};
Star.prototype.setSize = function(size) {
	this.elem.css("height", size + "px");
	this.elem.css("width", size + "px");
	this.size = size;
};
Star.prototype.setPosition = function(x, y) {
	this.elem.css("transform", "translate(" + (x - this.size/2) + "px, " + (y - this.size/2) + "px)");
};

////////////////////////////////////////////////////////////
Space = function(pageTransition) {
	this.colorTab = [
		{rad: -Math.PI*1   , rgb: [
			{r: 4095, g: 3261, b: 2784},
			{r: 4095, g: 3391, b: 2784},
			{r: 4095, g: 3261, b: 2633},
			{r: 4095, g: 4095, b: 2546},
			{r: 4095, g: 3521, b: 2568}]},
		{rad: -Math.PI*0.75, rgb: [
			{r:  834, g:  314, b:  791},
			{r: 1007, g:  487, b:  379},
			{r: 1809, g: 1246, b: 2004},
			{r: 4095, g: 3261, b: 2676},
			{r: 4095, g: 2871, b: 1939}]},
		{rad: -Math.PI*0.5 , rgb: [
			{r:  487, g:  141, b:  271},
			{r:  119, g:  184, b:  206},
			{r: 1268, g: 1181, b: 1289},
			{r:    0, g:    0, b:  227},
			{r:    0, g:   54, b:   97}]},
		{rad: -Math.PI*0.25, rgb: [
			{r:    0, g:   11, b:  401},
			{r:    0, g:    0, b:  249},
			{r:    0, g:    0, b:   54},
			{r:    0, g:    0, b:    0},
			{r:   11, g:   32, b:   54}]},
		{rad:  0           , rgb: [
			{r:    0, g:    0, b:    0},
			{r:    0, g:    0, b:    0},
			{r:    0, g:    0, b:    0},
			{r:    0, g:    0, b:    0},
			{r:    0, g:    0, b:    0}]},
		{rad:  Math.PI*0.25, rgb: [
			{r:    0, g:    0, b:   76},
			{r:    0, g:    0, b:  119},
			{r:    0, g:    0, b:   54},
			{r:    0, g:    0, b:    0},
			{r:  227, g:   54, b:    0}]},
		{rad:  Math.PI*0.5 , rgb: [
			{r:   76, g:  141, b:  466},
			{r:    0, g:  141, b:  314},
			{r: 1116, g:  856, b:  466},
			{r: 1354, g:  769, b:   11},
			{r: 1961, g:  812, b:  444}]},
		{rad:  Math.PI*0.75, rgb: [
			{r: 4095, g: 3261, b: 3369},
			{r: 4095, g: 3391, b: 3694},
			{r: 4095, g: 3261, b: 2654},
			{r: 1268, g:  726, b:   11},
			{r: 1311, g:  682, b:  444}]},
		{rad:  Math.PI*  1,  rgb: [
			{r: 4095, g: 3261, b: 2784},
			{r: 4095, g: 3391, b: 2784},
			{r: 4095, g: 3261, b: 2633},
			{r: 4095, g: 4095, b: 2546},
			{r: 4095, g: 3521, b: 2568}]},
	];
	
	this.w = $("#page-space").width();
	this.h = $("#page-space").height();
	this.cx = this.w/2;
	this.cy = this.h/2;
	this.size = 1.2 * ((this.w > this.h)? this.h: this.w);
	this.touchStartX;
	this.touchStartY;
	this.touchStartRadian;

	this.earthRadian = 0;

	this.earth = new Star($("#earth"));
	this.brightstar = new Star($("#brightstar"));
	this.darkstar = new Star($("#darkstar"));

	this.pageTransition = pageTransition;
	
	$("#space").bind('touchstart', this.inputStart.bind(this));
	$("#space").bind('touchmove', this.inputMove.bind(this));
	$("#space").bind('touchend', this.inputEnd.bind(this));
	$(".page-to-index").bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));
};
Space.prototype.inputStart = function(e) {
	this.touchStartX = e.originalEvent.touches[0].pageX;
	this.touchStartY = e.originalEvent.touches[0].pageY;
	this.touchStartRadian = this.earthRadian;
};
Space.prototype.inputMove = function(e) {
    var x = e.originalEvent.touches[0].pageX;
	var y = e.originalEvent.touches[0].pageY;

	var ax = (x - this.cx);
	var ay = (y - this.cy);
	var bx = (this.touchStartX - this.cx);
	var by = (this.touchStartY - this.cy);
	var rotX = ax*by - bx*ay;
	var rotY = ax*bx + ay*by;

	this.earthRadian = this.touchStartRadian + Math.atan2(rotX, rotY);

	if (this.earthRadian < -Math.PI) {
		this.earthRadian += 2*Math.PI;
	} else if (this.earthRadian > Math.PI) {
		this.earthRadian -= 2*Math.PI;
	}
	this.earthRotate();
	e.preventDefault();
};
Space.prototype.inputEnd = function(e) {
};
Space.prototype.setSize = function() {
	$("#space").css("height", this.size + 'px');
	$("#space").css("width", this.size + 'px');
	$("#space").css("transform", "translate(" +
					((this.w - this.size)/2) + "px, " +
					((this.h - this.size)/2) + "px)");

	this.earth.setSize(this.size / 10);
	this.brightstar.setSize(this.size / 16);
	this.darkstar.setSize(this.size / 16);
};
Space.prototype.setPosition = function() {
	this.earthRotate();
	this.brightstar.setPosition(this.size/2, this.size/2 - this.size/2*0.7);
	this.darkstar.setPosition(this.size/2, this.size/2 + this.size/2*0.7);
};
Space.prototype.earthRotate = function() {
	const ratio = 0.6;
	var x = this.size/2 + ratio*this.size/2*Math.sin(this.earthRadian);
	var y = this.size/2 + ratio*this.size/2*Math.cos(this.earthRadian);
	this.earth.setPosition(x, y);

	for (var i = 0; i < this.colorTab.length; i++) {
		if (this.colorTab[i].rad > this.earthRadian) {
			var a = this.colorTab[i - 1];
			var b = this.colorTab[i];
			var ratioA = (b.rad - this.earthRadian)/(b.rad - a.rad);
			var ratioB = (this.earthRadian - a.rad)/(b.rad - a.rad);

			for (var i = 0; i < 5; i++) {
				brightness[3*i + 0] = Math.floor(a.rgb[i].r*ratioA + b.rgb[i].r*ratioB);
				brightness[3*i + 1] = Math.floor(a.rgb[i].g*ratioA + b.rgb[i].g*ratioB);
				brightness[3*i + 2] = Math.floor(a.rgb[i].b*ratioA + b.rgb[i].b*ratioB);
			}
			
			//			console.log(this.earthRadian);
			//			console.log(brightness);

			break;
		}
	}

	tlc5940_update();
};
Space.prototype.show = function() {
	$('#page-space').css('display', 'block');
	this.setSize();
	this.setPosition();
};
Space.prototype.hidden = function() {
	$('#page-space').css('display', 'none');
};
