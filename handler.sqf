/*
    Name: fn_handler.sqf
    Author: Nikko
    Description: Handles Communication Will Extension

    Parameters:
    NONE

    Return:
    NONE
*/
 
Example_Server_fnc_Steam2OwnerID = compileFinal "
    _owner = (((allPlayers - entities 'HeadlessClient_F')apply{ if(getplayeruid _x == _this)then{owner _x}else{''}}) - ['']);
    (if(count _owner == 1)then{_owner#0}else{-100})
";
Example_Server_fnc_callExtension = compileFinal "
	private _return = '';
	private _params = _this;
	if((typeName _params) isEqualTo 'ARRAY')then{
		_params set [1,((_params#1) apply {
			switch(typeName _x)do
			{
				case 'OBJECT': {call compile(_x call BIS_fnc_objectVar)};
				default {_x};
			};
		})];
	}; 
	_return = ('Example' callExtension _params);

	private _errorlog = '<Example> Extension Returned Error';

	if(typeName _return isEqualTo 'ARRAY')then{ 
		_return params [
			['_checkExt_RtnMessage',''],
			['_checkExt_StatusCode',-1],
			['_checkExt_ErrorCode',-1]
		];
		
		private __ERROR_CODES__ = [-2,-1,400,401,402,403,404,405,406,407,408,409,410,411,412,412,414,415,416,417,418];

		if(_checkExt_StatusCode in __ERROR_CODES__)then{
		
			if(_checkExt_RtnMessage != '' && _checkExt_ErrorCode == 0)then{
				diag_log format['%1 Message: %2',_errorlog,_checkExt_RtnMessage]; 
			}else{
				switch _checkExt_ErrorCode do {
					case 0: {diag_log _errorlog;};
					case 101: {diag_log format['%1 Exception: (SYNTAX_ERROR_WRONG_PARAMS_SIZE)',_errorlog];}; 
					case 102: {diag_log format['%1 Exception: (SYNTAX_ERROR_WRONG_PARAMS_TYPE)',_errorlog];}; 
					case 201: {diag_log format['%1 Exception: (PARAMS_ERROR_TOO_MANY_ARGS)',_errorlog];}; 
					case 302: {diag_log format['%1 Exception: (EXECUTION_WARNING_TAKES_TOO_LONG)',_errorlog];}; 
				};
			};
		};
		_return = _checkExt_RtnMessage;
	} else {
		if(_return == '')then{
			diag_log _errorlog;
		};
	};

	_return
";
Example_Server_fnc_ExtensionCallback = {
	params [["_name","",[""]], ["_function","",[""]], ["_params","[]",[""]]];

	_params = call compile _params;
	Testp = _this;
	//Handle different requests from dll
	switch _name do {
		case "CallFunction": 
		{ 
			_expression = str(_params) + " call (missionNamespace getVariable ['"+_function+"',{diag_log format['<Example_Server_fnc_ExtensionCallback> Function <%1> does not exist in <missionNamespace>!','"+_function+"']}]);";
			call compile _expression;
		};
		case "SpawnFunction": 
		{ 
			_expression = str(_params) + " spawn (missionNamespace getVariable ['"+_function+"',{diag_log format['<Example_Server_fnc_ExtensionCallback> Function <%1> does not exist in <missionNamespace>!','"+_function+"']}]);";
			call compile _expression;
		};
		case "RunCallCode": 
		{ 
			_code = [_params, 1,{},[{}]]call BIS_fnc_param;
			_steamID = [_params, 2,""]call BIS_fnc_param;
			_params = [_params, 0,[]]call BIS_fnc_param;

			if(typeName _code isEqualTo "STRING")then{ _code = compile _code; };
			if !(typeName _code isEqualTo "CODE")exitWith{};

			if(_steamID != "")then{
				[_params,_code] remoteExecCall ["call",(_steamID call Example_Server_fnc_Steam2OwnerID)];
			}else{
				_params call _code;
			};
		};
		case "RunSpawnCode": 
		{ 
			_code = [_params, 1,{},[{}]]call BIS_fnc_param;
			_steamID = [_params, 2,""]call BIS_fnc_param;
			_params = [_params, 0,[]]call BIS_fnc_param;

			if(typeName _code isEqualTo "STRING")then{ _code = compile _code; };
			if !(typeName _code isEqualTo "CODE")exitWith{};

			if(_steamID != "")then{
				[_params,_code] remoteExecCall ["spawn",(_steamID call Example_Server_fnc_Steam2OwnerID)];
			}else{
				_params spawn _code;
			};
			
		};
		case "Rpt": 
		{ 
			if(count _params < 1)exitWith{};
			if(count _params == 1)then{
				diag_log (_params#0);
			}else{
				diag_log format _params;
			};
		};
		case "Hint": 
		{ 
			_steamID = [_params, 1,"",[""]]call BIS_fnc_param;
			_params = [_params, 0,[],[[]]]call BIS_fnc_param;
			(format _params) remoteExecCall ["hint",(_steamID call Example_Server_fnc_Steam2OwnerID)];
		};
	};
};
Example_Server_fnc_ExtensionInit = {
	///////////////////
	//-- Version
	private _version = "version" call Example_Server_fnc_callExtension;
	diag_log format ["<ExampleTest> version: %1", _version];

	///////////////////
	//-- Setup Callback EventHandler
	//removeAllMissionEventHandlers "ExtensionCallback";
	Example_Server_var_ExtensionCallback = addMissionEventHandler ["ExtensionCallback",{_this call Example_Server_fnc_ExtensionCallback;}];
	ExampleTest = { systemChat format ["<ExampleTest> Params: %1", _this] };

	///////////////////
	//-- Beta Testing
	["start",[""]] call Example_Server_fnc_callExtension; // "OK" || ""
};

[] call Example_Server_fnc_ExtensionInit;
"test" call Example_Server_fnc_callExtension;
//[ "test", [ player, [ 1, "test" ], {} ] ] call Example_Server_fnc_callExtension;