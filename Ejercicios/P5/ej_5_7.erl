-module(ej_5_7).
-export([createLock/0, lock/1, unlock/1, destroyLock/1, lock_loop/1]).
-export([createSem/1, semP/1, semV/1, destroySem/1, lock_sem/2]).
-export([testLock/0, testSem/0]).

createLock() ->
    spawn(ej_5_7, lock_loop, [libre]).

lock(L) ->
    L ! {lock, self()},
    receive
        ok ->
            ok
    end.

unlock(L) ->
    L ! {unlock, self()},
    receive
        ok ->
            ok
    end.

destroyLock(L) ->
    L ! {destroy, self()},
    receive
        ok ->
            ok
    end.

lock_loop(libre) ->
    receive
        {lock, PidCliente} ->
            PidCliente ! ok,
            lock_loop(ocupado);
        {destroy, PidCliente} ->
            PidCliente ! ok
    end;
lock_loop(ocupado) ->
    receive
        {unlock, PidCliente} ->
            PidCliente ! ok,
            lock_loop(libre)
    end.

createSem(N) ->
    spawn(ej_5_7, loop_sem, [N, N]).

destroySem(S) ->
    S ! {destroy, self()},
    receive
        ok ->
            ok
    end.

semV(S) ->
    S ! {post, self()},
    receive
        ok ->
            ok
    end.

semP(S) ->
    S ! {wait, self()},
    receive
        ok ->
            ok
    end.

lock_sem(N, M) ->
    receive
        {post, PidCliente} ->
            PidCliente ! ok,
            lock_sem(N, M + 1);
        {wait, PidCliente} when M > 0 ->
            PidCliente ! ok,
            lock_sem(N, M - 1);
        {destroy, PidCliente} when M == N ->
            PidCliente ! ok
    end.

f(L, W) ->
    lock(L),
    % regioncritica(),
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    unlock(L),
    W ! finished.

waiter(L, 0) ->
    destroyLock(L);
waiter(L, N) ->
    receive
        finished -> waiter(L, N - 1)
    end.

waiter_sem(S, 0) ->
    destroySem(S);
waiter_sem(S, N) ->
    receive
        finished -> waiter_sem(S, N - 1)
    end.

testLock() ->
    L = createLock(),
    W = spawn(fun() -> waiter(L, 3) end),
    spawn(fun() -> f(L, W) end),
    spawn(fun() -> f(L, W) end),
    spawn(fun() -> f(L, W) end),
    ok.

sem(S, W) ->
    semP(S),
    %regioncritica(), bueno, casi....
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    io:format("cin ~p~n", [self()]),
    io:format("sei ~p~n", [self()]),
    semV(S),
    W ! finished.

testSem() ->
    % a lo sumo dos usando io al mismo tiempo
    S = createSem(2),
    W = spawn(fun() -> waiter_sem(S, 5) end),
    spawn(fun() -> sem(S, W) end),
    spawn(fun() -> sem(S, W) end),
    spawn(fun() -> sem(S, W) end),
    spawn(fun() -> sem(S, W) end),
    spawn(fun() -> sem(S, W) end),
    ok.
