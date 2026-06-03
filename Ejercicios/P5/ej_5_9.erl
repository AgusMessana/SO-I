-module(ej_5_9).
-export([inicio/0, supervisor/0, loop_supervisor/0]).
-export([trabajador/1]).

supervisor() ->
    process_flag(trap_exit, true),
    spawn_link(ej_5_9, trabajador, [5]),
    ej_5_9:loop_supervisor().

loop_supervisor() ->
    receive
        {'EXIT', _PidProcesoMuerto, _Motivo} ->
            spawn_link(ej_5_9, trabajador, [5]),
            ej_5_9:loop_supervisor()
    end.

trabajador(0) ->
    timer:sleep(1000),
    throw(exit);
trabajador(N) ->
    io:fwrite("Hola ~n", []),
    timer:sleep(1000),
    ej_5_9:trabajador(N - 1).

inicio() ->
    spawn(ej_5_9, supervisor, []).
