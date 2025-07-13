# Gerador de Registros Aleatórios

## Descrição Geral
Este módulo contém um gerador de registros aleatórios para uso em testes de estruturas de dados, especialmente para a tabela hash e outras estruturas que utilizam o arquivo `registros.dat` como base de dados.

O programa gera registros compostos por nome, CPF (único) e nota, salvando-os em um arquivo binário. Também permite a leitura e visualização dos registros gerados.

## Estrutura do Registro
```c
#define TAM_NOME 50

typedef struct {
    char nome[TAM_NOME];
    long long int cpf;
    int nota;
} Registro;
```

- **nome**: Nome completo (nome + sobrenome), preenchido até 50 caracteres.
- **cpf**: Número de CPF gerado aleatoriamente, garantindo unicidade dos 9 primeiros dígitos.
- **nota**: Valor inteiro entre 0 e 100.

## Funcionalidades
- **Geração de registros aleatórios**: Cria 10.000 registros únicos e armazena em `registros.dat`.
- **Leitura de registros**: Permite visualizar os registros salvos no arquivo.

## Como Compilar
Abra o terminal na pasta do projeto e execute:

```
gcc gera_registros.c -o gera_registros.exe
```

## Como Executar
No terminal, execute:

```
./gera_registros.exe
```

## Menu de Opções
Ao rodar o programa, o seguinte menu será exibido:

```
MENU:
1 - Gerar novos registros
2 - Ler registros existentes
0 - Sair
Escolha:
```

- **1 - Gerar novos registros**: Cria 10.000 registros aleatórios e salva em `registros.dat`.
- **2 - Ler registros existentes**: Lê e exibe os registros do arquivo `registros.dat`.
- **0 - Sair**: Encerra o programa.

## Observações
- O arquivo `registros.dat` é sobrescrito a cada nova geração.
- Os nomes são compostos a partir de listas de nomes e sobrenomes comuns.
- O CPF é gerado como um número inteiro de 11 dígitos, garantindo unicidade dos 9 primeiros dígitos.
- O campo `nota` é um valor aleatório entre 0 e 100.
- O programa imprime o progresso a cada 1000 registros gerados.

## Uso Típico
1. Gere os registros com a opção 1.
2. Utilize o arquivo `registros.dat` como entrada para outros módulos do projeto (ex: tabela hash).
3. Consulte os registros gerados usando a opção 2, se desejar.

## Autor
- Implementação didática para a disciplina de Estrutura de Dados (UFF).
