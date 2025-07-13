# Árvore B+

Esta pasta contém a implementação de uma Árvore B+ para armazenamento, busca e manipulação eficiente de registros em arquivos binários. O projeto é didático e utiliza como base de dados o arquivo `registros.dat` gerado pelo módulo de geração de registros.

## Estrutura dos Arquivos
- **main_registros.c**: Programa principal com menu interativo para manipulação da Árvore B+.
- **opera_arvorebmais.c**: Operações principais sobre a Árvore B+ (inserção, busca, remoção, etc).
- **functions.c / functions.h**: Funções auxiliares e definições.
- **TARVBP.c / TARVBP.h**: Implementação da estrutura da Árvore B+.
- **bplus_dados.dat, bplus_index.dat, raiz.dat**: Arquivos binários para armazenamento dos dados, índices e raiz da árvore.

## Como Compilar

Abra o terminal na pasta `arvore_b+` e compile separadamente cada programa, pois ambos possuem uma função `main()`:

- Para o programa de carga inicial e manipulação básica:
  ```
  gcc -o main_registros.exe main_registros.c TARVBP.c functions.c
  ```
- Para o menu interativo de operações na árvore B+:
  ```
  gcc -o opera_arvorebmais opera_arvorebmais.c TARVBP.c functions.c
  ```


## Como Executar

- Para executar o programa de carga inicial:
  ```
  ./main_registros.exe
  ```
- Para executar o menu interativo:
  ```
  ./opera_arvorebmais.exe
  ```

## Menu Interativo

O programa apresenta um menu para:
- Inserir registros
- Buscar registros

Consulte as opções exibidas no terminal para navegar pelas funcionalidades.

## Observações
- O arquivo `registros.dat` (gerado na pasta principal) é utilizado como base de dados para inserção inicial na Árvore B+.
- Os arquivos binários de dados e índices são criados/atualizados automaticamente.
- Recomenda-se gerar ou atualizar `registros.dat` antes de testar a Árvore B+.

## Recomendações
- Sempre gere os registros antes de popular a Árvore B+.
- Consulte os comentários nos arquivos `.c` e `.h` para detalhes de implementação.

---

Implementação didática para a disciplina de Estrutura de Dados (UFF).
