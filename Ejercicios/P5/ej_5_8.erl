-module(ej_5_8).
-export([inicio/0, supervisor/0, loop_supervisor/0, trabajador/1]).

supervisor() ->
    process_flag(trap_exit, true),
    spawn_link(ej_5_8, trabajador, [5]),
    loop_supervisor().

loop_supervisor() ->
    receive
        {'EXIT', _PidProcesoMuerto, _Motivo} ->
            spawn_link(ej_5_8, trabajador, [5]),
            loop_supervisor()
    end.

trabajador(0) ->
    timer:sleep(1000),
    throw(exit);
trabajador(N) ->
    io:fwrite("Hello ~n", []),
    timer:sleep(1000),
    trabajador(N - 1).

inicio() ->
    spawn(ej_5_8, supervisor, []).
