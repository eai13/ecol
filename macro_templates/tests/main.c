#include <stdio.h>
#include <string.h>

#define TEMPLATE_FUNCTION_CALL_N(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, ...) \
	TEMPLATE_FUNCTION_CALL_N_(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, __VA_ARGS__, PP_RSEQ_N())

#define TEMPLATE_FUNCTION_CALL_N_(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, ...) \
	TEMPLATE_FUNCTION_ARGN(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, __VA_ARGS__)

#define TEMPLATE_FUNCTION_ARGN(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59, N, ...) \
    TEMPLATE_FUNCTION_##N(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, \
                          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                          _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                          _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
                          _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
                          _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
                          _51,_52,_53,_54,_55,_56,_57,_58,_59)

#define PP_RSEQ_N() \
59,58,57,56,55,54,53,52,51,50, \
49,48,47,46,45,44,43,42,41,40, \
39,38,37,36,35,34,33,32,31,30, \
29,28,27,26,25,24,23,22,21,20, \
19,18,17,16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0

#define UNBRACE(N) N

#define TEMPLATE_FUNCTION_SET_TYPES(TYPEDEFS, TYPES) \
    typedef TYPES TYPEDEFS;

#define TEMPLATE_FUNCTION(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, ...) \
    TEMPLATE_FUNCTION_CALL_N(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, __VA_ARGS__)

#define PREPROC_DEF #define
#define PREPROC_UNDEF #undef
#define PREPROC_NEWLINE /*
*/

#define TEMPLATE_FUNCTION_2(FUNC_RETURN, FUNC_NAME, FUNC_BODY, TYPEDEFS, TYPES_1, TYPES_2, ...) \
    PREPROC_DEF TYPEDEFS TYPES_1 PREPROC_NEWLINE\
    FUNC_RETURN FUNC_NAME ## _ ## TYPEDEFS ## _ ## TYPES_1 FUNC_BODY PREPROC_NEWLINE\
    PREPROC_UNDEF TYPEDEFS PREPROC_NEWLINE\
    PREPROC_DEF TYPEDEFS TYPES_2 PREPROC_NEWLINE\
    FUNC_RETURN FUNC_NAME ## _ ## TYPEDEFS ## _ ## TYPES_2 FUNC_BODY PREPROC_NEWLINE\
    PREPROC_UNDEF TYPEDEFS PREPROC_NEWLINE\
    PREPROC_DEF FUNC_NAME(x) _Generic((x), TYPES_1 : FUNC_NAME ## _ ## TYPEDEFS ## _ ## TYPES_1, TYPES_2 : FUNC_NAME ## _ ## TYPEDEFS ## _ ## TYPES_2) (x)

typedef struct{
    int a;
}mystruct_1;

typedef struct{
    int a;
}mystruct_2;

TEMPLATE_FUNCTION(
Type1, myinc, (Type1 a){
    a.a += 1;
    return a;
}, Type1, mystruct_1, mystruct_2)

int main(){
    mystruct_1 str_1;
    str_1.a = 1;
    mystruct_2 str_2;
    str_2.a = 2;
    printf("INC %d %d\r\n", myinc(str_1).a, myinc(str_2).a);
    return 0;
}