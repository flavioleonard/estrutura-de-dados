# Estruturas de Dados: Projeto de Tabela Hash e Árvore B+

## Visão Geral
Este repositório reúne implementações didáticas de duas importantes estruturas de dados: **Tabela Hash com Endereçamento Aberto** e **Árvore B+**. O objetivo é permitir testes, comparações e estudos práticos sobre armazenamento, busca e manipulação eficiente de registros em arquivos.

O fluxo principal do projeto é:
1. **Gerar registros aleatórios** (base de dados comum)
2. **Carregar esses registros** em uma estrutura (Hash ou B+)
3. **Manipular e consultar** os dados via menus interativos

---

## 1. Gerador de Registros Aleatórios
- **Arquivo:** `gera_registros.c`
- **Executável:** `gera_registros.exe`
- **Saída:** `registros.dat`

Gera 10.000 registros únicos (nome, CPF, nota) e salva em `registros.dat`, que serve como base de dados para as demais estruturas.

### Como usar
```sh
gcc gera_registros.c -o gera_registros.exe
./gera_registros.exe
```
Escolha a opção 1 para gerar os registros.

---

## 2. Tabela Hash com Endereçamento Aberto
- **Pasta:** `tabela_hash/`
- **Arquivos principais:**
  - `enderecamento_aberto.c` (carga inicial a partir de `registros.dat`)
  - `operacoes_tabela.c` (menu interativo)
- **Arquivo de dados:** `tabela_hash.dat`

### Como usar
1. **Inicialize a tabela hash e carregue os registros:**
   ```sh
   cd tabela_hash
   gcc enderecamento_aberto.c -o enderecamento_aberto.exe
   ./enderecamento_aberto.exe
   ```
   Isso criará `tabela_hash.dat` com todos os registros de `../registros.dat`.

2. **Menu interativo para manipulação:**
   ```sh
   gcc operacoes_tabela.c -o operacoes_tabela.exe
   ./operacoes_tabela.exe
   ```
   Opções do menu:
   - Inserir registro
   - Buscar registro
   - Remover registro
   - Imprimir tabela

---

## 3. Árvore B+
- **Pasta:** `arvore_b+/`
- **Arquivos principais:**
  - `main_registros.c`, `opera_arvorebmais.c`, `functions.c`, `TARVBP.c`
- **Arquivos de dados:** `bplus_dados.dat`, `bplus_index.dat`, `raiz.dat`

### Como usar
1. **Compile os arquivos necessários:**
   ```sh
   cd arvore_b+
   gcc main_registros.c opera_arvorebmais.c functions.c TARVBP.c -o main_registros.exe
   ```
2. **Execute o programa principal:**
   ```sh
   ./main_registros.exe
   ```
   O menu interativo permitirá inserir, buscar, remover e listar registros na Árvore B+.

> **Importante:**
> - O arquivo `registros.dat` gerado pelo `gera_registros.exe` é a base de dados para **tanto a tabela hash quanto a árvore B+**.
> - Sempre gere ou atualize `registros.dat` antes de testar as estruturas.

---

## Estrutura do Projeto
```
Estruturas de Dados/
├── gera_registros.c
├── gera_registros.exe
├── registros.dat
├── tabela_hash/
│   ├── enderecamento_aberto.c
│   ├── operacoes_tabela.c
│   ├── tabela_hash.dat
│   └── ...
└── arvore_b+/
    ├── main_registros.c
    ├── opera_arvorebmais.c
    ├── functions.c
    ├── TARVBP.c
    ├── bplus_dados.dat
    ├── bplus_index.dat
    ├── raiz.dat
    └── ...
```

---

## Recomendações de Uso
- Sempre **gere os registros** antes de testar as estruturas.
- Cada estrutura possui seu próprio menu interativo para facilitar testes e manipulação.
- Consulte os READMEs específicos em cada pasta para detalhes de implementação e opções avançadas.

---

## Créditos
Implementação didática para a disciplina de Estrutura de Dados (UFF).

---

Bons estudos e experimentos!
