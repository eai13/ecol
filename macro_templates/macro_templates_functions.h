#ifndef __ECOL_MACRO_TEMPLATES_FUNCTIONS_H_
#define __ECOL_MACRO_TEMPLATES_FUNCTIONS_H_

#include "macro_templates_global.h"

#define DECLARE_TEMPLATE_FUNCTION_CALL_N(MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, ...) \
    DECLARE_TEMPLATE_FUNCTION_CALL_N_(MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__, PP_RSEQ_N())

#define DECLARE_TEMPLATE_FUNCTION_CALL_N_(MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, ...) \
    DECLARE_TEMPLATE_FUNCTION_CALL_N_OVERLOADS(MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_CALL_N_OVERLOADS(MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59, N, ...) \
    DECLARE_TEMPLATE_FUNCTION_##N((,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, \
                          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                          _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                          _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
                          _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
                          _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
                          _51,_52,_53,_54,_55,_56,_57,_58,_59)

#define DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    DECLARE_TYPE_OVERLOAD_N_(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, UNBRACE MT_TEMPLATE_TYPES, PP_RSEQ_N())

#define DECLARE_TYPE_OVERLOAD_N_(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N_OVERLOADS(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, __VA_ARGS__)

#define DECLARE_TYPE_OVERLOAD_N_OVERLOADS( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59, N, ...) \
    DECLARE_TYPE_OVERLOAD_##N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, \
                          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                          _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                          _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
                          _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
                          _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
                          _51,_52,_53,_54,_55,_56,_57,_58,_59)

#define UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    UNDECLARE_TYPE_OVERLOAD_N_(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, UNBRACE MT_TEMPLATE_TYPES, PP_RSEQ_N())

#define UNDECLARE_TYPE_OVERLOAD_N_(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    UNDECLARE_TYPE_OVERLOAD_N_OVERLOADS(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, __VA_ARGS__)

#define UNDECLARE_TYPE_OVERLOAD_N_OVERLOADS( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59, N, ...) \
    UNDECLARE_TYPE_OVERLOAD_##N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, \
                          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                          _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                          _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
                          _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
                          _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
                          _51,_52,_53,_54,_55,_56,_57,_58,_59)

#define DECLARE_TEMPLATE_FUNCTION_11(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 11 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_10((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 11 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_10(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 10 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_9((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 10 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_9(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 9 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_8((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 9 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_8(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 8 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_7((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 8 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_7(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 7 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_6((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 7 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_6(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 6 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_5((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 6 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_5(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 5 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_4((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 5 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_4(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 4 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_3((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 4 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_3(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 3 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_2((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 3 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_2(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 2 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    DECLARE_TEMPLATE_FUNCTION_1((UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 2 ,), MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, __VA_ARGS__)

#define DECLARE_TEMPLATE_FUNCTION_1(MT_GENERIC_ACCUMULATOR, MT_RETURN_TYPE, MT_FUNCTION_NAME, MT_FUNCTION_BODY, MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE, ...) \
    DECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    MT_RETURN_TYPE MT_FUNCTION_NAME ## _ ## 1 MT_FUNCTION_BODY PREPROCESSOR_NEWLINE \
    UNDECLARE_TYPE_OVERLOAD_N(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPES) PREPROCESSOR_NEWLINE \
    PREPROCESSOR_DEFINE MT_FUNCTION_NAME(x) _Generic((x) UNBRACE MT_GENERIC_ACCUMULATOR MT_OVERLOAD_TYPE : MT_FUNCTION_NAME ## _ ## 1) (x)

#define DECLARE_TYPE_OVERLOAD_5(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_DEFINE TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    DECLARE_TYPE_OVERLOAD_4(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define DECLARE_TYPE_OVERLOAD_4(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_DEFINE TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    DECLARE_TYPE_OVERLOAD_3(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define DECLARE_TYPE_OVERLOAD_3(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_DEFINE TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    DECLARE_TYPE_OVERLOAD_2(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define DECLARE_TYPE_OVERLOAD_2(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_DEFINE TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    DECLARE_TYPE_OVERLOAD_1(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define DECLARE_TYPE_OVERLOAD_1(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_DEFINE TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE

#define UNDECLARE_TYPE_OVERLOAD_5(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_UNDEF TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    UNDECLARE_TYPE_OVERLOAD_4(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define UNDECLARE_TYPE_OVERLOAD_4(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_UNDEF TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    UNDECLARE_TYPE_OVERLOAD_3(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define UNDECLARE_TYPE_OVERLOAD_3(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_UNDEF TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    UNDECLARE_TYPE_OVERLOAD_2(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define UNDECLARE_TYPE_OVERLOAD_2(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_UNDEF TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE
    UNDECLARE_TYPE_OVERLOAD_1(TAKE_EXCEPT_FIRST MT_TEMPLATE_TYPES, TAKE_EXCEPT_FIRST MT_OVERLOAD_TYPE)

#define UNDECLARE_TYPE_OVERLOAD_1(MT_TEMPLATE_TYPES, MT_OVERLOAD_TYPE) \
    PREPROCESSOR_UNDEF TAKE_FIRST MT_TEMPLATE_TYPES TAKE_FIRST MT_OVERLOAD_TYPE

#endif