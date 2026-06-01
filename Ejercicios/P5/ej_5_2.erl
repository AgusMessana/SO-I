-module(ej_5_2).
-export([wait/1, crono_loop/3, cronometro/3]).

% ### Inciso a)
wait(MS) ->
    % Al poner el receive completamente vacío (sin ningún patrón para hacer match antes del after), es imposible que el proceso agarre algún mensaje. Entonces, se queda bloqueado esperando, el tiempo se agota indefectiblemente, y termina ejecutando el ok.
    receive
    after MS -> ok
    end.

% ### Inciso b)
crono_loop(Fun, Hasta, Periodo) ->
    if
        (Hasta < Periodo) ->
            ok;
        true ->
            wait(Periodo),
            Fun(),
            crono_loop(Fun, Hasta - Periodo, Periodo)
    end.

cronometro(Fun, Hasta, Periodo) ->
    spawn(ej_5_2, crono_loop, [Fun, Hasta, Periodo]).
