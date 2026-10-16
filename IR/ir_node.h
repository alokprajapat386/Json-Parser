

#ifndef IR_NODE_H
#define IR_NODE_H

#include<stdlib.h>
#include<stdio.h>

typedef enum {
    IRTYPE_STRING,
    IRTYPE_NUMBER,
    IRTYPE_BOOLEAN,
    IRTYPE_NULL,
    IRTYPE_ARRAY,
    IRTYPE_OBJECT
}   IRNodeType;



typedef struct {
    IRNodeType type;
    void *value;
} IRValue;

typedef struct {
    char* key;
    IRValue value;
} IRPair;

typedef struct {
    IRPair **elements;
    int cap;
    int size;
    
} IRObject;

typedef struct {
    IRValue **elements;
    int cap;
    int size;
} IRArray;


IRObject *createObject(){
    IRObject* object = (IRObject*)malloc(sizeof(IRObject));
    (object->size)=0;
    object->cap=10;
    object->elements= (IRPair**)malloc(object->cap*sizeof(IRPair*));

    return object;
}

IRArray *createArray(){
    IRArray* array= (IRArray*)malloc(sizeof(IRArray));
    array->cap=10;
    array->size=0;
    array->elements=(IRValue**)malloc(array->cap*sizeof(IRValue*));
    return array;
}


IRObject *addPair(IRObject *object, IRPair *pair){
    // printf("Object-size after add : %d\n", object->size );
    if(object->cap==object->size){
        object->cap*=2;
        object->elements=(IRPair**)realloc(object->elements, object->cap*sizeof(IRPair*));
    }
    object->elements[object->size]=pair;
    object->size++;
    return object;
}

IRArray* addValue(IRArray *array, IRValue *value){
    if(array->cap==array->size){
        array->cap*=2;
        array->elements=(IRValue**)realloc(array->elements, array->cap*sizeof(IRValue*));
    }
    array->elements[array->size]=value;
    array->size++;
    return array;

}

IRValue* createValue(IRNodeType nodetype, void *value){
    IRValue* ir_value=(IRValue*)malloc(sizeof(IRValue));
    ir_value->type=nodetype;
    ir_value->value=value;
}

IRPair* createPair(char *key, IRValue *value){
    IRPair *pair= (IRPair*)malloc(sizeof(IRPair));
    pair->key=key;
    pair->value=*value;

    return pair;
}   

// void printIRPair(IRPair* pair);
// void printIRObject(IRObject * object);
// void printIRValue(IRValue *value);
// void printIRArray(IRArray *array);

// void printIRPair(IRPair* pair){
//     printf("%s : ", pair->key);
//     printIRValue(&pair->value);
// }

// void printIRObject(IRObject * object){
//     bool is_first=true;
//     // print_tabs();
//     printf("{\n");
//     tab_count++;

//     for(int i=0; i<object->size; i++){
//         if(!is_first){
//             printf(",\n");
//         }
//         is_first=false;
//         print_tabs();
//         printIRPair(object->elements[i]);
//     }
//     printf("\n");
//     tab_count--;
//     print_tabs();

//     printf("}");
// }


// void printIRValue(IRValue *value){
//     switch (value->type){
//         case IRTYPE_ARRAY:
//             printIRArray((IRArray*)value->value);
//             break;
//         case IRTYPE_OBJECT:
//             printIRObject((IRObject*)value->value);
//             break;
//         default:
//             printf("%s", (char*)value->value);
            
//     }
// }

// void printIRArray(IRArray *array){
//     bool is_first=true;
//     // print_tabs();
//     printf("[\n");

//     tab_count++;
//     for(int i=0; i<array->size; i++){
//         if(!is_first){
//             printf(",\n");
//         }
//         is_first=false;
//         print_tabs();
//         printIRValue(array->elements[i]);
//     }
//     printf("\n");
//     tab_count--;
//     print_tabs();
//     printf("]");
// }




#endif