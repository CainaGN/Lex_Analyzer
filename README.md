# Lex_Analyzer
# Analisador Lexical em C

## Descrição
Este projeto implementa um analisador lexical em C que processa um arquivo-fonte e identifica tokens, como palavras-chave, identificadores, números, operadores, delimitadores e caracteres especiais. O analisador utiliza um grafo para armazenar os tokens extraídos do código-fonte.

## Funcionalidades
- Identifica palavras-chave da linguagem C.
- Reconhece identificadores (nomes de variáveis e funções).
- Detecta números inteiros, hexadecimais e floats.
- Processa operadores matemáticos e lógicos.
- Identifica delimitadores como espaços, tabulações e pontuação.
- Trata caracteres de escape como `\n` e `\t`.
- Ignora comentários de linha (`//`) e de bloco (`/* ... */`).
- Armazena tokens em um grafo para posterior análise.

## Estrutura do Código
- **`Node`**: Estrutura de um nó do grafo para armazenar caracteres de tokens.
- **`Graph`**: Representa um grafo de tokens, com uma lista de adjacência.
- **`create_Node`**: Cria um novo nó com um caractere.
- **`create_Graph`**: Inicializa o grafo de tokens.
- **`add_Edge`**: Adiciona um caractere a um token no grafo.
- **`lexical_Analyzer`**: Função principal que processa o arquivo-fonte e identifica tokens.
- **`print_Graph`**: Exibe os tokens armazenados no grafo.
- **`LookAhead`**: Lê o próximo caractere sem consumi-lo.
- **Funções auxiliares**: `is_Delimiter`, `is_Operator`, `is_Alpha`, `is_Digit`, `is_Alpha_num`, `is_Keyword`.

## Entrada e Saída
### Entrada
- O programa lê um arquivo chamado `codigo.c` contendo o código-fonte a ser analisado.

### Saída
- Os tokens identificados são gravados no arquivo `output.txt`, com a classificação de cada token.
- O grafo contendo os tokens é impresso no console.

## Como Executar
1. Compile o código com `gcc`:
   ```sh
   gcc analisador_lexico.c -o analisador
   ```
2. Execute o programa:
   ```sh
   ./analisador
   ```
3. Verifique a saída no arquivo `output.txt`.

## Exemplo de Saída
**Entrada (`codigo.c`)**:
```c
int soma(int a, int b) {
    return a + b;
}
```

**Saída (`output.txt`)**:
```
Palavra chave: int
Identificador: soma
Delimitador: (
Identificador: a
Delimitador: ,
Identificador: b
Delimitador: )
Delimitador: {
Palavra chave: return
Identificador: a
Operador: +
Identificador: b
Delimitador: ;
Delimitador: }
```

## Possíveis Melhorias
- Melhor tratamento de erros lexicais.
- Implementar análise sintática.
- Melhorar a visualização do grafo.


