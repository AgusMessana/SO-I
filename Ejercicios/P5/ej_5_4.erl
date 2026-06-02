-module(ej_5_4).
-export([server/0]).

server() ->
    {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
    wait_connect(ListenSocket, 0).

wait_connect(ListenSocket, N) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    spawn(fun() -> wait_connect(ListenSocket, N + 1) end),
    get_request(Socket, N).

get_request(Socket, N) ->
    io:fwrite("Esperando mensajes de ~p~n", [Socket]),
    receive
        {tcp, Socket, _Paquete} ->
            ok,
            NumeroTexto = integer_to_list(N) ++ "\n",
            gen_tcp:send(Socket, NumeroTexto),
            get_request(Socket, N);
        {tcp_closed, Socket} ->
            ok
    end.

% b)
% Sí, hay una gran diferencia de consumo de memoria entre C y Erlang. En C, cuando se crea cada hilo, el Sistema Operativo le reserva un bloque grande de memoria RAM para su stack. De esta forma, si creamos 20.000 hilos, la cantidad de memoria reservada crece exponencialmente, lo que puede ocasionar congelamiento.
% En Erlang, la máquina virtual crea procesos significativamente más ligeros y propios, sin pedirle hilos al Sistema Operativo. Por ende, puede haber muchos procesos por una poca cantidad de memoria RAM.

% c)
% En C no podríamos tener 50.000 conexiones simultáneas por lo explicado en b). En Erlang, si no ejecutamos el comando "ulimit -n", no podremos porque el Sistema Operativo lo impide. Si usamos ese comando y aumentamos la cantidad de File Descriptors que podemos abrir, Erlang podrá tener sin problema las 50.000 conexiones simultáneas e inluso más.
