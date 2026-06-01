-module(ej2).
-export([wait/1, crono/3]).

wait (S) ->
    receive
    after(S*1000)-> ok
end.

crono(Fun,0,Periodo) -> ok;
crono(Fun, Hasta, Periodo) ->
    if 
       ( Hasta < Periodo) ->
            ok;
        true -> 
            receive
                after(Periodo) -> Fun()
            end,
            crono(Fun,Hasta-Periodo,Periodo)
end.