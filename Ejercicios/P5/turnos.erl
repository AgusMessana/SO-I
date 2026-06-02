-module(turnos).
-export([server/0]).

server() ->
	{ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
	wait_connect(ListenSocket, 0).

wait_connect(ListenSocket, N) ->
	{ok, Socket} = gen_tcp:accept(ListenSocket),
	spawn (fun () -> wait_connect (ListenSocket, N+1) end),
	get_request(Socket).

get_request(Socket) ->
	io:fwrite("Esperando mensajes de ~p~n", [Socket]),
	receive
		_X -> ok,
		      get_request(Socket)
	end.
