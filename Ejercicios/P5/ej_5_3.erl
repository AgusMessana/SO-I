-module(ej_5_3).
-export([bal/1, bal_corregida/1]).

bal(Servs) ->
    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(rand:uniform(length(Servs)), Servs),
            Proc ! {req, Arg, self()},
            receive
                Reply -> Pid ! Reply
            end
    end,
    bal(Servs).

% El problema es el segundo receive. En Erlang, un proceso entra a receive y se queda esperando hasta recibir algo. Si el procesoencargado de mandar la respuesta demora 10 segundos, el proceso del receive se quedará esperando 10 segundos. De esta forma, no puede atender a los demás clientes mientras tanto.

% Para solucionarlo podemos plantear la siguiente función:
bal_corregida(Servs) ->
    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(rand:uniform(length(Servs)), Servs),
            Proc ! {req, Arg, Pid}
    end,
    bal_corregida(Servs).

% En lugar de que el Servidor (Proc) le devuelva la respuesta al Balanceador (para que este se la pase al Cliente), modificamos el mensaje para que el Servidor le envíe la respuesta directamente al Cliente original (Pid). Así, el Balanceador no tiene que quedarse esperando.
