# Small C Compiler

## Compilar

```bash
g++ src/*.cpp -o mcc
```

## Uso

```bash
mcc <input_file> <output_file>
```

## Gramática

```
declaración_externa ::= 
      especificador_tipo identificador declaración declaración_externa 
    | ""
    ;

declaración ::= 
      "=" expresión_de_asignación lista_declaradores ";"
    | "(" lista_de_parametros ")" definición_de_función
    | lista_declaradores ";"
    ;

lista_declaradores ::= 
      "," indentificador declarador lista_declaradores
    | ""
    ;

declarador ::= 
      "=" expresión_de_asignación
    | "(" lista_de_parametros ")" 
    | ""
    ;

definición_de_función ::=
      proposición_compuesta
    | lista_declaradores ";"
    ;

especificador_de_tipo ::=
      "int" 
    | "void"
    ;

lista_de_parametros ::=
      especificador_de_tipo identificador {"," especificador_de_tipo identificador}
    | ""
    ;

proposición ::=
      "if" "(" expresión ")" proposicion desenlace_if
    | "while" "(" expresión ")" proposición
    | "do" proposición "while" "(" expresión ")" ";"
    | "for" "(" expresión_opt ";" expresión_opt ";" expresión_opt ")" proposición
    | especificador_tipo indentificador declarador lista_declaradores ";"
    | proposición_compuesta
    | proposición_de_salto ";"
    | expresión_opt ";"
    ;

proposición_compuesta ::=
      "{" lista_de_propocisiones "}"

proposición_de_salto ::=
      "break"
    | "continue"
    | "return" expresión_opt

lista_de_proposiciones ::= 
      proposición lista_de_proposiciones
    | ""
    ;

desenlace_if ::=
      "else" proposición
    | ""
    ;

expresión_opt ::=
      expresión
    | ""
    ;

expresión ::= 
    expresión_de_asignación {"," expresión_de_asignación}
    ;

expresión_de_asignación ::= 
      identificador "=" expresión_de_asignación
    | expresión_lógica_OR
    ;

expresión_lógica_OR ::= 
      expresión_lógica_AND {"||" expresión_lógica_AND}
    ;

expresión_lógica_AND ::=
      expresión_de_igualdad {"&&" expresión_de_igualdad}

expresión_de_igualdad ::=
      expresión_relacional {op_igual expresión_relacional}
    ;

expresión_relacional ::=
      expresión_aditiva {op_relacional expresión_aditiva}
    ;

expresión_aditiva
      expresion_multiplicativa {op_adición expresión_multiplicativa}
    ;

expresión_multiplicativa ::=
      expresión_unaria {op_mult expresión_unaria}
    ;

expresión_unaria ::=
      op_adición expresión_unaria
    | expresión_primaria
    ;

expresión_primaria ::=
      identificador llamada_a_función
    | entero
    | "(" expresión ")"
    ;

llamada_a_función ::=
      "(" lista_de_argumentos ")"
    | ""
    ;

lista_de_argumentos ::=
      expresión_de_asignación {"," expresión_de_asignación}
    | ""

op_igual ::= "==" | "!=" ;

op_relacional ::= "<" | ">" | ">=" | "<=" ;

op_adición ::= "+" | "-" ;

op_mult ::= "*" | "/"  | "%" ;

entero ::= digito {digito};

identificador ::= (letra | "_") {(letra | digito | "_")};

letra      ::= minuscula | mayuscula;
minuscula  ::= "a"..."z";
mayuscula  ::= "A"..."Z";
digito     ::= "0"..."9";
```