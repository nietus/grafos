from collections import defaultdict
import sys
import threading
import time

sys.setrecursionlimit(1 << 25)
threading.stack_size(1 << 27)

tempo_atual = 0
arq = 'data/graph_x.txt'

def ler_arestas(arquivo):
    arestas = []
    vertice_maximo = 0 
    with open(arquivo, 'r') as f:
        linhas = f.readlines()
        numero_vertices_declarado, numero_arestas = map(int, linhas[0].strip().split())
        print(f"Total de Vértices Declarados: {numero_vertices_declarado}, Total de Arestas: {numero_arestas}")
        for linha in linhas[1:]:
            if not linha.strip():
                continue  # Ignora linhas vazias
            u, v = map(int, linha.strip().split())
            arestas.append((u - 1, v - 1))  # Subtrai 1 para indexação 0-based
            vertice_maximo = max(vertice_maximo, u, v)
    numero_vertices = max(numero_vertices_declarado, vertice_maximo)
    if numero_vertices > numero_vertices_declarado:
        print(f"Aviso: O número de vértices especificado ({numero_vertices_declarado}) é menor que o máximo encontrado ({vertice_maximo}).")
    return arestas, numero_vertices_declarado

def encontrar_pontos_articulacao_e_blocos(grafo, numero_vertices):
    global tempo_atual
    tempo_atual = 0  # Reseta o tempo atual
    visitado = [False] * numero_vertices
    tempo_descoberta = [-1] * numero_vertices
    tempo_mais_baixo = [-1] * numero_vertices
    pais = [-1] * numero_vertices
    pontos_articulacao = set()  # Conjunto para armazenar pontos de articulação
    stack = []
    blocos = []  # Lista para armazenar os blocos biconexos

    def dfs(u):
        nonlocal grafo, visitado, tempo_descoberta, tempo_mais_baixo, pais, pontos_articulacao, stack, blocos
        global tempo_atual
        visitado[u] = True
        tempo_descoberta[u] = tempo_mais_baixo[u] = tempo_atual
        tempo_atual += 1
        filhos = 0

        for v in grafo[u]:
            if not visitado[v]:
                pais[v] = u
                filhos += 1
                stack.append((u, v))  # Armazena a aresta na pilha
                dfs(v)

                tempo_mais_baixo[u] = min(tempo_mais_baixo[u], tempo_mais_baixo[v])

                # Verifica se u é um ponto de articulação
                if (pais[u] == -1 and filhos > 1) or (pais[u] != -1 and tempo_mais_baixo[v] >= tempo_descoberta[u]):
                    pontos_articulacao.add(u)
                    # Forma um bloco biconexo
                    bloco = []
                    vertices_bloco = set()
                    while True:
                        e = stack.pop()
                        bloco.append(e)
                        vertices_bloco.update(e)
                        if e == (u, v) or e == (v, u):
                            break
                    blocos.append(bloco)
            elif v != pais[u]:
                tempo_mais_baixo[u] = min(tempo_mais_baixo[u], tempo_descoberta[v])
                if tempo_descoberta[v] < tempo_descoberta[u]:
                    stack.append((u, v))

    for i in range(numero_vertices):
        if not visitado[i]:
            dfs(i)
            # Se restarem arestas na pilha, formam um bloco biconexo
            if stack:
                bloco = []
                vertices_bloco = set()
                while stack:
                    e = stack.pop()
                    bloco.append(e)
                    vertices_bloco.update(e)
                if len(vertices_bloco) > 2:
                    blocos.append(bloco)

    return pontos_articulacao, blocos

def main():
    start_time = time.perf_counter()  # Inicia a contagem do tempo total

    arestas, numero_vertices = ler_arestas(arq)

    # Construir o grafo
    grafo = defaultdict(list)
    for u, v in arestas:
        grafo[u].append(v)
        grafo[v].append(u)

    # Encontrar pontos de articulação e blocos biconexos
    pontos_articulacao, blocos = encontrar_pontos_articulacao_e_blocos(grafo, numero_vertices)

    # Exibir pontos de articulação
    print("\nPontos de Articulação:")
    if pontos_articulacao:
        pontos_articulacao_exibicao = [str(v + 1) for v in sorted(pontos_articulacao)]
        print(' '.join(pontos_articulacao_exibicao))
    else:
        print("Nenhum ponto de articulação encontrado.")

    # Exibir blocos (componentes biconexas)
    print(f"\nTotal de blocos (componentes biconexas): {len(blocos)}\n")
    bloco_idx = 1
    for bloco in blocos:
        vertices_bloco = set()
        for u, v in bloco:
            vertices_bloco.add(u + 1)
            vertices_bloco.add(v + 1)
        print(f"Bloco {bloco_idx}: Vértices {sorted(vertices_bloco)}")
        bloco_idx += 1

    end_time = time.perf_counter()
    total_time = end_time - start_time
    print(f"\nTempo total de execução: {total_time:.4f} segundos")

if __name__ == "__main__":
    threading.Thread(target=main).start()