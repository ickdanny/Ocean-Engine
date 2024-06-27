#include "Unknown_Value.h"

#include "PGUtil.h"

/*
 * Returns true if the two specified values are equal,
 * false otherwise
 */
bool unValueEquals(UNValue a, UNValue b){
    if(a.type != b.type){
        return false;
    }
    switch(a.type){
        case un_bool:
            return unAsBool(a) == unAsBool(b);
        case un_number:
            return unAsNumber(a) == unAsNumber(b);
        default:
            pgError(
                "unexpected default in value equals; "
                SRC_LOCATION
            );
            return false;
    }
}

/* todo: prints the specified value */
void unValuePrint(UNValue value){
    switch(value.type){
        case un_bool:
            printf(unAsBool(value) ? "true" : "false");
            break;
        case un_number:
            printf("%g", unAsNumber(value));
            break;
        case un_invalidValue:
            printf("invalid value");
            break;
    }
}