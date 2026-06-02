-module(ej_5_5).
-export([loop/1, loop_2/1, iniciar/1]).
-export([crear_nodos/2]).

loop(NextPid) ->
    receive
        {msg, N} when N > 0 ->
            NextPid ! {msg, N - 1},
            loop(NextPid);
        {msg, 0} ->
            NextPid ! exit,
            receive
                exit -> ok
            end;
        exit ->
            NextPid ! exit,
            ok
    end.

loop_2(NextPid) ->
    receive
        {msg, OriginalPid} when OriginalPid /= self() ->
            NextPid ! {msg, OriginalPid},
            loop_2(NextPid);
        {msg, OriginalPid} when OriginalPid == self() ->
            NextPid ! exit,
            receive
                exit -> ok
            end;
        exit ->
            NextPid ! exit,
            ok
    end.

crear_nodos(0, NextPid) ->
    NextPid;
crear_nodos(CantRestantes, NextPid) ->
    NuevoPid = spawn(ej_5_5, loop, [NextPid]),
    crear_nodos(CantRestantes - 1, NuevoPid).

iniciar(CantProcesos) ->
    PidDerecha = crear_nodos(CantProcesos - 1, self()),

    % DESCOMENTAR LA QUE SE QUIERA USAR Y COMENTAR LA QUE NO SE QUIERA USAR
    % Para que se envíen N cantidad de mensajes
    % PidDerecha ! {msg, CantProcesos},

    % Para que dé sólo una vuelta
    PidDerecha ! {msg, self()},

    loop(PidDerecha).
