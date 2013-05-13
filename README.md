Syntax-Analyzer

===============
DESCRIPTION:
===============
A syntax analyzer for our group's final project

===============
GRAMMAR:
===============

PROGRAM	: USES OBJ
		;

USES	: USE USES
		;

OBJ		: ‘obj’ ID ‘->’ FUNCLIST ‘;’
		| ‘obj’ ID ‘->’ DEC ‘;’
		;

STM		: IF | FOR | FOREACH | WHILE | EXEC | RETURN | ASSIGN
		;

FUNCLIST	: FUNCTION
			| DEC
			| ARRAY
			| ASSIGN
			|
			;

STMLIST	: STM STMLIST
		| DEC STMLIST
		| ARRAY STMLIST
		| ASSIGN STMLIST
		| METHOD STMLIST
		|
		;

IF1		: ‘if’ BOOLEXP ‘->’ STMLIST ‘;’
		| ‘if’ BOOLEXP ‘->’ STMLIST ‘;’ ELSEIF
		| ‘if’ BOOLEXP ‘->’ STMLIST ‘;’ ELSE1
		;

ELSEIF	: ‘elseif’ BOOLEXP ‘->’ STMLIST ‘;’
		| ‘elseif’ BOOLEXP ‘->’ STMLIST ‘;’ ELSEIF
		| ‘elseif’ BOOLEXP ‘->’ STMLIST ‘;’ ELSE1
		;

ELSE1	: ‘else’ ‘->’ STMLIST ‘;’
		;

FOR1	: ‘for’ VAR ‘=’ EXP ‘(‘ INC ‘)’ EXP ‘->’ STMLIST ‘;’
		;

INC		: ‘+’ EXP
		| ‘-’ EXP
		;

FOREACH	: ‘foreach’ VAR ‘=’ EXP ‘in’ ID -> STMLIST ‘;’
		;

WHILE1	: ‘while’ BOOLEXP ‘->’ STMLIST ‘;’
		;

EXEC	: ‘exec’ ‘->’ STMLIST ‘;’ ‘while’ BOOLEXP ‘;’
		;

RETURN1	: ‘return’ ID ‘;’
		| ‘return’ EXP ‘;’
		| ‘return’ ‘;’
		;

USE		: CHARACTER ‘;’
		| CHARACTER ‘.’ USE
		;

ASSIGN	: ID ‘=’ STRINGEXP ;
		| ID ‘=’ EXP ;
		;

METHOD		: ID ‘(‘ ‘)’ ‘;’
		| ID ‘(‘ PARAMETERS ‘)’ ‘;’
		| ID ‘.’ METHOD
		;

EXP		: ARITHEXP
		| BOOLEXP
		| STRINGEXP
		;

T		: ‘+’ EXP
		| ‘-’ EXP
		| ‘*’ EXP
		| ‘/’ EXP
		;

U		: ‘==’ EXP
		| ‘!=’ EXP
		| ‘>’ EXP
		| ‘<’ EXP
		| ‘>=’ EXP
		| ‘<=’ EXP
		;

V		: T V
		|
		;

X		: U X
		|
		;

Z		: T V U X Z
		|
		;

ARITHEXP	: V
			| BOOLEXP T V
			;

BOOLEXP	: BOOLEAN X Z
		| CHARACTER X Z
		| V U X Z
		;

STRINGEXP 	: STRING1 STRINGEXP
			| STRING1
			;

VAR		: ‘any’ ID
		;

DEC		: ‘any’ ID ‘;’
		| ‘any’ ID ‘=’ EXP ‘;’
		| ‘def’ ID ‘=’ EXP ‘;’
		;

ID		: CHARACTER
		| CHARACTER INTEGER CHARACTER
		| CHARACTER ‘_’ CHARACTER
		;

ARRAY	: ‘arr’ ID ‘->’ DIMENS ‘;’
		| ‘arr’ ID ‘[‘ INTEGER ‘]’ ‘;’
		| ‘arr’ ID ‘[‘ INTEGER ‘]’ ‘[‘ INTEGER ‘]’ ‘;’
		;

DIMENS	: MULTIARR
		| MULTIARR ‘|’ DIMENS
		|
		;

MULTIARR	: EXP
			| EXP ‘,’ MULTIARR
			;

STRINGWORD	: CHARACTER STRINGWORD
			| SYMBOL STRINGWORD
			|
			;

STRING1	: ‘“‘ STRINGWORD ‘“‘
		|
		;

BOOLEAN	: ‘true’
		| ‘false’
		;

FUNCTION	: ‘func’ ID ‘(‘ PARAMETERS ‘)’ ‘->’ STMLIST ‘;’

PARAMETERS	: ‘any’ VAR ‘,’ PARAMETERS
			| ‘any’ VAR
			|
			;

NUM		: ‘0’ | ‘1’ | ‘2’ | ‘3’ | ‘4’ | ‘5’ | ‘6’ | ‘7’ | ‘8’ | ‘9’
		;

INTEGER	: NUM INTEGER
		|
		;

CHARACTER	: ‘A’ | ... | ‘z’
			;

SYMBOL	: ‘ ‘ | ‘!’ | ‘.’ | ‘$’ | …
		;

FLOAT	: INTEGER ‘.’ INTEGER
		;
