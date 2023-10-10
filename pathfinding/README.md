# Pathfinding

Esse modulo é dedicado na lógica geral do sistema de busca.
Atualmente, há uma simulação do robo em funcionamento, onde o rato tenta alcançar um objetivo
O rato não tem conhecimento do labirinto, apenas se nos seus arredores há uma parede ou um caminho livre.
Dessa forma, o rato deve explorar o mundo em busca do objetivo.

O arquivo `mouse.py` representa o rato que busca pelo objetivo em um mundo desconhecido.
O rato só tem conhecimento dos seus arredores, então é construido um mapa conforme o rato anda.
A movimentação acontece por meio de algoritmos de grafos aplicados ao mapa, onde é utilizado o algorítimo Depth First Search.

## Uso

```
$ bash run.sh <mapa>

# Exemplo
$ bash run.sh maps/01.map
```
