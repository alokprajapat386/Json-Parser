
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include<stdlib.h>
#include "../IR/ir_node.h"
#include "validator.h"
#include<string.h>

void skip_whitespace(char *s, int *curr_index, int len);
IRValue* parseString(char* json, int *curr_index, int len);
IRValue* parseValue(char*json, int *curr_index, int len);
IRPair* parsePair(char *json, int * curr_index, int len);
IRValue* parseObject(char*json, int *curr_index, int len);
IRValue* parseArray(char*json, int *curr_index, int len);
IRValue* parseJson(char *json, int *curr_index, int len);


void skip_whitespace(char *s, int *curr_index, int len){
    while(*curr_index<len && (s[*curr_index]==' ' || s[*curr_index]=='\n' || s[*curr_index]=='\t')){
        (*curr_index)++;
    }
}

IRValue* parseString(char* json, int *curr_index, int len){
    int end_index=*curr_index;
    bool escaping=false;
    int string_len=0;

    while(end_index<len){
        
        if(!escaping && json[end_index]=='\\'){
            escaping=true;
            end_index++;
            continue;
        }

        if((json[end_index]=='"') && !escaping){
            break;
        }

        string_len++;

        if(escaping){
            escaping=false;
        }

        end_index++;
    }


    //TODO: if(end_index==len){   }
 
    char* string=(char*)malloc((string_len+1)*(sizeof(char)));
    int str_ind=0;
    end_index=*curr_index;  
    escaping =false;
     while(end_index<len){
        if(!escaping && json[end_index]=='\\'){
            escaping=true;
            end_index++;
            continue;
        }

        if((json[end_index]=='"') && !escaping){
            break;
        }

        string[str_ind]=json[end_index];
        str_ind++;
        if(escaping){
            escaping=false;
        }

        end_index++;
    }

    string[str_ind]='\0';

    *curr_index=end_index+1;

    IRValue *value= (IRValue*) malloc(sizeof(IRValue));
    value->type=IRTYPE_STRING;
    value->value=(void*)string;

    // printf("curr_index 1 : %d, char at curr_index : %c ", *curr_index, json[*curr_index]);

    return value;
}



IRValue* parseValue(char*json, int *curr_index, int len){
    skip_whitespace(json, curr_index, len);
     int end_index=*curr_index;

     if(end_index<len && json[end_index]=='"'){
       ( *curr_index)++;
        // printf( "curr_index 1 : %d, char at curr_index: %c\n", *curr_index, json[*curr_index]);
        return parseString(json, curr_index, len);
     }

     while(end_index<len && (json[end_index]!=',' && json[end_index]!='}')){
        // printf("end_index : %d, char at end_index : %c\n", end_index, json[end_index]);
        end_index++;
     }

     int vallen=end_index-*curr_index;

    // printf("vallen : %d\n", vallen);

      char* value=(char*)malloc((vallen+1)*(sizeof(char)));

    // printf("%d", (vallen+1)*sizeof(char));
     if(value==NULL){
        printf("MALLOC Failed : %d", vallen+1);
     }
    // printf("here\n");
     end_index=*curr_index;
     for(int i=0; i<vallen; i++){
        // printf("i : %d, end_index : %d\n", i, end_index);
        value[i]=json[end_index];
        end_index++;
     }

     value[vallen]='\0';
     *curr_index=end_index+1;

    // printf("Json Value: %s", value);
     IRValue* ir_value=(IRValue*)malloc(sizeof(IRValue));
     ir_value->value=value;
     if(validate_number(value,vallen)){
        ir_value->type=IRTYPE_NUMBER;
        // printf("curr_index 2 : %d, char at curr_index : %c\n", *curr_index, json[*curr_index]);
        return ir_value;
     }

    // printf("not a number\n");
     if(is_boolean(value, vallen)){
        ir_value->type=IRTYPE_BOOLEAN;
        return ir_value;
     }

     if(is_null(value, vallen)){
        ir_value->type=IRTYPE_NULL;
        return ir_value;
     }
    
     //TODO: handling INVALID
     return NULL;
}

IRPair* parsePair(char *json, int * curr_index, int len){
    skip_whitespace(json, curr_index, len);
    if(json[*curr_index]=='"'){
        (*curr_index)++;
        int end_index=*curr_index;

        bool escaping=false;
        int key_len=0;

        while(end_index<len){
            if(!escaping && json[end_index]=='\\'){
                escaping=true;
                end_index++;
                continue;
            }

            if((json[end_index]=='"') && !escaping){
                break;
            }

            key_len++;

            if(escaping){
                escaping=false;
            }

            end_index++;
        }

        //TODO: if(end_index==len){   }
        // printf(" end_index : %d, key_len : %d\n",end_index,key_len);
        char* key=(char*)malloc((key_len+1)*(sizeof(char)));
        int str_ind=0;
        end_index=*curr_index;  

        while(end_index<len){
            if(!escaping && json[end_index]=='\\'){
                escaping=true;
                end_index++;
                continue;
            }

            if((json[end_index]=='"') && !escaping){
                break;
            }

            key[str_ind]=json[end_index];
            str_ind++;
            if(escaping){
                escaping=false;
            }

            end_index++;
        }

        key[str_ind]='\0';
        *curr_index=end_index+1;
        skip_whitespace(json, curr_index, len);

       
        if(json[*curr_index]==':'){
            (*curr_index)++;
        }

        
        IRValue *value;
        skip_whitespace(json, curr_index, len);
        // printf("curr_index 3 : %d, char at curr_index : %c\n", *curr_index, json[*curr_index]);
        if(json[*curr_index]=='{'){
            (*curr_index)++;
            value=parseObject(json, curr_index, len);
        }else if(json[*curr_index]=='['){
            (*curr_index)++;
            value=parseArray(json, curr_index, len);
        }else{
            value=parseValue(json, curr_index, len);
        }

        //TODO: To be declared
        IRPair* pair=createPair(key, value);

        return pair;
    }
    return NULL;

}



IRValue* parseObject(char*json, int *curr_index, int len){
    skip_whitespace(json, curr_index, len);
    IRObject *object=createObject();

    while((*curr_index)<len && json[*curr_index]!='}'){
        IRPair * pair= parsePair(json, curr_index, len);
        skip_whitespace(json, curr_index, len);
        object =addPair(object, pair);

        if((*curr_index)<len && json[*curr_index]==','){ (*curr_index)++;}
        skip_whitespace(json , curr_index, len);

        // printf("curr_index 4 :  %d, char at curr_index : %c\n" , *curr_index, json[*curr_index]);
    }
    if((*curr_index)<len) (*curr_index)++;
    else{
        //TODO: Error Handling
    }
    skip_whitespace(json, curr_index, len);

    IRValue *value=createValue(IRTYPE_OBJECT, (void*)object);
    return value;
}

IRValue* parseArray(char*json, int *curr_index, int len){
    skip_whitespace(json, curr_index, len);
    IRArray *array=createArray();
    while((*curr_index)<len && json[*curr_index]!=']'){
        IRValue* value=parseValue(json, curr_index, len);
        skip_whitespace(json, curr_index, len);
        // printf("tdone\n");
        array= addValue(array, value);
        // printf("done\n");

        if((*curr_index)<len && json[*curr_index]==','){ (*curr_index)++;}
        skip_whitespace(json , curr_index, len);
        //  printf("curr_index 5 : %d, char at curr index : %c\n", *curr_index, json[*curr_index]);

    }

   
    if((*curr_index)<len) (*curr_index)++;
    else{
        //TODO: Error Handling
    }

    IRValue *value= createValue(IRTYPE_ARRAY, (void*)array);
    return value;

}


IRValue* parseJson(char *json, int *curr_index, int len){

    skip_whitespace(json, curr_index, len);


    if(json[*curr_index]=='{'){
        (*curr_index)++;
        IRValue *value= parseObject(json, curr_index, len);
        IRObject *obj=value->value;
        return value;
    }else if(json[*curr_index]=='['){
        (*curr_index)++;
        return parseArray(json, curr_index, len);

    }

    return NULL;
}

#endif


