#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct Node 
{
    char data;
    struct Node* nextChar;
} Node;

typedef struct Graph 
{
    int numVertices;
    Node** adjLists;
} Graph;

Node* create_Node(char data) 
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->nextChar = NULL;
    return newNode;
}

Graph* create_Graph(int numVertices) 
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));

    for (int i = 0; i < numVertices; i++) 
    {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

void add_Edge(Graph* graph, int src, char data) 
{
    Node* newNode = create_Node(data);    
    newNode->nextChar = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

int LookAhead(FILE *file) 
{
    int nextChar = fgetc(file);
    ungetc(nextChar, file);
    return nextChar;
}

bool is_Delimiter(char ch) 
{
    char delimiters[] = " \t\n(){}[],;\".";

    for (int i = 0; i < strlen(delimiters); i++) 
    {
        if (ch == delimiters[i])
        {
            return true;
        }       
    }
    return false;
}

bool is_Operator(char ch) 
{
    char operators[] = "+-*/%<>=!&|^~?:";
    for (int i = 0; i < strlen(operators); i++) 
    {
        if (ch == operators[i]) 
        {
            return true;
        }
    }
    return false;
}

int is_Alpha(char ch) 
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int is_Digit(char ch) 
{
    return ch >= '0' && ch <= '9';
}

int is_Alpha_num(char ch) 
{
    return is_Alpha(ch) || is_Digit(ch) || ch == '_';
}

int is_Keyword(char* word) 
{
    char keywords[][20] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}


void print_Tokens(Node* node)
{
    if (node == NULL) 
        return;

    print_Tokens(node->nextChar);
    printf("%c", node->data);
}

void print_Graph(Graph* graph) 
{
    for (int i = 0; i < graph->numVertices; i++) 
    {
        printf("Vertice %d: ", i);
        print_Tokens(graph->adjLists[i]);
        printf("\n");
    }
}


void lexical_Analyzer(FILE* file) 
{
    int tokenCount = 0;
    int graphCapacity = 5; // Capacidade inicial do grafo. Aumenta conforme o grafo cresce
    Graph* tokenGraph = create_Graph(graphCapacity);

    FILE* f_output = fopen("output.txt", "w");
    if (f_output == NULL) 
    {
        printf("Erro na abertura de arquivo.\n");
        return 1;
    }


    
    char ch;
    while ((ch = fgetc(file)) != EOF) 
    {
        
        if (tokenCount >= graphCapacity) 
        {
            // Aumenta a capacidade do grafo
            int newCapacity = graphCapacity + 5;
            Node** newAdjLists = realloc(tokenGraph->adjLists, newCapacity * sizeof(Node*));
            if (newAdjLists == NULL) 
            {
                printf("Falha na alocação de memória.\n");
                break;
            }
            tokenGraph->adjLists = newAdjLists;
            graphCapacity = newCapacity;
        }
   
        if (ch == ' ' || ch == '\t' || ch == '\n')
            continue;
        

        if (ch == '/' && LookAhead(file) == '*')
                {
                    while((ch = fgetc(file)) !=  '/')
                    {
                        continue;
                    }
                    continue;
                }

        
        // Identificadores e palavras-chave
        if (is_Alpha(ch) || ch == '_') 
        {
            char buff[100];
            int j = 0;
            while (is_Alpha_num(ch) || ch == '_') 
            {
                add_Edge(tokenGraph, tokenCount, ch);
                buff[j] = ch;
                ch = fgetc(file);
                j++;
            }
            ungetc(ch, file);
   
            buff[j] = '\0';


            if (is_Keyword(buff))
            {
                fprintf(f_output, "Palavra chave: %s\n", buff);
            }
            else
            {
                fprintf(f_output, "Identificador: %s\n", buff);
            }

            tokenCount++;
            tokenGraph->numVertices = tokenCount;
            
        }
        // Números 
        else if (is_Digit(ch)) 
        {
            int j = 0, i = 0;
            int hasDot = 0;
            int hasExp = 0;
            int isFloat = 0, isHexa = 0;
            char buff[100];

            // Verifica hexadecimal
            if(ch == 48 && LookAhead(file) == 120)
            {
                
                add_Edge(tokenGraph, tokenCount, ch);
                buff[i] = ch;
                i++;
                ch = fgetc(file);
                add_Edge(tokenGraph, tokenCount, ch);
                buff[i] = ch;
                ch = fgetc(file);
                i++;
                while(is_Digit(ch))
                {
                    add_Edge(tokenGraph, tokenCount, ch);
                    buff[i] = ch;
                    i++;
                    ch = fgetc(file);
                }
                fprintf(f_output, "Hexadecimal: %s\n", buff);
                isHexa = 1;
            }
            
            while (is_Digit(ch) || ch == '.' || ch == 'e' || ch == 'E' || ch == '+' || ch == '-') 
            {
                if (ch == '.') 
                {
                    if (hasDot || hasExp) 
                    {
                        // Mais de um ponto ou ponto após "e" ou "E"
                        break;
                    }
                    hasDot = 1;
                    isFloat = 1;
                }
                else if (ch == 'e' || ch == 'E') 
                {
                    if (hasExp) 
                    {
                        // Mais de um "e" ou "E"
                        break;
                    }
                    hasExp = 1;
                    isFloat = 1;
                }
                else if (ch == '+' || ch == '-') 
                {
                    if (!hasExp) 
                    {
                        // Sinal de "+" ou "-" fora da notação científica
                        break;
                    }
                }

                add_Edge(tokenGraph, tokenCount, ch);
                buff[j] = ch;
                ch = fgetc(file);
                j++;
            
            }
            ungetc(ch, file);
            buff[j] = '\0';



            if (isFloat) 
            {
                fprintf(f_output, "Float: %s\n", buff);
            }
            else if(!isHexa)
            {
            fprintf(f_output, "Inteiro: %s\n", buff);
            }

            /*printToken(tokenGraph->adjLists[tokenCount]);
            printf("\n");*/
            tokenCount++;
            tokenGraph->numVertices = tokenCount;
        }

        // Operadores
        else if (is_Operator(ch)) 
        {      
            char nextChar = LookAhead(file);
            char token[3];
            token[0] = ch;


            if (ch == '/' && LookAhead(file) == 42)
                {
                    while ((ch = fgetc(file)) != '*' && LookAhead(file) != '/')
                    {
                        continue;
                    }
                    continue;
                }

            if ((ch == '%' && LookAhead(file) == 's') || (ch == '%' && LookAhead(file) == 'i') || 
                (ch == '%' && LookAhead(file) == 'x') || (ch == '%' && LookAhead(file) == 'd') ||
                (ch == '%' && LookAhead(file) == 'f') || (ch == '%' && LookAhead(file) == 'c') ||
                (ch == '%' && LookAhead(file) == 'p') || (ch == '%' && LookAhead(file) == 'e') ||
                (ch == '%' && LookAhead(file) == 'g') || (ch == '%' && LookAhead(file) == 'E') ||
                (ch == '%' && LookAhead(file) == 'G'))
            {
                token[1] = LookAhead(file);
                token[2] = '\0';
                ch = fgetc(file);

                for (int i = 0; i < strlen(token); i++) 
                    {
                        add_Edge(tokenGraph, tokenCount, token[i]);
                        
                    }
                    fprintf(f_output, "Especificador: %s\n", token);
                    tokenCount++;
                    tokenGraph->numVertices = tokenCount;
                    continue;
            }

                
            if (is_Operator(nextChar)) 
            {

                if ((ch == '+' && nextChar == '+') ||
                    (ch == '-' && nextChar == '-') ||
                    (ch == '&' && nextChar == '&') ||
                    (ch == '|' && nextChar == '|') ||
                    (ch == '=' && nextChar == '=') ||
                    (ch == '!' && nextChar == '=') ||
                    (ch == '<' && nextChar == '=') ||
                    (ch == '>' && nextChar == '=') ||
                    (ch == '+' && nextChar == '=') ||
                    (ch == '-' && nextChar == '=') ||
                    (ch == '*' && nextChar == '=') ||
                    (ch == '/' && nextChar == '=') ||
                    (ch == '%' && nextChar == '=') ||
                    (ch == '&' && nextChar == '=') ||
                    (ch == '|' && nextChar == '=') ||
                    (ch == '^' && nextChar == '=') ||
                    (ch == '<' && nextChar == '<') ||
                    (ch == '>' && nextChar == '>'))
                {
                    token[1] = LookAhead(file);
                    token[2] = '\0';
                    ch = fgetc(file);

                    // Adiciona os caracteres do token no grafo
                    for (int i = 0; i < strlen(token); i++) 
                    {
                        add_Edge(tokenGraph, tokenCount, token[i]);
                        
                    }
                    fprintf(f_output, "Operador: %s\n", token);
                    tokenCount++;
                    tokenGraph->numVertices = tokenCount;
                
                } else if (ch == '/' && nextChar == '/')
                {
                    while((ch = fgetc(file)) != '\n')
                    {
                        continue;
                    }
                    continue;
                }
                
                else 
                {
                    for (int i = 0; i < strlen(token); i++) 
                    {
                        add_Edge(tokenGraph, tokenCount, token[i]);
                    }
                    
                    fprintf(f_output, "Combinação de tokens inválida: %s\n", token);
                    tokenCount++;
                    tokenGraph->numVertices = tokenCount;
                    }
            }
           
                
            else 
            {
                token[1] = '\0';
                add_Edge(tokenGraph, tokenCount, ch);
                fprintf(f_output, "Operador: %s\n", token);
                tokenCount++;
                tokenGraph->numVertices = tokenCount;
            }         
        }

        else if (ch == 92) 
        { // Verifica se é uma barra invertida '\'
            char token[3];
            token[0] = ch;

            char nextChar = LookAhead(file);
            if (nextChar == 'n' || nextChar == 't') 
            {
                token[1] = LookAhead(file);
                token[2] = '\0';
                ch = fgetc(file);

                // Adiciona os caracteres ao grafo
                for (int i = 0; i < strlen(token); i++) 
                {
                    add_Edge(tokenGraph, tokenCount, token[i]);  
                }
            } else {
                token[1] = '\0';
                add_Edge(tokenGraph, tokenCount, ch);
            }
            fprintf(f_output, "Escape: %s\n", token);
            tokenCount++;
            tokenGraph->numVertices = tokenCount;
        }

        // Delimitadores
        else if (is_Delimiter(ch)) 
        {
            add_Edge(tokenGraph, tokenCount, ch);

            char token[2];
            token[0] = ch;
            token[1] = '\0';

            fprintf(f_output, "Delimitador: %s\n", token);
            tokenCount++;
            tokenGraph->numVertices = tokenCount;
              
        }
        // Outros caracteres não definidos
        else {
            add_Edge(tokenGraph, tokenCount, ch);
            fprintf(f_output, "Caractere nao definido: %c\n", ch);
            
            tokenCount++;
            tokenGraph->numVertices = tokenCount;
        }
    }   

    
    printf("Grafo:\n");
    print_Graph(tokenGraph);

    // Libera a memória alocada para a lista de adjacência
    for (int i = 0; i < tokenGraph->numVertices; i++) 
    {
        Node* curr = tokenGraph->adjLists[i];
        while (curr != NULL) 
        {
            Node* temp = curr;
            curr = curr->nextChar;
            free(temp);
        }
    }
    free(tokenGraph->adjLists);
    free(tokenGraph);
    fclose(f_output);
}

int main() 
{
    FILE* file = fopen("codigo.c", "r");
    if (file == NULL) 
    {
        printf("Erro na abertura de arquivo.\n");
        return 1;
    }

    lexical_Analyzer(file);
    fclose(file);
    return 0;
}
