elem * Diff_ln(elem * element){
    Gde;
    elem * function_f = Copy_Elem(element->left->left);
    Mini_Delete(element->left);
    element->data_type = TYPE_OP;
    element->value = CMD_DIV;
    element->right = Copy_Elem(function_f);
    element->left = Diff_op(function_f);
    return element;
}

elem * Diff_sin(elem * element){
    Gde;
    elem * function_br = element->left;
    elem * function_f = element->left->left;
    Add_Right(element);

    element->data_type = TYPE_OP;
    element->value = CMD_MUL;
    Add_Left(element);

    element->left->value = FUNC_cos;
    element->left->data_type = TYPE_FUNCTION;
    element->left->left = function_br;
    element->right = Diff_op(Copy_Elem(function_f));
    element->right->previous = element;
    return element;
}

elem * Diff_cos(elem * element){
    Gde;
    elem * function_br = Copy_Elem(element->left);
    elem * function_f = Copy_Elem(element->left->left);
    Mini_Delete(element->left);
    Add_Right(element);
    Add_Left(element);

    element->data_type = TYPE_OP;
    element->value = CMD_MUL;

    element->left->value = CMD_MUL;
    element->left->data_type = TYPE_OP;
    Add_Left(element->left);
    element->left->left->data_type = TYPE_NUMBER;
    element->left->left->value = -1;

    element->left->right = Diff_op(function_f);

    element->right->data_type = TYPE_FUNCTION;
    element->right->value = FUNC_sin;
    element->right->left = function_br;
    return element;
}

elem * Diff_op(elem * element){
    if (!Elem_OK(element)) {
        _error = ERROR_BAD_TREE;
        Elem_Dump(element);
        printf(("Veehodeem\n"));
        return NULL;
    }

    switch (element->data_type) {
        case 0: {
            _error = ERROR_WRONG_TYPE;
            return NULL;
        }
        case TYPE_NUMBER: {
            element->data_type = TYPE_NUMBER;
            element->value = 0;
            break;
        }

        case TYPE_X: {
            element->data_type = TYPE_NUMBER;
            element->value = 1;
            break;
        }
        case TYPE_OP: {
            switch ((int) element->value) {
                case CMD_ADD: {
                    Diff_op(element->left);
                    Diff_op(element->right);
                    break;
                }
                case CMD_SUB: {
                    Diff_op(element->left);
                    Diff_op(element->right);
                    break;
                }
                case CMD_MUL: {
                    element->value = CMD_ADD;
                    elem *function_f = Copy_Elem(element->left);
                    elem *function_g = Copy_Elem(element->right);
                    Mini_Delete(element->left);
                    Mini_Delete(element->right);

                    Add_Left(element);
                    element->left->value = CMD_MUL;
                    element->left->data_type = TYPE_OP;
                    element->left->left = Copy_Elem(function_f);
                    element->left->left->previous = element->left;
                    element->left->right = Copy_Elem(function_g);
                    element->left->right->previous = element->left;
                    Diff_op(element->left->left);

                    Add_Right(element);
                    element->right->value = CMD_MUL;
                    element->right->data_type = TYPE_OP;
                    element->right->left = function_f;
                    element->right->left->previous = element->right;
                    element->right->right = function_g;
                    element->right->right->previous = element->right;
                    Diff_op(element->right->right);
                    break;
                }
                case CMD_DIV: {
                    element->value = CMD_SUB;
                    element->data_type = TYPE_OP;
                    elem *function_f = Copy_Elem(element->left);
                    elem *function_g = Copy_Elem(element->right);
                    Mini_Delete(element->right);
                    Mini_Delete(element->left);
                    Add_Left(element);
                    element->left->value = CMD_DIV;
                    element->left->data_type = TYPE_OP;
                    element->left->left = Diff_op(Copy_Elem(function_f));
                    element->left->right = Copy_Elem(function_g);

                    Add_Right(element);
                    element->right->value = CMD_MUL;
                    element->right->data_type = TYPE_OP;
                    element->right->left = function_f;
                    Add_Right(element->right);
                    element->right->right->value = CMD_DIV;
                    element->right->right->data_type = TYPE_OP;
                    element->right->right->left = Diff_op(Copy_Elem(function_g));
                    Add_Right(element->right->right);
                    element->right->right->right->value = CMD_MUL;
                    element->right->right->right->data_type = TYPE_OP;
                    element->right->right->right->right = function_g;
                    element->right->right->right->left = function_g;
                    break;
                }
                case CMD_POW: {
                    elem *function_f = Copy_Elem(element->left);
                    elem *function_g = Copy_Elem(element->right);
                    Mini_Delete(element->right);
                    Mini_Delete(element->left);
                    Add_Left(element);
                    Add_Right(element);

                    element->value = CMD_MUL;

                    element->left->value = CMD_POW;
                    element->left->data_type = TYPE_OP;
                    element->left->left = Copy_Elem(function_f);
                    element->left->left->previous = element->left;
                    element->left->right = Copy_Elem(function_g);


                    element->right->value = CMD_MUL;
                    element->right->data_type = TYPE_OP;
                    Add_Left(element->right);
                    element->right->left->value = FUNC_ln;
                    element->right->left->data_type = TYPE_FUNCTION;
                    Add_Left(element->right->left);
                    element->right->left->left->value = CMD_LBR;
                    element->right->left->left->data_type = TYPE_OP;
                    element->right->left->left->left = function_f;
                    element->right->left->left->left->previous = element->right->left->left;

                    Add_Right(element->right->left->left);
                    element->right->left->left->right->value = CMD_RBR;
                    element->right->left->left->right->data_type = TYPE_OP;

                    element->right->right = function_g;
                    element->right->right->previous = element->right;
                    printf("\n\n\n\n");
                    Gde;

                    Diff_op(element->right);

                    break;
                }
                default:{
                    _error = ERROR_UNDEFINED_VALUE;
                    printf("error %i\n", _error);
                    Err;
                    Elem_Dump(element);
                    return NULL;
                }
            }
            break;
        }
        case TYPE_FUNCTION: {
            Diff_Func(element);
            break;
        }
        default: {
            _error = ERROR_WRONG_TYPE;
            return NULL;
        }
    }




    return element;
}

elem * Diff_Func(elem * element) {
    if (!Elem_OK(element)) {
        _error = ERROR_BAD_TREE;
        return NULL;
    }
    switch ((int) element->value) {
#define FUNC(name, num)\
        case FUNC_##name: {\
        Diff_##name(element);\
        break;\
    }

#include "super_functions.h"
#undef FUNC
    }
}
