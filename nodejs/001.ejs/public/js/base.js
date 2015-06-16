var Pager = function() {
	this.index = new Index(this.pageTransition.bind(this));
	this.sky = new Sky(this.pageTransition.bind(this));
	this.status = new Status(this.pageTransition.bind(this));
	this.currentPage = this.id2obj("#page-index");
};
Pager.prototype.id2obj = function(id) {
	switch (id) {
	case "#page-index":
		return this.index;
	case "#page-sky":
		return this.sky;
	case "#page-status":
		return this.status;
	}
};
Pager.prototype.pageTransition = function(to) {
	this.currentPage.hidden();

	this.currentPage = this.id2obj(to);
	this.currentPage.show();
};
Pager.prototype.show = function() {
	this.currentPage.show();
};
Pager.prototype.resize = function() {
	this.currentPage.resize();
};

$(function() {
	var pager = new Pager();
	pager.show();

	$(window).resize(function() {
		pager.resize();
	});
});

