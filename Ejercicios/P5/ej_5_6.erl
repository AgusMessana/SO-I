-module(ej_5_6).
-export([iniciar/0]).
-export([loop/1, suscribirse/1, desuscribirse/1]).
-export([difundir/2]).

loop(Suscriptores) ->
    receive
        {suscribirse, PidCliente} ->
            case lists:member(PidCliente, Suscriptores) of
                true ->
                    PidCliente ! {error, ya_suscrito},
                    loop(Suscriptores);
                false ->
                    PidCliente ! {ok, suscrito},
                    loop([PidCliente | Suscriptores])
            end;
        {difundir, Mensaje} ->
            lists:foreach(fun(Suscriptor) -> Suscriptor ! Mensaje end, Suscriptores),
            loop(Suscriptores);
        {desuscribirse, PidCliente} ->
            PidCliente ! {ok, desuscrito},
            NuevosSuscriptores = lists:delete(PidCliente, Suscriptores),
            loop(NuevosSuscriptores)
    end.

suscribirse(ServerPid) ->
    ServerPid ! {suscribirse, self()},
    receive
        {ok, suscrito} ->
            ok_suscrito;
        {error, ya_suscrito} ->
            ya_estaba
    after 5000 ->
        {error, timeout}
    end.

desuscribirse(ServerPid) ->
    ServerPid ! {desuscribirse, self()},
    receive
        {ok, desuscrito} ->
            ok_desuscrito
    after 5000 ->
        {error, timeout}
    end.

difundir(ServerPid, Mensaje) ->
    ServerPid ! {difundir, Mensaje}.

iniciar() ->
    spawn(ej_5_6, loop, [[]]).
