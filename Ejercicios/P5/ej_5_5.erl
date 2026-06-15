-module(ej_5_5).
-export([loop/1, loop_2/1, iniciar_nums/1, iniciar_pids/1]).
-export([crear_nodos/3]).

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

crear_nodos(0, NextPid, _FuncionLoop) ->
    NextPid;
crear_nodos(CantRestantes, NextPid, FuncionLoop) ->
    NuevoPid = spawn(ej_5_5, FuncionLoop, [NextPid]),
    crear_nodos(CantRestantes - 1, NuevoPid, FuncionLoop).

iniciar_nums(CantProcesos) ->
    PidDerecha = crear_nodos(CantProcesos - 1, self(), loop),
    PidDerecha ! {msg, CantProcesos},
    loop(PidDerecha).

% Iniciador para el anillo del testigo (1 sola vuelta)
iniciar_pids(CantProcesos) ->
    PidDerecha = crear_nodos(CantProcesos - 1, self(), loop_2),
    PidDerecha ! {msg, self()},
    loop_2(PidDerecha).
