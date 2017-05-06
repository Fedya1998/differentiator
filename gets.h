#include <malloc.h>

char * s = NULL;

void Parse_Error(){
    printf("Error %i on %s\n", _error, s);
}

tree_header * GetG0(char * str){
    s = str;
    tree_header * header = Tree_Ctor();
    Add_First(header);
    GetE(header->first);
    if (*s != '\0' || _error) Parse_Error();
    return header;
}

elem * GetSt(elem * element){//Try to ger number, x or function
    Gde;
    p(11, ("Got %s\n", s));
    if (!GetN(element)) GetFunc(element);
    return element;
}

elem * GetN(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    int res = 0;
    _error = ERROR_EMPTY_LINE;
    while('0'<= *s && *s <= '9'){
        _error = 0;
        res = res * 10 + *s - '0';
        s++;
    }
    if (!_error){
        element->value = res;
        element->data_type = TYPE_NUMBER;
        p(10, ("I'm returning res %i element %i\n", res, element));
        return element;
    } else return NULL;

}

elem * GetFunc(elem * element) {
    Gde;
    p(11, ("Got %s\n", s));
    if (GetX(element)) return element;
    char *func_end = strchr(s, '(');
    char *super_func = (char *) calloc(func_end - s + 1, sizeof(char));
    assert(super_func);

    for (char * i = s; i < func_end; ++i) {
        super_func[i - s] = *i;
    }
    super_func[func_end - s] = '\0';
    d(12, printf("my function %s\n", super_func));

#define FUNC(name, num)\
    if (strcmp(#name, super_func) == 0) {\
        p(13, ("EEE podoshlo %s\n", #name));\
        element->value = num;\
        element->data_type = TYPE_FUNCTION;\
        s = func_end;\
        d(12, Elem_Dump(element));\
        Add_Left(element);\
        Gde;\
        GetP(element->left);\
    } else
#include "super_functions.h"
    {
        _error = ERROR_UNDEFINED_FUNCTION;
        Parse_Error();
        return NULL;
    }
#undef FUNC

    free(super_func);

    return element;
}

elem * GetX(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    if (*s == 'x') {
        if (_error == 1) _error = 0;
        s++;
        element->data_type = TYPE_X;
        return element;
        p(10, ("Returning x\n"));
    }
    p(10, ("Not x\n"));
    return NULL;
}

elem * GetE(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    Add_Left(element);
    GetT(element->left);
    while (*s == '+' || *s == '-'){
        s++;
        char operation = s[-1];

        element->data_type = TYPE_OP;
        if (operation == '+') {
            element->value = CMD_ADD;
        }
        if (operation == '-') {
            element->value = CMD_SUB;
        }
        Add_Right(element);
        element = element->right;
        GetT(element);
    }
    p(10, ("I'm returning element %i\n", element));
    return element;
}

elem * GetT(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    Add_Left(element);
    GetD(element->left);
    while (*s == '*' || *s == '/'){
        s++;
        char operation = s[-1];
        element->data_type = TYPE_OP;
        if (operation == '*') {
            element->value = CMD_MUL;
        }
        if (operation == '/') {
            element->value = CMD_DIV;
        }
        Add_Right(element);
        element = element->right;
        GetD(element);
    }
    p(10, ("I'm returning element %i\n", element));
    return element;
}

elem * GetD(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    Add_Left(element);
    GetP(element->left);
    while (*s == '^'){
        s++;
        element->data_type = TYPE_OP;
        element->value = CMD_POW;
        Add_Right(element);
        element = element->right;
        GetP(element);
    }
    p(10, ("I'm returning element %i\n", element));
    return element;
}



elem * GetP(elem * element){
    Gde;
    p(11, ("Got %s\n", s));
    if (*s == '('){
        Add_Left(element);
        s++;
        element->value = CMD_LBR;
        element->data_type = TYPE_OP;
        GetE(element->left);
        if (*s != ')') _error = ERROR_WRONG_PARENTHESES;

        Add_Right(element);
        element->right->value = CMD_RBR;
        element->right->data_type = TYPE_OP;

        s++;
    }
    else return GetSt(element);

    p(10, ("I'm returning element %i\n", element));
    return element;
}

