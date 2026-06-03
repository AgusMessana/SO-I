-module(ej_5_10).
-export([ping/0, pong/0, start_ping/0, start_pong/0]).

ping() ->
    receive
        {fin, PidPong} ->
            io:fwrite("Ping Terminó: ~p ~n", [PidPong]);
        {0, PidPong} ->
            io:fwrite("Ping recv: ~p ~n", [PidPong]),
            PidPong ! {fin, self()},
            okPing;
        {N, PidPong} ->
            io:fwrite("Ping recv: ~p ~p ~n", [N, PidPong]),
            PidPong ! {N - 1, self()},
            ping()
    end.

pong() ->
    receive
        {fin, PidPing} ->
            io:fwrite("Pong Terminó: ~p ~n", [PidPing]);
        {0, PidPing} ->
            io:fwrite("Pong recv: ~p ~n", [PidPing]),
            PidPing ! {fin, self()},
            okPong;
        {N, PidPing} ->
            io:fwrite("Pong recv: ~p ~p ~n", [N, PidPing]),
            PidPing ! {N - 1, self()},
            pong()
    end.

start_ping() ->
    PidPing = spawn(ej_5_10, ping, []),
    register(jugador_ping, PidPing),
    {jugador_pong, 'nodo_b@Notebook-Agustin'} ! {10, PidPing}.

start_pong() ->
    PidPong = spawn(ej_5_10, pong, []),
    register(jugador_pong, PidPong).
