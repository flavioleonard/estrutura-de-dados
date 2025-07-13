# Estrutura de Dados: Tabela Hash com Endereçamento Aberto

## Descrição Geral
Esta pasta contém a implementação de uma tabela hash utilizando endereçamento aberto (sondagem linear) para armazenamento, busca e remoção de registros de pessoas (nome, CPF, nota). Os dados são persistidos em arquivo binário (`tabela_hash.dat`).

## Estrutura dos Arquivos
- **enderecamento_aberto.c**: Inicializa a tabela hash, insere registros a partir de um arquivo de origem (`registros.dat`) e contabiliza colisões.
- **operacoes_tabela.c**: Implementa um menu interativo para inserir, buscar, remover e imprimir registros diretamente na tabela hash.
- **tabela_hash.dat**: Arquivo binário que armazena os registros na tabela hash.

## Estrutura do Registro
```c
#define TAM_NOME 50

typedef struct Registro {
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} TRegistro;
```

## Algoritmo de Hash
- Utiliza sondagem linear para resolver colisões.
- Função hash:
  ```c
  int hash(long long cpf, int m, int k) {
      srand(cpf);
      return (rand() % m + k) % m;
  }
  ```
  - `cpf`: chave do registro
  - `m`: tamanho da tabela (100000)
  - `k`: tentativa atual (para sondagem linear)

## Como Compilar
Abra o terminal na pasta `tabela_hash` e execute:

```
gcc enderecamento_aberto.c -o enderecamento_aberto.exe

gcc operacoes_tabela.c -o operacoes_tabela.exe
```

## Como Executar
### 1. Inicializar e Popular a Tabela Hash
O arquivo `enderecamento_aberto.exe` inicializa a tabela hash e insere registros a partir de `../registros.dat`:

```
./enderecamento_aberto.exe
```

- Cria o arquivo `tabela_hash.dat` com todos os registros do arquivo de origem.
- Mostra o total de colisões ocorridas durante a inserção.

### 2. Menu Interativo
O arquivo `operacoes_tabela.exe` permite manipular a tabela hash via menu:

```
./operacoes_tabela.exe
```

#### Opções do Menu
1. **Inserir registro**: Solicita CPF, nome e nota, e insere na tabela hash.
2. **Buscar registro**: Solicita CPF e exibe o registro correspondente, se existir.
3. **Remover registro**: Solicita CPF e remove o registro da tabela hash.
4. **Imprimir tabela**: Exibe todos os registros não vazios da tabela hash.
0. **Sair**: Encerra o programa.

## Observações
- O arquivo `tabela_hash.dat` deve estar presente na mesma pasta dos executáveis.
- O tamanho da tabela é fixo em 100.000 posições.
- O campo `cpf` igual a zero indica posição vazia.
- O sistema não trata remoção lógica (marcações de "removido"), apenas zera o registro.
- O arquivo de origem para carga inicial deve estar em `../registros.dat`.

## Autores
- Implementação desenvolvida para fins didáticos na disciplina de Estrutura de Dados (UFF).
