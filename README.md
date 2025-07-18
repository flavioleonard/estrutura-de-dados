# Estruturas de Dados: Projeto de Tabela Hash e Árvore B+

## Visão Geral
Este repositório reúne implementações didáticas de três importantes estruturas de dados: **Tabela Hash com Endereçamento Aberto**, **Árvore B+** e **Heap**. 
Trabalho para a matéria de Estrutura de Dados lecionada pelo professor Luis Antonio Kowada.

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
Escolha a opção 2 para ver os registros que foram gerados. 

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
   - isso pode demorar alguns segundos
<img width="1065" height="148" alt="image" src="https://github.com/user-attachments/assets/4144c340-7a47-4724-8a8f-05e722562ed7" />

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
   - aconselho usar os dois arquivos .dat que já estão aqui nesse repositório devido ao tempo que a arvore demora a ser construida 
   ```
   O menu interativo permitirá inserir, buscar, remover na Árvore B+.

> **Importante:**
> - O arquivo `registros.dat` gerado pelo `gera_registros.exe` é a base de dados para **tanto a tabela hash quanto a árvore B+**.
> - Sempre gere ou atualize `registros.dat` antes de testar as estruturas.

---

4. **Busque** um CPF de registros.dat. Voce pode encontrar o cpf abrindo gera_registros.exe e digitando 2. 
<img width="1317" height="556" alt="image" src="https://github.com/user-attachments/assets/81c264a6-3922-42a2-b791-9c4991fe447b" />

5. **Inserindo** registro e buscando para conferir que foi inserido 
<img width="616" height="424" alt="image" src="https://github.com/user-attachments/assets/c78c3152-e3aa-4005-a1d5-8e98e9caaeac" />
<img width="628" height="492" alt="image" src="https://github.com/user-attachments/assets/22101503-6174-4115-8741-b9fe51f6f90b" />

6. **Apagando** registro e tentando buscar -> registro nao encontrado
<img width="682" height="616" alt="image" src="https://github.com/user-attachments/assets/ea050524-0e18-4ec1-8cef-ac59bbabf4a8" />


## 4. Heap (Heap Máxima em Arquivo)

O módulo de Heap permite manipular um ranking de registros (nome, CPF, nota) diretamente em arquivo, utilizando a estrutura de heap máxima. Todas as operações são feitas em disco, simulando um cenário realista de grandes volumes de dados.

### Arquivo principal
- `heap_arquivo.c`
- **Executável:** `heap_arquivo.exe`
- **Arquivo de dados:** `heap.dat` (gerado a partir de `registros.dat`)

### Como usar
1. **Compile o arquivo:**
   ```sh
   gcc heap_arquivo.c -o heap_arquivo.exe
   ```
2. **Execute o programa:**
   ```sh
   ./heap_arquivo.exe
   ```

### Funcionalidades do menu interativo

O menu do programa oferece as seguintes opções:

1. **Gerar heap máxima em disco**
   - Copia todos os registros de `registros.dat` para `heap.dat` e constrói a heap máxima.
   - Deve ser a primeira opção utilizada.
<img width="1070" height="306" alt="image" src="https://github.com/user-attachments/assets/f80edb37-8253-4817-b1bd-0d6d858a429a" />


2. **Mostrar ranking (top k notas)**
   - Permite visualizar os `k` registros com as maiores notas diretamente do arquivo heap.
   - O usuário informa o valor de `k`.
<img width="636" height="921" alt="image" src="https://github.com/user-attachments/assets/6ec28cae-b20e-4f6f-9415-5e56b431444d" />


3. **Inserir novo registro na heap**
   - Permite inserir um novo registro (nome, CPF, nota) na heap em disco.
   - O registro é adicionado ao final do arquivo e a operação de subida é realizada para manter a propriedade de heap máxima.
<img width="811" height="336" alt="image" src="https://github.com/user-attachments/assets/31814835-2561-4fdc-b84c-6cdef81adeaf" />

4. **Buscar**
- Permite buscar o registro na heap
<img width="1017" height="320" alt="image" src="https://github.com/user-attachments/assets/e2f42064-af31-40f8-acab-6229a0a22cdb" />



4. **Excluir raiz da heap (maior nota)**
   - Remove o registro com a maior nota (raiz da heap).
   - O último registro do arquivo substitui a raiz e a operação de descida é realizada para restaurar a heap.
<img width="700" height="154" alt="image" src="https://github.com/user-attachments/assets/0ba74d29-54b6-4b3b-9e62-38262708a258" />
Veja que abaixo o primeiro foi eliminado era o Erick Cardoso
<img width="1125" height="510" alt="image" src="https://github.com/user-attachments/assets/4cd8ef27-c14a-4b0a-8c77-991d8549ba69" />




0. **Sair**
   - Encerra o programa.

#### Observações importantes
- Sempre gere a heap máxima antes de usar as demais opções.
- O valor de `n` (quantidade de registros na heap) é atualizado automaticamente após inserções e exclusões.
- O arquivo `heap.dat` é manipulado diretamente, simulando operações eficientes para grandes volumes de dados.

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
