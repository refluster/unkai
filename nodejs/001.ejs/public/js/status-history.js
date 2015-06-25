var StatusHistory = function(pageTransition) {
	this.pageTransition = pageTransition;

	this.$page = $("#page-status-history");
	this.$toStatus = $("#page-status-history-to-status");

	this.$toStatus.bind("touchend", function(e) {
		this.pageTransition("#page-status");
	}.bind(this));
};

StatusHistory.prototype.show = function() {
	var parseDate = d3.time.format("%Y/%m/%d %H:%M:%S").parse;

	var margin = {top: 20, right: 20, bottom: 30, left: 40},
		width = 960 - margin.left - margin.right,
		height = 500 - margin.top - margin.bottom;

	var x = d3.time.scale()
	//    .domain([-width / 2, width / 2])
		.domain([parseDate("2015/06/23 14:22:05"), parseDate("2015/06/25 10:26:35")])
		.range([0, width]);

	var y = d3.scale.linear()
		.domain([-height / 2, height / 2])
		.range([height, 0]);

	var xAxis = d3.svg.axis()
		.scale(x)
		.orient("bottom")
		.tickSize(-height);

	var yAxis = d3.svg.axis()
		.scale(y)
		.orient("left")
		.ticks(5)
		.tickSize(-width);

	var zoom = d3.behavior.zoom()
		.x(x)
		.y(y)
		.scaleExtent([1, 10])
		.on("zoom", zoomed);

	var svg = d3.select("#status-history-chart").append("svg")
		.attr("width", width + margin.left + margin.right)
		.attr("height", height + margin.top + margin.bottom)
		.append("g")
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")")
		.call(zoom);

	svg.append("rect")
		.attr("width", width)
		.attr("height", height);

	svg.append("g")
		.attr("class", "x axis")
		.attr("transform", "translate(0," + height + ")")
		.call(xAxis);

	svg.append("g")
		.attr("class", "y axis")
		.call(yAxis);

	d3.select("button").on("click", reset);

	function zoomed() {
		svg.select(".x.axis").call(xAxis);
		svg.select(".y.axis").call(yAxis);
		svg.select("path.line").attr('d', line);
	}

	function reset() {
		d3.transition().duration(750).tween("zoom", function() {
			var ix = d3.interpolate(x.domain(), [-width / 2, width / 2]),
				iy = d3.interpolate(y.domain(), [-height / 2, height / 2]);
			return function(t) {
				zoom.x(x.domain(ix(t))).y(y.domain(iy(t)));
				zoomed();
			};
		});
	}

	//////////////////////////////
	var line = d3.svg.line()
		.x(function(d) { return x(d.date); })
		.y(function(d) { return y(d.illuminance); });

	d3.tsv("data/sensor.tsv", function(error, data) {
		if (error) throw error;

		data.forEach(function(d) {
			d.date = parseDate(d.date);
		});

		svg
			.append("path")
			.datum(data)
			.attr("class", "line")
			.attr("d", line);
	});

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
//		.css('top', (this.h - this.$svg.height())/2 + 'px')
//		.css('left', (this.w - this.$svg.width())/2 + 'px')
};
