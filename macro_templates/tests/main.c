#include <stdio.h>
#include <string.h>

#include "../macro_templates.h"

typedef struct{
    int a;
}mystruct_1;

typedef struct{
    int a;
}mystruct_2;

typedef struct{
    int a;
}mystruct_3;

typedef struct{
    int a;
}mystruct_4;

typedef struct{
    int a;
}mystruct_5;

typedef struct{
    int a;
}mystruct_6;

typedef struct{
    int a;
}mystruct_7;

typedef struct{
    int a;
}mystruct_8;

typedef struct{
    int a;
}mystruct_9;

typedef struct{
    int a;
}mystruct_10;

typedef struct{
    int a;
}mystruct_11;

DECLARE_TEMPLATE_FUNCTION(
Type2, myinc, (Type1 a){
    Type2 resStr;
    resStr.a += a.a * 2;
    return resStr;
},
(Type1,      Type2),
(mystruct_1, mystruct_4),
(mystruct_2, mystruct_4),
(mystruct_3, mystruct_4)
)

int main(){
    mystruct_1 str_1;
    str_1.a = 1;
    mystruct_2 str_2;
    str_2.a = 2;
    mystruct_3 str_3;
    str_3.a = 3;
    printf("INC %d %d %d\r\n", myinc(str_1).a, myinc(str_2).a, myinc(str_3).a);
    return 0;
}