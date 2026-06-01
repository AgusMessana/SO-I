-module(ej_5_1).
-export([init/0, match_test/0]).
-export([nombre/1, apellido/1]).
-export([
    filtrar_por_apellido/2,
    tuple_test/2,
    filtrar_por_apellido_comprension/2
]).

match_test() ->
    {A, B} = {5, 4},
    % {C, C} = {5, 4},
    {B, A} = {4, 5},
    {D, D} = {5, 5}.

string_test() ->
    [
        helloworld == 'helloworld',
        "helloworld" < 'helloworld',
        helloworld == "helloworld",
        [$h, $e, $l, $l, $o, $w, $o, $r, $l, $d] == "helloworld",
        [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] <
            {104, 101, 108, 108, 111, 119, 111, 114, 108, 100},
        [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] > 1,
        [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] == "helloworld"
    ].

tuple_test(P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido({persona, {nombre, _Nombre}, {apellido, Apellido}}) ->
    % También funciona apellido({persona, _, {apellido, Apellido}})
    Apellido.

nombre({persona, {nombre, Nombre}, {apellido, _Apellido}}) ->
    % También funciona nombre({persona, {nombre, Nombre}, _})
    Nombre.

filtrar_por_apellido([], _Apellido) ->
    [];
filtrar_por_apellido([P | Resto], Apellido) ->
    case apellido(P) == Apellido of
        true ->
            [nombre(P) | filtrar_por_apellido(Resto, Apellido)];
        false ->
            filtrar_por_apellido(Resto, Apellido)
    end.

filtrar_por_apellido_comprension(Personas, Apellido) ->
    [nombre(P) || P <- Personas, apellido(P) == Apellido].

init() ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia"),
    io:fwrite("Mi lista es: ~p~n", [Garcias]).

% a)
% En Erlang, el operador = tiene dos usos: si la variable está libre (unbound), le asigna un dato. Si ya tiene un dato asignado (bound), se hace una comparación. Teniendo en cuenta esto, tenemos que:
% i) {A,B} = {5, 4} es válido pues las variables A y B están unbound.
% ii) {C,C} = {5, 4} es inválido pues al principio la variable C está unbound, y le asigna 5. Luego, se toma la segunda variable C y se la compara con 4, lo que es incorrecto.
% iii) {B,A} = {4, 5} es válida pues al estar las dos variables bound, compara si B es igual a 4 y A igual a 5, lo cual es verdad.
% iv) {D,D} = {5, 5} es válida pues al principio la variable D está unbound, así que se le asigna el valor 5. Luego, al estar bound, se compara si D es igual a 5, lo que es verdad.

% c)
% number < atom < reference < fun < port < pid < tuple < map < list < bitstring

% i) helloworld == 'helloworld' es true pues son dos formas iguales de escribir un átomo. Puede escribirse con '' o sin ellas.
% ii) "helloworld" < 'helloworld' es false pues el tipo de dato list (string en este caso) es más grande que el tipo de dato atom.
% iii) [$h, $e, $l, $l, $o, $w, $o, $r, $l, $d] == "helloworld" es true pues el comando $(letra) devuelve el código ASCII de ella. Al comparar la lista de códigos con el string, coinciden dato a dato.
% iv) [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] < {104, 101, 108, 108, 111, 119, 111, 114, 108, 100} es false pues el dato list es mayor que el dato tuple.
% v) [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] > 1 es true pues el tipo de dato list es mayor que el tipo de dato number.
% vi) [104, 101, 108, 108, 111, 119, 111, 114, 108, 100] == "helloworld" es true por lo explicado con los códigos ASCII.
