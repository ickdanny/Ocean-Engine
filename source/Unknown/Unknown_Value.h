#ifndef UNKNOWN_VALUE_H
#define UNKNOWN_VALUE_H

#include <stdbool.h>

/*
 * Enumerates the types of values that Unknown
 * supports
 */
typedef enum UNValueType{
    un_invalidValue,
    un_bool,
    un_number, //todo: double for now
    un_object,
} UNValueType;

/* A tagged union of values for Unknown */
typedef struct UNValue{
    UNValueType type;
    union{
        bool boolean;
        double number;
        UNObject *object;
    } as;
} UNValue;

/* Constructs a UNValue for the specified bool */
#define unBoolValue(BOOL) \
    ((UNValue){un_bool, {.boolean = (BOOL)}})

/* Constructs a UNValue for the specified number */
#define unNumberValue(NUMBER) \
    ((UNValue){un_number, {.number = (NUMBER)}})

/* Constructs a UNValue for the specified object */
#define unObjectValue(OBJECT) \
    ((UNValue){ \
        un_object, \
        {.object = (UNObject*)(OBJECT)} \
    })

/*
 * Unboxes the specified UNValue as a bool, error if
 * invalid tag
 */
static inline bool unAsBool(UNValue value){
    if(value.type == un_bool){
        return value.as.boolean;
    }
    else{
        pgError("bad access for bool");
        return false;
    }
}

/*
 * Unboxes the specified UNValue as a number, error if
 * invalid tag
 */
static inline double unAsNumber(UNValue value){
    if(value.type == un_number){
        return value.as.number;
    }
    else{
        pgError("bad access for number");
        return 0;
    }
}

/*
 * Unboxes the specified UNValue as an object, error
 * if invalid tag
 */
static inline UNObject *unAsObject(UNValue value){
    if(value.type == un_object){
        return value.as.object;
    }
    else{
        pgError("bad access for object");
        return 0;
    }
}

/*
 * Returns true if the two specified values are equal,
 * false otherwise
 */
bool unValueEquals(UNValue a, UNValue b);

/* prints the specified value */
void unValuePrint(UNValue value);

#endif