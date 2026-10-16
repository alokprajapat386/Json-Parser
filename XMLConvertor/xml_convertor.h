#include "../IR/ir_node.h"
#include<string.h>
#include "../HelperStructure/file_writer.h"

static int tab_count=0;
void print_tabs(FileWriter* fw){
    for(int i=0; i<tab_count; i++){
        printInFile(fw, "    ");
    }
}

char* start_tag(char* key){
    int len=strlen(key);
    char* tag= (char*)malloc((3+len)*sizeof(char));
    tag[0]='<';
    tag[len+1]='>';
    for(int i=1; i<=len; i++){
        tag[i]=key[i-1];
    }
    tag[len+2]='\0';
    return tag;
}

char* end_tag(char* key){
    int len=strlen(key);
    char* tag= (char*)malloc((4+len)*sizeof(char));
    tag[0]='<';
    tag[1]='/';
    tag[len+2]='>';
    for(int i=2; i<=len+1; i++){
        tag[i]=key[i-2];
    }
    tag[len+3]='\0';
    return tag;
}

void convertIntoXMLArray(FileWriter *fw, IRArray *array, char* key);
void convertIntoXMLObject(FileWriter *fw, IRObject *object);
void convertIntoXMLValue(FileWriter *fw, IRValue *value);

void convertIntoXMLArray(FileWriter *fw, IRArray *array, char* key){
    char* s_tag=start_tag(key);
    char* e_tag=end_tag(key);
    for(int i=0; i<array->size; i++){
        print_tabs(fw);
        printInFile(fw, s_tag);
        tab_count++;
        if(array->elements[i]->type==IRTYPE_ARRAY || array->elements[i]->type==IRTYPE_OBJECT){
            printf("\n");
        }
        convertIntoXMLValue(fw, array->elements[i]);

        tab_count--;

        if(array->elements[i]->type==IRTYPE_ARRAY || array->elements[i]->type==IRTYPE_OBJECT){
            print_tabs(fw);
        }
        printInFile(fw, e_tag);
        printInFile(fw, "\n");
    
    }

}

void convertIntoXMLObject(FileWriter *fw, IRObject *object){
    
    for(int i=0; i<object->size; i++){
        IRPair * element=object->elements[i];
        if(element->value.type==IRTYPE_ARRAY){
            convertIntoXMLArray(fw, (IRArray*)element->value.value, element->key);
            continue;
        }
        char* s_tag=start_tag(element->key);
        char* e_tag=end_tag(element->key);
        print_tabs(fw);
        printInFile(fw, s_tag);
        tab_count++;
        

        //TODO: Handling value
        if(element->value.type==IRTYPE_ARRAY || element->value.type==IRTYPE_OBJECT){
            printInFile(fw, "\n");
        }
        convertIntoXMLValue(fw, &element->value);

        tab_count--;
        if(element->value.type==IRTYPE_ARRAY || element->value.type==IRTYPE_OBJECT){
            print_tabs(fw);
        }
        printInFile(fw, e_tag);
        printInFile(fw, "\n");
        
    }

}

void convertIntoXMLValue(FileWriter *fw, IRValue *value){
    
    switch(value->type){
        case IRTYPE_ARRAY:
            convertIntoXMLArray(fw, (IRArray*)value->value, "root");
        break;
        case IRTYPE_OBJECT:
            convertIntoXMLObject(fw, (IRObject*)value->value);
        break;
        default:
            
            printInFile(fw, (char*)value->value);
            
    }
}

void convertIntoXML(FileWriter *fw, IRValue *value){    
    convertIntoXMLValue(fw, value);
}


