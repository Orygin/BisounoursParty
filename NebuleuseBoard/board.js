angular.module('ngView', ['ui.bootstrap'], function($routeProvider, $locationProvider) {
		$routeProvider.
			when('/', {templateUrl:'template/en/home.html'}).
			when('/Achievements/', {templateUrl:'template/en/achievements.html', controller: AchievementsCtrl}).
			when('/AddAchievement/', {templateUrl:'template/en/editachievement.html', controller: AchievementCtrl}).
			when('/EditAchievement/:id', {templateUrl:'template/en/editachievement.html', controller: AchievementCtrl}).
			when('/Updates/', {templateUrl:'template/en/updates.html', controller:updatesCtrl}).
			when('/Settings/', {templateUrl:'template/en/settings.html', controller: settingsCtrl}).
			when('/Players/', {templateUrl:'template/en/players.html', controller: playersCtrl}).
			otherwise({redirectTo:'/'});
});

function MainCtrl($scope, $http) {
	$http.get('api/getSettings/').success(function(data) {
		console.log(data);
		$scope.settings = data;
	}).error(function()
	{
		$scope.addAlert('Error, could not load the settings.', error);
	});

	$scope.updateStats = function(){
		$http.get('api/getStats/').success(function(data) {
			$scope.stats = data.stats;
		}).error(function()
		{
			$scope.addAlert('Error, could not load the stats info.', error);
		});
	};
	$scope.updateStats();
	$scope.alerts = [
	];
	$scope.switchChannel = function(){
		$http.get('api/switchChannel/').success(function(data) {
			document.location.reload(true);
		}).error(function()
		{
			$scope.addAlert('Error, could not switch channels.', error);
		});
	};
	$scope.addAlert = function(message, typ) {
		$scope.alerts.push({msg: message, type:typ});
	};

	$scope.closeAlert = function(index) {
		$scope.alerts.splice(index, 1);
	};

  	$scope.toDateTime = function(secs){
		var t = new Date(1970,0,1);
	    t.setSeconds(secs);
	    var s = t.toTimeString().substr(0,8);
	    if(secs > 86399)
	    	s = Math.floor((t - Date.parse("1/1/70")) / 3600000) + s.substr(2);
	    return s;
	};
}
function AchievementsCtrl($scope, $http, $location)
{
	$scope.updateStats();
	$http.get('api/getAchievements/').success(function(data) {
		$scope.Achievements = data.achievements;
	}).error(function(){
		$scope.addAlert("Couldn't load the achievements data", 'error');
	});
	$scope.AddAchievement = function(){
		$location.path("/EditAchievement/");
	};
	$scope.editAchievement = function(id){
		$location.path("/EditAchievement/" + id + "/");
	};
}
function AchievementCtrl($scope, $routeParams, $http, $location) {
	$scope.id = $routeParams.id;
	$scope.updateStats();

	if(!$routeParams.id) // new achievement
		$scope.Achievement = { "icon": "img/avatar.jpg", "id": -1, "name": "Dev name", "fullName": "Name", "fullDesc": "Description", "progress": 0, "progressMax": 10 };
	else
	{
		$http.get('api/getAchievement/' + $routeParams.id).success(function(data) {
			$scope.Achievement = data.achievement;
		}).error(function()
		{
			$scope.addAlert("Couldn't load the achievement data", 'error');
		});
	}
	$scope.isFormValid = function()
	{
		if($scope.AchForm.AchievementTitle.$invalid)
			return false;
		else if($scope.AchForm.AchievementDescr.$invalid)
			return false;
		else if($scope.AchForm.AchievementMax.$invalid)
			return false;
		return true;
	};
	$scope.save = function()
	{
		if(!$scope.isFormValid())
			$scope.addAlert('The form is not valid', 'error');
		else
		{
			var url;
			if($scope.Achievement.id == -1)
				url = 'api/addAchievement/';
			else
				url = 'api/setAchievement/';
			$http({
			    method: 'POST',
			    url: url,
			    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
			    transformRequest: function(obj) {
			        var str = [];
			        for(var p in obj)
			        str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
			        return str.join("&");
			    },
			    data: {
			    		id:$scope.Achievement.id,
			    		devname:$scope.Achievement.name,
			    		fullname:$scope.Achievement.fullName,
			    		fulldesc:$scope.Achievement.fullDesc,
			    		max:$scope.Achievement.progressMax
			    	}
			}).success(function(data)
			{
				$location.path('/Achievements/');
			}).error(function()
			{
				$scope.addAlert('Error, could not update the achievement.', error);
			});
		}
	}
	$scope.goBack = function()
	{
		$location.path('/Achievements/');
	};
	$scope.delete = function()
	{
		$http({
		    method: 'POST',
		    url: 'api/deleteAchievement/',
		    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		    transformRequest: function(obj) {
		        var str = [];
		        for(var p in obj)
		        str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
		        return str.join("&");
		    },
		    data: {
		    		id:$scope.Achievement.id
		    	}
		}).success(function(data)
		{
			$location.path('/Achievements/');
		}).error(function()
		{
			$scope.addAlert('Error, could not delete the achievement.', error);
		});
	}
}
function settingsCtrl($scope, $http, $location, $dialog)
{
	$scope.updateStats();
	$scope.settings = [];
	$http.get('api/getSettings/').success(function(data) {
		$scope.settings = data;
	}).error(function()
	{
		$scope.addAlert('Error, could not load the settings.', error);
	});
	$scope.save = function(){
		if(!$scope.isFormValid())
			$scope.addAlert('The form is not valid', 'error');
		else
		{
			$http({
		    	method: 'POST',
			    url: 'api/setSettings/',
			    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
			    transformRequest: function(obj) {
		    	    var str = [];
		        	for(var p in obj)
		        	str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
		        	return str.join("&");
		    	},
		    	data: $scope.settings
			}).success(function(data)
			{
				$location.path('/settings/');
			}).error(function()
			{
				$scope.addAlert('Error, could not update the settings.', error);
			});
		}
	};
	$scope.goBack = function(){
		$location.path('/');
	};
	$scope.isFormValid = function()
	{
		if($scope.SettingsForm.GameName.$invalid)
			return false;
		else if($scope.SettingsForm.GameKey.$invalid)
			return false;
		else if($scope.SettingsForm.SessionTimeOut.$invalid)
			return false;
		return true;
	};

	$scope.clearSessions = function(){
		var title = 'Are you sure ?';
		var msg = 'This will disconnect all currently connected players.';
		var btns = [{result:'cancel', label: 'Cancel'}, {result:'ok', label: 'OK', cssClass: 'btn-primary'}];

		$dialog.messageBox(title, msg, btns)
		.open()
		.then(function(result){
			if(result=="ok")
			{
				$http.get('api/clearSessions/').success(function(){
					$scope.addAlert('Successfully cleared sessions', 'info');
				}).error(function(){
					$scope.addAlert('Couldn\'t clear sessions', 'error');
				});
			}
		});
	};
	$scope.exportDatabase = function(){
		window.location = 'api/exportDatabase/';
	};
}
function updatesCtrl($scope, $http, $location, $dialog)
{
	$scope.updateStats();

	$http.get('api/getUpdates/').success(function(data) {
		$scope.updates = data.updates;
		for (var i = data.updates.length - 1; i >= 0; i--) {
			$scope.updates[i].log = $scope.updates[i].log.replace(/\r\n/g, '<br />');
		};
	}).error(function()
	{
		$scope.addAlert('Error, could not load the updates.', error);
	});
	
	$http.get('api/getMirrors/').success(function(data) {
		$scope.mirrors = data.mirrors;
	}).error(function()
	{
		$scope.addAlert('Error, could not load the mirrors.', error);
	});

	$scope.setVersion = function(ver){
		$http({
		   	method: 'POST',
		    url: 'api/setActiveVersion/',
		    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		    transformRequest: function(obj) {
		   	    var str = [];
		       	for(var p in obj)
		       	str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
		       	return str.join("&");
		   	},
		   	data: {version: ver}
		}).success(function(data)
		{
			$scope.settings.gameVersion = ver;
		}).error(function()
		{
			$scope.addAlert('Error, could not change the active version.', error);
		});
	};
	$scope.deleteUpdate = function(update){
		var title = 'Are you sure ?';
		var msg = 'This will not delete the patch file on the mirrors.';
		if(update.version == $scope.settings.gameVersion)
			msg = msg + " This update is the current version of the game."
		var btns = [{result:'cancel', label: 'Cancel'}, {result:'ok', label: 'OK', cssClass: 'btn-primary'}];

		$dialog.messageBox(title, msg, btns)
		.open()
		.then(function(result){
			if(result=="ok")
			{
				$http({
				   	method: 'POST',
				    url: 'api/deleteUpdate/',
				    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
				    transformRequest: function(obj) {
				   	    var str = [];
				       	for(var p in obj)
				       	str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
				       	return str.join("&");
				   	},
				   	data: {version: update.version}
				}).success(function(data)
				{
					$scope.removeUpdateFromList(update.version);
				}).error(function()
				{
					$scope.addAlert('Error, could not delete the selected update.', error);
				});
			}
		});
	};
	$scope.showLog = function(update){
		var opts = {
		    backdrop: true,
		    keyboard: true,
		    backdropClick: true,
			templateUrl: 'template/en/dialogChangelog.html',
		    controller: 'ChangelogDialogCtrl',
		    update: update
  		};
  		$dialog.dialog(opts).open();
	};
	$scope.addUpdate = function(){
		var opts = {
		    backdrop: true,
		    keyboard: true,
		    backdropClick: true,
			templateUrl: 'template/en/dialogAddUpdate.html',
		    controller: 'AddUpdateDialogCtrl',
		    version: $scope.settings.gameVersion,
		    callback: $scope.addUpdateToList
  		};
  		$dialog.dialog(opts).open();
	};
	$scope.addUpdateToList = function(update){
		$scope.updates.push(update);
	};
	$scope.removeUpdateFromList = function(version){
		for (var i = $scope.updates.length - 1; i >= 0; i--) {
			if($scope.updates[i].version == version)
				$scope.updates.splice(i,1);
		};
	};
}

function ChangelogDialogCtrl($scope, dialog){
	$scope.update = dialog.options.update;
	$scope.close = function(result){
		dialog.close(result);
	};
}
function AddUpdateDialogCtrl($scope, dialog, $http, $location){
	$scope.update = { version: dialog.options.version +1, size: 1, log:"abc" };
	$scope.close = function(result){
		if(result == 'close')
			dialog.close(result);
		else if(result == 'save')
		{
			if($scope.updateForm.size.$invalid || $scope.updateForm.version.$invalid)
				return;

			$http({
			   	method: 'POST',
			    url: 'api/addUpdate/',
			    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
			    transformRequest: function(obj) {
			   	    var str = [];
			       	for(var p in obj)
			       	str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
			       	return str.join("&");
			   	},
			   	data: {
			   			version: $scope.update.version,
			   			size: $scope.update.size,
			   			log: $scope.update.log
			   			}
			}).error(function()
			{
				$scope.addAlert('Error, could not change the active version.', error);
			}).success(function(data){
				function ISODateString(d){
				  function pad(n){return n<10 ? '0'+n : n}
				  return d.getUTCFullYear()+'-'
				      + pad(d.getUTCMonth()+1)+'-'
				      + pad(d.getUTCDate())+' '
				      + pad(d.getUTCHours())+':'
				      + pad(d.getUTCMinutes())+':'
				      + pad(d.getUTCSeconds())+''
				}
				 
				var d = new Date();

				dialog.options.callback({
			   			version: $scope.update.version,
			   			size: $scope.update.size,
			   			log: $scope.update.log,
			   			date: ISODateString(d),
			   			fullyPropagated: true
			   			});
			});
			dialog.close(result);
		}
	};
}

function playersCtrl($scope, $http, $location, $dialog)
{
	$scope.loadCurrentPage = function(){
		$http.get('api/getPlayers/' + $scope.currentPage).success(function(data) {
			$scope.players = data.players;
			$scope.pages = Math.ceil(data.count/30);
			$scope.players.forEach(function(pl){
				$http.get('api/getPlayer/' + pl.steamId).success(function(data){
					pl.name = data.response.players[0].personaname;
					pl.avatar = data.response.players[0].avatar;
				});
			});
		}).error(function()
		{
			$scope.addAlert('Error, could not load the player list.', error);
		});
	};

	$scope.updateStats();
	$scope.predicate = '-rank';
	$scope.reverse = false;
	$scope.currentPage = 1;
	$scope.pages = 1;
	$scope.$watch('currentPage', $scope.loadCurrentPage);
	$scope.loadCurrentPage();


	$scope.setPlayerRank = function(player, prank){
		if(player.rank+prank < 0)
			return;
		$http({
		    method: 'POST',
		    url: 'api/setPlayerRank/',
		    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		    transformRequest: function(obj) {
		        var str = [];
		        for(var p in obj)
		        str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
		        return str.join("&");
		    },
		    data: {id:player.id, rank:player.rank+prank}
		}).success(function(data){
			console.log(data);
					player.rank = player.rank+prank;
		}).error(function(){
			addAlert('Could not change player rank', 'error');
		});
	};
	$scope.deletePlayer = function(pid){
		var title = 'Are you sure ?';
		var msg = 'This will delete all of the player\'s data.';
		var btns = [{result:'cancel', label: 'Cancel'}, {result:'ok', label: 'OK', cssClass: 'btn-primary'}];

		$dialog.messageBox(title, msg, btns)
		.open()
		.then(function(result){
			if(result=="ok")
			{
				$http({
				    method: 'POST',
				    url: 'api/deletePlayer/',
				    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
				    transformRequest: function(obj) {
				        var str = [];
				        for(var p in obj)
				        str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
				        return str.join("&");
				    },
				    data: {id:pid.id}
				}).error(function(){
					$scope.addAlert('Could not delete the player', 'error');
				}).success(function(data){
					for(var i =0; i<$scope.players.length; i++)
					{
						if($scope.players[i].id == pid.id)
							$scope.players.splice(i,1);
					}
				});
			}
		});
	};
}