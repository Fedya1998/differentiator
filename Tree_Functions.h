tree_header * Tree_Ctor(void) {
    tree_header *header1 = new tree_header();
    header1->first = NULL;
    return header1;
}

elem * Add_First(tree_header * header){
    assert(header);
    header->first = new elem();
    header->first->header = header;
    return header->first;
}

void Tree_Dtor(tree_header *header) {
    Delete_Tree(header);
    delete header;
}

bool Elem_OK(const elem * element){
    if (!element) return false;
    /*if (element->data_type == TYPE_OP && (!element->left || !element->right))
        return false;*/
    return (bool) element->header;
}

bool Elem_And_Next_OK(const elem *element) {
    bool OK = Elem_OK(element);
    if (element->right) OK = OK && Elem_And_Next_OK(element->right);
    if (element->left) OK = OK && Elem_And_Next_OK(element->left);
    return OK;
}

bool Tree_OK(const tree_header *header) {
    elem *element = header->first;
    if (element == NULL) {
        p(12, ("Empty list\n"));
        return true;
    }//If our list is empty
    return Elem_And_Next_OK(element);
}



void Elem_Dump(const elem *element) {
    printf("\nelement %i\n", element);
    if (!element)
        return;
    printf("type_value %i\nvalue %lg\n", element->data_type, element->value);
    if (DBG) {
        printf("pointer %i\n", element);
        printf("header %i\n", element->header);
        printf("right %i\n", element->right);
        printf("left %i\n", element->left);
    }
}

void Mini_File_Dump(const elem * element){
    fprintf(super_tree_dump, "%celement %i\nprevious %i\nvalue_type %i\nvalue %lg\nheader %i\nleft %i\nright %i%c", 34,
            element, element->previous, element->data_type,
            element->value, element->header, element->left, element->right, 34);
}

void Elem_And_Next_Dump(const elem * elm){
    const elem * element = elm;
    if (!elm) return;

    if (element->left) {

        Mini_File_Dump(element);

        fprintf(super_tree_dump, "->");

        Mini_File_Dump(element->left);
    }

    if (element->left) Elem_And_Next_Dump(element->left);

    if(element->right) {

        Mini_File_Dump(element);
        fprintf(super_tree_dump, "->");
        Mini_File_Dump(element->right);

    }
    if (element->right) Elem_And_Next_Dump(element->right);
}

void Tree_Dump(const tree_header *header) {
    fprintf(super_tree_dump, "digraph G{\n");
    if (!header) return;
    if (!header->first) return;
    Elem_And_Next_Dump(header->first);
    fprintf(super_tree_dump, "}");
}





elem * Add_Left(elem * This) {
    SUPER_NOT_OK_DUMP(Elem);
    //Gde;
    This->left = new elem();
    //Gde;
    This->left->header = This->header;
    This->left->previous = This;
    SUPER_NOT_OK_DUMP(Elem);
    return This->left;
}

elem * Add_Right(elem * This) {
    SUPER_NOT_OK_DUMP(Elem);
    //Gde;
    This->right = new elem();
    //Gde;
    This->right->header = This->header;
    This->right->previous = This;
    SUPER_NOT_OK_DUMP(Elem);
    return This->right;
}

int Delete_Tree(tree_header * header){
    Mini_Delete(header->first);
    delete header;
    return errno;
}

void Mini_Delete(elem * element){
    if (!element)
        return;
    if (element->right) Mini_Delete(element->right);
    if (element->left) Mini_Delete(element->left);
    delete element;
}




int Optimize_First(elem * element){
    if (!element) return 0;
    if (!element->value && !element->data_type && element == element->header->first && element->left) {
        elem * elm = element->left;
        element->header->first = elm;
        delete element;
        element = elm->header->first;
        element->previous = NULL;
    }
    if (!element->value && !element->data_type && element == element->header->first) Optimize_First(element);

    return errno;
}

int Tree_Optimize(elem * element){
    if (!element) return ERROR_BAD_TREE;
    if (element->left && !element->left->value && !element->left->data_type && element->left->left) {
        elem * elm = element->left->left;
        delete element->left;
        element->left = elm;
        element->left->previous = element;
    }

    if (element->right && !element->right->value && !element->right->data_type && element->right->left) {
        elem * elm = element->right->left;
        delete element->right;
        element->right = elm;
        element->right->previous = element;
    }

    if (element->left) Tree_Optimize(element->left);
    if (element->right) Tree_Optimize(element->right);
    return errno;
}

int Optimize_Before_Diff(elem * element){
    tree_header * header = element->header;
    Optimize_First(header->first);
    Tree_Optimize(header->first);
    Tree_Optimize(header->first);
    Delete_Useless_Parentheses(header->first);
    return _error;
}

elem * Copy_Elem(elem * element){
    if (!element) return NULL;
    elem * new_elem = new elem();
    new_elem->value = element->value;
    new_elem->data_type = element->data_type;
    new_elem->header = element->header;
    new_elem->left = Copy_Elem(element->left);
    if (new_elem->left) new_elem->left->previous = new_elem;
    new_elem->right = Copy_Elem(element->right);
    if (new_elem->right) new_elem->right->previous = new_elem;
    return new_elem;
}

elem * Optimize_Mul(elem * element){
    if (!Elem_OK) return NULL;
    if (element->data_type == TYPE_OP && element->value == CMD_MUL){

        if (element->right && Wow_Its_Null(element->right)) {
            Mini_Delete(element->left);
            element->left = NULL;
            element->right = NULL;
            element->data_type = TYPE_NUMBER;
            element->value = 0;
        } else if (element->left && Wow_Its_Null(element->left)) {
            Mini_Delete(element->right);
            element->right = NULL;
            element->left = NULL;
            element->data_type = TYPE_NUMBER;
            element->value = 0;
        }

        if (element->left) {
            if (element->left->data_type == TYPE_NUMBER && element->left->value == 1)
                Pridvin_Poblizhe(element->left, 1);
        }

        if (element->right) {
            if (element->right->data_type == TYPE_NUMBER && element->right->value == 1)
                Pridvin_Poblizhe(element->right, 2);
        }


    }


    if (element->left) Optimize_Mul(element->left);
    if (element->right) Optimize_Mul(element->right);
    return element;
}

elem * Optimize_Add(elem * element){
    if (!Elem_OK(element)) {
        return NULL;
    }
    if (element->data_type == TYPE_OP && (element->value == CMD_SUB || element->value == CMD_ADD)){
        if (element->left->data_type == TYPE_NUMBER && element->left->value == 0){
            Pridvin_Poblizhe(element->left, 1);
        }
        if (element->right->data_type == TYPE_NUMBER && element->right->value == 0)
            Pridvin_Poblizhe(element->right, 2);
    }
    if (element->left) Optimize_Add(element->left);
    if (element->right) Optimize_Add(element->right);
    return element;
}

bool Wow_Its_Null(elem * element){
    if (element->data_type == TYPE_NUMBER && element->value == 0){
        Mini_Delete(element);
        return true;
    }
    return false;
}

elem * Pridvin_Poblizhe(elem * element, int otkooda){//1 - left, 2 - right
    if (!Elem_OK(element)) return NULL;
    if (otkooda == 1) {//In the left branch there is 1
        elem * to_the_right = element->previous->right->right;
        elem * to_the_left = element->previous->right->left;
        element->previous->value = element->previous->right->value;
        element->previous->data_type = element->previous->right->data_type;

        if (to_the_right) to_the_right->previous = element->previous;
        if (to_the_left) to_the_left->previous = element->previous;
        if (element->previous->right) delete element->previous->right;
        
        element->previous->right = to_the_right;
        element->previous->left = to_the_left;
        
        delete element;
    }

    if (otkooda == 2) {//Now we won't give a damn about right elem, we'll just take left one higher
        elem * to_the_left = element->previous->left->left;
        elem * to_the_right = element->previous->left->right;
        element->previous->value = element->previous->left->value;
        element->previous->data_type = element->previous->left->data_type;

        if (to_the_right) to_the_right->previous = element->previous;
        if (to_the_left) to_the_left->previous = element->previous;
        if (element->previous->left) delete element->previous->left;


        element->previous->left = to_the_left;
        element->previous->right = to_the_right;

        delete element;
    }


    return element;
}

int Pokazhee_Pliz(elem * element){
    if (!Elem_OK(element)) return 1;
    if (element->data_type == TYPE_OP && element->value != CMD_LBR && element->value != CMD_RBR) printf("(");

    if (element->left && element->data_type != TYPE_FUNCTION) {
        Pokazhee_Pliz(element->left);
    }

    switch (element->data_type){
        case TYPE_NUMBER: {
            printf("%lg", element->value);
            break;
        }
        case TYPE_X: {
            printf("x");
            break;
        }
        case TYPE_OP:{
            Che_Tam_Op(element);
            break;
        }
        case TYPE_FUNCTION:{
            switch ((int) element->value) {
                #define FUNC(name, num)\
                case num: {\
                printf(#name);\
                break;\
                }

                #include "super_functions.h"
                default: {
                    printf("_Undefined function_");
                    _error = ERROR_UNDEFINED_FUNCTION;
                }
                #undef FUNC
            }
            break;
        }
        default:{
            printf("Undefined type");
            _error = ERROR_UNDEFINED_TYPE;
        }
    }

    if (element->left && element->data_type == TYPE_FUNCTION) {
        printf("(");
        Pokazhee_Pliz(element->left);
    }
    if (element->right)Pokazhee_Pliz(element->right);
    if (element->data_type == TYPE_FUNCTION) printf(")");
    if (element->data_type == TYPE_OP && element->value != CMD_LBR && element->value != CMD_RBR) printf(")");

    return 0;
}

int Che_Tam_Op(elem * element){
    if (!element)
        Elem_Dump(element);

    switch ((int) element->value) {
        case CMD_ADD: {
            printf(" + ");
            break;
        }
        case CMD_SUB: {
            printf(" - ");
            break;
        }
        case CMD_MUL: {
            printf(" * ");
            break;
        }
        case CMD_DIV: {
            printf(" / ");
            break;
        }
        case CMD_LBR: {
            if (element->left->left == NULL || element->left->data_type == TYPE_FUNCTION)
                break;
            if (element->previous && element->previous->data_type == TYPE_FUNCTION)
                break;
            printf("(");
            break;
        }
        case CMD_RBR:{
            if (element->previous && (element->previous->left->left == NULL || element->previous->left->data_type == TYPE_FUNCTION))
                break;
            if (element->previous && element->previous->previous && element->previous->previous->data_type == TYPE_FUNCTION)
                break;
            printf(")");
            break;
        }
        case CMD_POW: {
            printf("^");
            break;
        }
        default: {
            printf("Undefined value\n");
            Elem_Dump(element);
            _error = ERROR_UNDEFINED_VALUE;
            return 1;
        }
    }
    return 0;
}

void Delete_Useless_Parentheses(elem * element){
    elem * This = element;
    SUPER_NOT_OK_DUMP(Elem);
    if (element->data_type == TYPE_OP && element->value == CMD_LBR){
        if (element->previous->data_type != TYPE_FUNCTION){
            //printf("deleting\n");
            Pridvin_Poblizhe(element->right, 2);
        }
    }
    if (element->left) Delete_Useless_Parentheses(element->left);
    if (element->right) Delete_Useless_Parentheses(element->right);
}
