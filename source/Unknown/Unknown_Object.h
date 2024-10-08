#ifndef UNKNOWN_OBJECT_H
#define UNKNOWN_OBJECT_H

#include "Unknown_Value.h"
#include "Unknown_Program.h"

#include "PGUtil.h"
#include "Constructure.h"

/*
 * Defines the types of objects which Unknown supports
 */
typedef enum UNObjectType{
    un_invalidObject,
    un_stringObject,
    un_funcObject,
    un_nativeFuncObject
} UNObjectType;

/* the base struct for all object types */
typedef struct UNObject{
    UNObjectType type;
    /* forms a linked list of objects for GC */
    struct UNObject *nextPtr;
} UNObject;

/* An object holding a string */
typedef struct UNObjectString{
    UNObject objectBase;
    String string;
    size_t cachedHashCode;
} UNObjectString;

/* An object referring to a function */
typedef struct UNObjectFunc{
    UNObject objectBase;
    int arity;
    UNProgram program;
    UNObjectString *namePtr;
} UNObjectFunc;

/* The prototype for a native function */
typedef UNValue (*UNNativeFunc)(
    int argc,
    UNValue* argv
);

/* An object referring to a native function */
typedef struct UNObjectNativeFunc{
    UNObject objectBase;
    UNNativeFunc func;
} UNObjectNativeFunc;

/*
 * Returns true if the specified value is an object of
 * the given type, false otherwise
 */
static inline bool unObjectCheckType(
    UNValue value,
    UNObjectType expectedType
){
    return unIsObject(value)
        && unAsObject(value)->type == expectedType;
}

/*
 * access the type of the specified object; error if
 * the given UNValue is not actually an object
 */
#define unObjectGetType(VALUE) \
    (unAsObject(VALUE)->type)

/*
 * Returns true if the specified value is a string
 * object, false otherwise
 */
#define unIsString(VALUE) \
    unObjectCheckType((VALUE), un_stringObject)

/*
 * Returns true if the specified value is a func
 * object, false otherwise
 */
#define unIsFunc(VALUE) \
    unObjectCheckType((VALUE), un_funcObject)

/*
 * Returns true if the specified value is a native func
 * object, false otherwise
 */
#define unIsNativeFunc(VALUE) \
    unObjectCheckType((VALUE), un_nativeFuncObject)

/*
 * Returns a pointer to the String Object contained in
 * the specified value, error if the value is not a
 * String object
 */
static inline UNObjectString *unObjectAsString(
    UNValue value
){
    if(!unIsString(value)){
        pgError(
            "value is not a string; "
            SRC_LOCATION
        );
    }
    return (UNObjectString*) unAsObject(value);
}

/*
 * Returns a pointer to the C string contained in the
 * specified value, error if the value is not a String
 * object
 */
static inline char *unObjectAsCString(UNValue value){
    return unObjectAsString(value)->string._ptr;
}

/*
 * Returns a pointer to the func object contained in
 * the specified value, error if the value is not a
 * func object
 */
static inline UNObjectFunc *unObjectAsFunc(
    UNValue value
){
    if(!unIsFunc(value)){
        pgError(
            "value is not a func; "
            SRC_LOCATION
        );
    }
    return (UNObjectFunc*) unAsObject(value);
}

/*
 * Returns a pointer to the native func object
 * contained in the specified value, error if the
 * value is not a func object
 */
static inline UNObjectNativeFunc *unObjectAsNativeFunc(
    UNValue value
){
    if(!unIsNativeFunc(value)){
        pgError(
            "value is not a native func; "
            SRC_LOCATION
        );
    }
    return (UNObjectNativeFunc*) unAsObject(value);
}

/*
 * An equals function for UNObjectString* that actually
 * walks the strings, used for string interning
 */
bool _unObjectStringPtrCharwiseEquals(
    const void *voidPtr1,
    const void *voidPtr2
);

/*
 * Allocates and returns a new UNObjectString by
 * pointer, copying the specified number of characters
 * from the given character pointer, and inserting that
 * object at the start of the given object list
 * (nullable); however, if the string to copy is
 * already interned in the given HashMap, this
 * function simply returns a pointer to the
 * preexisting string and does not insert it in the
 * object list, otherwise inserts the string
 * into the map (NOTE: compile-constant strings are
 * owned by the literals and do not show up in the
 * virtual machine object list)
 */
UNObjectString *unObjectStringCopy(
    const char *chars,
    size_t length,
    UNObject **listHeadPtrPtr,
    HashMap *stringMapPtr
);

/*
 * Allocates and returns a new UNObjectString by
 * pointer, holding the concatenation of the two
 * specified UNObjectStrings, and also inserts that
 * object at the start of the given object list
 * (nullable); however, if the string to copy is
 * already interned in the given HashMap, this
 * function simply returns a pointer to the
 * preexisting string and does not insert it in the
 * object list, otherwise inserts the string
 * into the map (NOTE: compile-constant strings are
 * owned by the literals and do not show up in the
 * virtual machine object list)
 */
UNObjectString *unObjectStringConcat(
    UNObjectString *leftStringPtr,
    UNObjectString *rightStringPtr,
    UNObject **listHeadPtrPtr,
    HashMap *stringMapPtr
);

/*
 * Creates and returns a new UNObjectFunc by pointer;
 * the enclosing function object pointer is nullable
 */
UNObjectFunc *unObjectFuncMake(
    UNObjectFunc *enclosingPtr
);

/*
 * Creates and returns a new UNObjectNativeFunc by
 * pointer and also inserts the object at the start of
 * the given object list
 */
UNObjectNativeFunc *unObjectNativeFuncMake(
    UNNativeFunc func,
    UNObject **listHeadPtrPtr
);

/*
 * Returns true if the two specified objects are equal,
 * false otherwise
 */
bool unObjectEquals(UNObject *a, UNObject *b);

/* Prints the value if it an object, error otherwise */
void unObjectPrint(UNValue value);

/*
 * Frees the memory associated with the specified
 * object
 */
void unObjectFree(UNObject *objectPtr);

/* For use with the Constructure Hashmap */
size_t _unObjectStringPtrHash(const void *voidPtr);

/* For use with the Constructure Hashmap */
bool _unObjectStringPtrEquals(
    const void *voidPtr1,
    const void *voidPtr2
);

#endif