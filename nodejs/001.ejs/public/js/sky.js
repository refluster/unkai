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
	size = Math.floor(size);
	this.elem
		.css("height", size + "px")
		.css("width", size + "px")
		.css("position", "absolute");
	this.size = size;
};
Star.prototype.setPosition = function(x, y) {
	x = Math.floor(x);
	y = Math.floor(y);
	this.elem.css("transform", "translate(" + (x - this.size/2) + "px, " + (y - this.size/2) + "px)");
};

////////////////////////////////////////////////////////////
Sky = function(pageTransition) {
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
	
	this.$page = $("#page-sky");
	this.$sky = $("#sky-space");
	this.$toIndex = $(".page-to-index");

	this.$sky.css('background', 'radial-gradient(rgba(0, 0, 0, 1) 50%,' +
				  'rgba(0, 0, 0, 0.3) 60%,' +
				  'rgba(0, 0, 0, 0.1) 65%,' +
				  'rgba(240, 240, 240, 0) 70%)');

	this.touchStartX;
	this.touchStartY;
	this.touchStartRadian;

	this.earthRadian = 0;

	this.earth = new Star($("#earth"));
	this.brightstar = new Star($("#brightstar"));
	this.darkstar = new Star($("#darkstar"));

	this.pageTransition = pageTransition;
	
	this.$sky.bind('touchstart', this.inputStart.bind(this));
	this.$sky.bind('touchmove', this.inputMove.bind(this));
	this.$sky.bind('touchend', this.inputEnd.bind(this));
	this.$toIndex.bind("touchend", function(e) {
		this.pageTransition("#page-index");
	}.bind(this));
};
Sky.prototype.inputStart = function(e) {
	this.touchStartX = e.originalEvent.touches[0].pageX;
	this.touchStartY = e.originalEvent.touches[0].pageY;
	this.touchStartRadian = this.earthRadian;
};
Sky.prototype.inputMove = function(e) {
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
Sky.prototype.inputEnd = function(e) {
};
Sky.prototype.resize = function() {
	this.w = this.$page.width();
	this.h = this.$page.height();
	this.cx = this.w/2;
	this.cy = this.h/2;
	var baseSize = Math.floor(1.2 * ((this.w > this.h)? this.h: this.w));

	this.$sky.css("height", baseSize + 'px');
	this.$sky.css("width", baseSize + 'px');
	this.$sky.css("top", (this.cy - baseSize/2) + "px");
	this.$sky.css("left", (this.cx - baseSize/2) + "px");

	this.earth.setSize(baseSize / 10);
	this.brightstar.setSize(baseSize / 16);
	this.darkstar.setSize(baseSize / 16);

	this.earthRotate();
	this.brightstar.setPosition(baseSize/2, baseSize/2 - baseSize/2*0.7);
	this.darkstar.setPosition(baseSize/2, baseSize/2 + baseSize/2*0.7);
};
Sky.prototype.earthRotate = function() {
	const ratio = 0.6;
	var baseSize = 1.2 * ((this.w > this.h)? this.h: this.w);
	var x = baseSize/2 + ratio*baseSize/2*Math.sin(this.earthRadian);
	var y = baseSize/2 + ratio*baseSize/2*Math.cos(this.earthRadian);
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
			
			break;
		}
	}

	tlc5940_update();
};
Sky.prototype.show = function() {
	this.$page.css('display', 'block');
	this.resize();
//	this.setPosition();
};
Sky.prototype.hidden = function() {
	this.$page.css('display', 'none');
};
