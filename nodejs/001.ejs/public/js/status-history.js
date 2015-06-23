var StatusHistory = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-status-history");
	this.$toStatus = $("#page-status-history-to-status");

	this.$toStatus.bind("touchend", function(e) {
		this.pageTransition("#page-status");
	}.bind(this));
};

StatusHistory.prototype.show = function() {
	var margin = {top: 20, right: 20, bottom: 30, left: 50},
		width = 960 - margin.left - margin.right,
		height = 500 - margin.top - margin.bottom;

	var parseDate = d3.time.format("%Y/%m/%d %H:%M:%S").parse;

	var x = d3.time.scale()
		.range([0, width]);

	var y = d3.scale.linear()
		.range([height, 0]);

	var xAxis = d3.svg.axis()
		.scale(x)
		.orient("bottom");

	var yAxis = d3.svg.axis()
		.scale(y)
		.orient("left");

	var line = d3.svg.line()
		.x(function(d) { return x(d.date); })
		.y(function(d) { return y(d.illuminance); });

	var svgFrame = d3.select("#status-history-chart").append("svg")
		.attr("id", "status-history-svg")
		.attr("width", width + margin.left + margin.right)
		.attr("height", height + margin.top + margin.bottom)
	
	var svg = svgFrame.append("g")
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")");


	this.$svg = $("#status-history-svg");
	
	d3.tsv("data/sensor.tsv", function(error, data) {
		if (error) throw error;

		data.forEach(function(d) {
			d.date = parseDate(d.date);
		});

		x.domain(d3.extent(data, function(d) { return d.date; }));
		y.domain(d3.extent(data, function(d) { return d.illuminance; }));

		svg.append("g")
			.attr("class", "x axis")
			.attr("transform", "translate(0," + height + ")")
			.call(xAxis);

		svg.append("g")
			.attr("class", "y axis")
			.call(yAxis)
			.append("text")
			.attr("transform", "rotate(-90)")
			.attr("y", 6)
			.attr("dy", ".71em")
			.style("text-anchor", "end")
			.text("illuminance");

		svg.append("path")
			.datum(data)
			.attr("class", "line")
			.attr("d", line);
	}.bind(this));

	this.resize();
	this.$page.css('display', 'block');
};

StatusHistory.prototype.hidden = function() {
	this.$page.css('display', 'none');
};

StatusHistory.prototype.resize = function() {
	this.w = this.$page.width();
	this.h = this.$page.height();
	var shortSide = (this.w > this.h? this.h: this.w);
	var pageBackIconSide = Math.floor(shortSide * 0.08);
	var vMargin = this.h * 0.1;
	var hMargin = this.w * 0.1;

	this.$toStatus
		.css('display', 'block')
		.css('background-image', 'url(/img/page-back.png)')
		.css('background-size', pageBackIconSide + 'px')
		.css('opacity', '0.5')
		.css('top', '0px')
		.css('left', '0px')
		.css('width', pageBackIconSide + 'px')
		.css('height', pageBackIconSide + 'px');
	this.$svg
		.css('position', 'absolute')
		.css('top', vMargin + 'px')
		.css('bottom', vMargin + 'px')
		.css('left', hMargin + 'px')
		.css('right', hMargin + 'px');
};
