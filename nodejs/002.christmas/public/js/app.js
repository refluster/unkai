var app = angular.module('app', []);

app.controller('cBrightness', function($scope) {
	var disableUpdate = false;

	$scope.update = function() {
		if (disableUpdate) {
			return;
		}
		console.log($scope.brightness);
		socket.emit('led/setMaxBrightness', {brightness: $scope.brightness});
//		disableUpdate = true;
		setTimeout(function() {
			disableUpdate = false;
		}, 100)
	};
});

app.controller('cOptions', function($scope) {
	$scope.patterns = ['on', 'random', 'off']
	$scope.pattern = 'on';

	$scope.updatePattern = function() {
		console.log($scope.pattern);
		socket.emit('led/setPattern', {pattern: $scope.pattern});
	};
});

app.controller('cOn', function($scope) {
});

app.controller('cRandom', function($scope) {
	var disableUpdate = false;
	$scope.updateSpeed = function() {
		if (disableUpdate) {
			return;
		}
		$scope.times_per_sec = parseInt($scope.speed/8);
		socket.emit('led/setRandomInterval', {randomInterval: 8000/$scope.speed});
		console.log('interval ' + $scope.times_per_sec);
		disableUpdate = true;
		setTimeout(function() {
			disableUpdate = false;
		}, 100)
	};
	$scope.updateNumber = function() {
		if (disableUpdate) {
			return;
		}
		console.log($scope.number);
		disableUpdate = true;
		setTimeout(function() {
			disableUpdate = false;
		}, 100)
	};
});

app.controller('cOff', function($scope) {
});
