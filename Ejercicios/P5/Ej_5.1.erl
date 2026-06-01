-module(ej1).
-export([init/0]).
-export([nombre /1]).
-export([apellido/1]).
-export([filtrar_por_apellido/2]).
-export([tuple_test/2]).

match_test () ->
    {A,B} = {5,4},
    %{C,C} = {5,4},
    {B,A} = {4,5},
    {D,D} = {5,5}.

string_test () -> [
    helloworld == 'helloworld',
    "helloworld" < 'helloworld',
    helloworld == "helloworld",
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld",
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100},
    [104,101,108,108,111,119,111,114,108,100] > 1,
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"].

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido ({persona, {nombre, _Nombre}, {apellido, Apellido}}) ->
    Apellido.

nombre ({persona, {'nombre', Nombre},{'apellido', _Apellido}}) ->
    Nombre.


filtrar_por_apellido([], _Apellido) -> 
    [];
filtrar_por_apellido([P|Resto], Apellido)->
    case apellido(P) == Apellido of
        true ->
            [nombre(P) | filtrar_por_apellido(Resto, Apellido)];
        false ->
            filtrar_por_apellido(Resto, Apellido)
    end.

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia"),
    io:fwrite("Mi lista es: ~p~n", [Garcias]).

%a) {A,B} valido
%   {C,C} invalido
%   {B,A} valido
%   {D,D} valido
%
% b)
%