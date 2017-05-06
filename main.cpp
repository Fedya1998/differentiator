#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <clocale>
#include <malloc.h>
#include <my_functions.h>

#define NDBG 13

#include <my_debug.h>

#include "super_enum.h"

//G0 ::= E
//E ::= T {['+''-'] T}*
//T ::= D {['*''/'D}*
//D ::= P{^P}*
//P ::= '('E')'|N|X
//N ::= ['0' - '9']+
//X ::= 'x'

//Id ::= [A-Z|a-z][A-Z|a-z|_]*

FILE * super_tree_dump = fopen("dump.gv","w");

typedef struct elem;
typedef struct tree_header;

struct elem{
    elem * left = NULL;
    elem * right = NULL;
    double value = 0;
    int data_type = 0;// 1 - number, 2 - variable, 3 - operation, 4 - function
    elem * previous = NULL;
    tree_header * header = NULL;
};

const int TYPE_NUMBER = 1;
const int TYPE_X = 2;
const int TYPE_OP = 3;
const int TYPE_FUNCTION = 4;




struct tree_header{
    elem * first = NULL;
};

tree_header * Tree_Ctor(void);
void Tree_Dtor(tree_header * header);
bool Elem_And_Next_OK(const elem *element);
bool Tree_OK(const tree_header *header);
bool Elem_OK(const elem * element);
void Elem_Dump(const elem *element);
void Elem_And_Next_Dump(const elem * elm);
void Tree_Dump(const tree_header *header);
elem * Add_First(tree_header * header);
elem * Add_Left(elem * element);
elem * Add_Right(elem * element);
void Mini_Delete(elem * element);
int Delete_Tree(tree_header * header);
void Mini_File_Dump(const elem * element);

int Optimize_First(elem * element);
int Tree_Optimize(elem * element);
int Optimize_Before_Diff(elem * element);
bool Wow_Its_Null(elem * element);
elem * Pridvin_Poblizhe(elem * element, int otkooda);

elem * Optimize_Add(elem * element);
elem * Optimize_Mul(elem * element);
void Delete_Useless_Parentheses(elem * element);

elem * Copy_Elem(const elem * element);

elem * Diff_Func(elem * element);
elem * Diff_op(elem * element);


int _error = 0;

tree_header * GetG0(char * str);
elem * GetN(elem * element);
elem * GetX(elem * element);
elem * GetFunc(elem * element);
elem * GetE(elem * element);
elem * GetT(elem * element);
elem * GetP(elem * element);
elem * GetD(elem * element);

void Parse_Error();

int Pokazhee_Pliz(elem * element);
int Che_Tam_Op(elem * element);


int main(){
    char str[] = ("(cos(5*x^7))^2+ln(x)");

    tree_header * header = GetG0(str);

    Optimize_Before_Diff(header->first);


    Diff_op(header->first);
    printf("\n\n------------------------\n\n");

    Optimize_Mul(header->first);
    Optimize_Add(header->first);

    Tree_Dump(header);

    Pokazhee_Pliz(header->first);

    Delete_Tree(header);

    printf("\nerror %d\n", _error);


    return 0;
}

#include "errors.h"
#include "gets.h"
#include "Tree_Functions.h"
#include "diff_func.h"


