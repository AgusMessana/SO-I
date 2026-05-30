/**
 * La implementación del inciso d) convierte al sistema en un servidor de turnos.
 * Al modificar al agente para que despierte directamente a un fumador específico, este asume el rol de un despachador centralizado que asigna un turno unívoco a cada hilo.
 * Esto simplifica enormemente el programa y elimina cualquier posibilidad de deadlock o livelock, ya que los fumadores dejan de competir a ciegas por recursos genéricos y solo se despiertan cuando tienen garantizado su turno para fumar.
 */
