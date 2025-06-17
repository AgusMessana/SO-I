-module(p2p_node).
-behaviour(gen_server).

-export([start_link/1, init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2]).
-export([node_id/0, list_files/0, stop/0]).

%% API
start_link(NodeId) ->
    gen_server:start_link({local, ?MODULE}, ?MODULE, [NodeId], []).

node_id() ->
    gen_server:call(?MODULE, node_id).

list_files() ->
    gen_server:call(?MODULE, list_files).

stop() ->
    gen_server:cast(?MODULE, stop).

%% Callbacks
init([NodeId]) ->
    %% Crear carpetas si no existen
    file:make_dir("compartida"),
    file:make_dir("descargas"),
    
    %% Escanear archivos compartidos
    {ok, Files} = file:list_dir("compartida"),
    
    %% Registrar nodo en el archivo de consenso
    register_in_registry(NodeId),
    
    %% Iniciar servidor TCP
    {ok, ListenSocket} = gen_tcp:listen(12345, [binary, {active, false}, {reuseaddr, true}]),
    spawn_link(fun() -> accept_connections(ListenSocket) end),
    
    %% Iniciar CLI
    spawn_link(fun() -> cli_loop() end),
    
    {ok, #{node_id => NodeId,
           shared_files => Files,
           known_nodes => [],
           listen_socket => ListenSocket}}.

handle_call(node_id, _From, State) ->
    {reply, maps:get(node_id, State), State};
handle_call(list_files, _From, State) ->
    {reply, maps:get(shared_files, State), State};
handle_call(_Request, _From, State) ->
    {reply, ok, State}.

handle_cast(stop, State) ->
    {stop, normal, State};
handle_cast(_Msg, State) ->
    {noreply, State}.

handle_info(_Info, State) ->
    {noreply, State}.

terminate(_Reason, _State) ->
    ok.

%% Funciones internas
register_in_registry(NodeId) ->
    {ok, Hostname} = inet:gethostname(),
    NodeInfo = #{ip => Hostname, port => 12345},
    
    case file:read_file("nodes_registry.json") of
        {ok, Bin} ->
            Nodes = jsx:decode(Bin, [return_maps]),
            UpdatedNodes = Nodes#{NodeId => NodeInfo},
            file:write_file("nodes_registry.json", jsx:encode(UpdatedNodes));
        {error, enoent} ->
            file:write_file("nodes_registry.json", jsx:encode(#{NodeId => NodeInfo}))
    end.

accept_connections(ListenSocket) ->
    case gen_tcp:accept(ListenSocket) of
        {ok, Socket} ->
            spawn(fun() -> handle_connection(Socket) end),
            accept_connections(ListenSocket);
        {error, closed} ->
            ok
    end.

handle_connection(Socket) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Data} ->
            %% Procesar mensaje (implementar según protocolo)
            io:format("Received: ~p~n", [Data]),
            gen_tcp:send(Socket, <<"OK">>),
            handle_connection(Socket);
        {error, closed} ->
            ok
    end.

cli_loop() ->
    io:format("Comandos disponibles: id_nodo, listar_mis_archivos, salir~n"),
    case io:get_line("> ") of
        "id_nodo\n" ->
            io:format("ID: ~p~n", [node_id()]),
            cli_loop();
        "listar_mis_archivos\n" ->
            Files = list_files(),
            io:format("Archivos compartidos: ~p~n", [Files]),
            cli_loop();
        "salir\n" ->
            stop();
        _ ->
            io:format("Comando no reconocido~n"),
            cli_loop()
    end.