#ifndef CONSTRUCTURE_STRING_H
#define CONSTRUCTURE_STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "PGUtil.h"
#include "Trifecta_Uchar.h"

/* 
 * For index searching operations, failure
 * to find will be signaled by a return of
 * this constant equivalent to SIZE_MAX
 */
#define indexNotFound SIZE_MAX

/* Utility macro for +1 for readability */
#define lengthIncludingNull(length) \
    (length + ((unsigned char)1u))

/*
 * Unlike the generic containers in the rest
 * of Constructure, the String types will
 * only accept integral character types and
 * will have a different container type for
 * each - implemented via this type-declare 
 * macro:
 */
#define CONSTRUCTURE_STRING_DECL( \
    TYPENAME, \
    PREFIX, \
    CHARTYPE \
) \
\
/* \
 * A wrapper for a continguous null terminated \
 * fixed width character array on the heap \
 */ \
typedef struct TYPENAME{ \
    CHARTYPE *_ptr; \
\
    /* does not include null terminator */ \
    size_t length; \
\
    size_t _capacity; \
} TYPENAME; \
\
/* \
 * Counts the number of characters in a null \
 * terminated C string of the character type, \
 * not including the null terminator itself \
 */ \
extern inline size_t _##PREFIX##CLength( \
    const CHARTYPE *cStringPtr \
){ \
    assertNotNull( \
        cStringPtr, \
        "null in _CLength; " \
        SRC_LOCATION \
    ); \
\
    size_t charCount = 0u; \
\
    /* assumes null terminated */ \
    while(*cStringPtr){  \
        ++cStringPtr; \
        ++charCount; \
    } \
    return charCount; \
} \
\
/* \
 * Creates a string copy of the given null \
 * terminated C string and returns it by value \
 */ \
extern inline TYPENAME PREFIX##MakeC( \
    const CHARTYPE *cStringPtr \
){ \
    TYPENAME toRet = {0}; \
    toRet.length \
        = _##PREFIX##CLength(cStringPtr); \
    toRet._capacity = lengthIncludingNull( \
        toRet.length \
    ); \
    toRet._ptr = (CHARTYPE *)pgAlloc( \
        toRet._capacity,  \
        sizeof(CHARTYPE) \
    ); \
\
    /*  \
     * use memcpy instead of a custom \
     * _cStringCopy since the lengths of strings \
     * are already stored \
     */ \
    memcpy( \
        toRet._ptr,  \
        cStringPtr,  \
        toRet._capacity * sizeof(CHARTYPE) \
    ); \
    return toRet; \
} \
\
/* \
 * Creates a new empty string and reserves enough \
 * space for the requested number of characters \
 * including the null terminator \
 */ \
extern inline TYPENAME PREFIX##MakeAndReserve( \
    size_t initCapacity \
){ \
    assertFalse( \
        initCapacity == 0u, \
        "trying to reserve 0 capacity; " \
        SRC_LOCATION \
    ); \
    TYPENAME toRet = {0}; \
    toRet.length = 0u; \
    toRet._capacity = initCapacity; \
    toRet._ptr = (CHARTYPE *)pgAlloc( \
        toRet._capacity,  \
        sizeof(CHARTYPE) \
    ); \
    return toRet; \
} \
\
/* \
 * Creates and allocates space for an \
 * empty string \
 */ \
extern inline TYPENAME PREFIX##MakeEmpty(){ \
    enum{ stringInitCapacity = 16u }; \
    return PREFIX##MakeAndReserve( \
        stringInitCapacity \
    ); \
} \
\
/* \
 * Makes a copy of the given string and returns \
 * it by value \
 */ \
extern inline TYPENAME PREFIX##Copy( \
    const TYPENAME *toCopyPtr \
){ \
    TYPENAME toRet = {0}; \
    toRet.length = toCopyPtr->length; \
\
    /*  \
     * allocate just enough space for  \
     * the copy  \
     */ \
    toRet._capacity = lengthIncludingNull( \
        toRet.length \
    ); \
    toRet._ptr = (CHARTYPE *)pgAlloc( \
        toRet._capacity,  \
        sizeof(CHARTYPE) \
    ); \
\
    /*  \
     * use memcpy instead of a custom \
     * _cStringCopy since the lengths of strings \
     * are already stored \
     */ \
    memcpy( \
        toRet._ptr,  \
        toCopyPtr->_ptr,  \
        toRet._capacity * sizeof(CHARTYPE) \
    ); \
\
    return toRet; \
} \
\
/* \
 * Copies the contents of the source string \
 * into the destination string \
 */ \
extern inline void PREFIX##CopyInto( \
    TYPENAME *destPtr, \
    const TYPENAME *srcPtr \
){ \
    /* allocate more space if needed */ \
    if(destPtr->_capacity <= srcPtr->length){ \
        destPtr->_capacity = lengthIncludingNull( \
            srcPtr->length \
        ); \
        destPtr->_ptr = (CHARTYPE *)pgRealloc( \
            destPtr->_ptr, \
            destPtr->_capacity, \
            sizeof(CHARTYPE) \
        ); \
    } \
\
    /*  \
     * use memcpy instead of a custom \
     * _cStringCopy since the lengths of strings \
     * are already stored \
     */ \
    memcpy( \
        destPtr->_ptr,  \
        srcPtr->_ptr,  \
        destPtr->_capacity * sizeof(CHARTYPE) \
    ); \
} \
\
/* \
 * Returns true if the given string is empty, \
 * false otherwise \
 */ \
extern inline bool PREFIX##IsEmpty( \
    const TYPENAME *stringPtr \
){ \
    return stringPtr->length == 0u; \
} \
\
/* \
 * Clears the given string but does not deallocate \
 * its memory \
 */ \
extern inline void PREFIX##Clear( \
    TYPENAME *stringPtr \
){ \
    memset( \
        stringPtr->_ptr, \
        0u, \
        stringPtr->_capacity * sizeof(CHARTYPE) \
    ); \
    stringPtr->length = 0u; \
} \
\
/* \
 * Reallocates more space for the given string  \
 * if its capacity does not fit its nominal  \
 * length; returns false as error code, true \
 * otherwise \
 */ \
extern inline bool _##PREFIX##GrowToFitNewLength( \
    TYPENAME *stringPtr \
){ \
    if(stringPtr->length >= stringPtr->_capacity){ \
        stringPtr->_capacity = lengthIncludingNull( \
            stringPtr->length \
        ); \
        stringPtr->_ptr = (CHARTYPE *)pgRealloc( \
            stringPtr->_ptr, \
            stringPtr->_capacity, \
            sizeof(CHARTYPE) \
        ); \
        if(!(stringPtr->_ptr)){ \
            return false; \
        } \
    } \
    return true; \
} \
\
/* \
 * Reallocates more space for the given string \
 * if it is currently at or above capacity;  \
 * returns false as error code, true otherwise \
 */ \
extern inline bool _##PREFIX##GrowIfNeeded( \
    TYPENAME *stringPtr \
) { \
    enum{ growRatio = 2u }; \
    if(lengthIncludingNull(stringPtr->length) \
        >= stringPtr->_capacity \
    ){ \
        stringPtr->_capacity *= growRatio; \
        ++(stringPtr->_capacity); \
        stringPtr->_ptr = (CHARTYPE *)pgRealloc( \
            stringPtr->_ptr, \
            stringPtr->_capacity, \
            sizeof(CHARTYPE) \
        ); \
        if(!(stringPtr->_ptr)){ \
            return false; \
        } \
    } \
    return true;   \
} \
\
/*  \
 * Prepends the given null terminated C string \
 * of the specified length onto the front of  \
 * the given string; do not attempt to pass in \
 * the same string twice \
 */ \
extern inline void _##PREFIX##PrependHelper( \
    TYPENAME *stringPtr, \
    const CHARTYPE *toPrependPtr, \
    size_t toPrependLength \
){ \
    assertFalse( \
        stringPtr->_ptr == toPrependPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
    if(!toPrependLength){ \
        return; \
    } \
\
    size_t originalLength = stringPtr->length; \
    stringPtr->length += toPrependLength; \
\
    /* allocate more space if needed */ \
    assertTrue( \
        _##PREFIX##GrowToFitNewLength(stringPtr), \
        "failed to grow for Prepend; " \
        SRC_LOCATION \
    ); \
\
    memmove( \
        stringPtr->_ptr + toPrependLength, \
        stringPtr->_ptr, \
        /* copy null */ \
        lengthIncludingNull(originalLength) \
            * sizeof(CHARTYPE) \
    ); \
    /* use memcpy; length is known */ \
    memcpy( \
        stringPtr->_ptr,  \
        toPrependPtr, \
        /* do not copy null */ \
        toPrependLength * sizeof(CHARTYPE) \
    ); \
} \
\
/*  \
 * Prepends the given null terminated C string \
 * onto the front of the given string; do not \
 * attempt to pass in the same string twice \
 */ \
extern inline void PREFIX##PrependC( \
    TYPENAME *stringPtr, \
    const CHARTYPE *toPrependPtr \
){ \
    size_t toPrependLength = _##PREFIX##CLength( \
        toPrependPtr \
    ); \
   _##PREFIX##PrependHelper( \
        stringPtr, \
        toPrependPtr, \
        toPrependLength \
    ); \
} \
\
/*  \
 * Prepends the second given string onto the \
 * front of the first; do not attempt to pass \
 * in the same string twice \
 */ \
extern inline void PREFIX##Prepend( \
    TYPENAME *stringPtr, \
    TYPENAME *toPrependPtr \
){ \
    assertFalse( \
        stringPtr == toPrependPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
   _##PREFIX##PrependHelper( \
        stringPtr, \
        toPrependPtr->_ptr, \
        toPrependPtr->length \
    ); \
} \
\
/* \
 * Appends the given null terminated C string \
 * of the specified length onto the back of the  \
 * given string; do not attempt to pass in the \
 * same string twice \
 */ \
extern inline void _##PREFIX##AppendHelper( \
    TYPENAME *stringPtr, \
    const CHARTYPE *toAppendPtr, \
    size_t toAppendLength \
){ \
    assertFalse( \
        stringPtr->_ptr == toAppendPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
    if(!toAppendLength){ \
        return; \
    } \
\
    size_t originalLength = stringPtr->length; \
    stringPtr->length += toAppendLength; \
\
    /* allocate more space if needed */ \
    assertTrue( \
        _##PREFIX##GrowToFitNewLength(stringPtr), \
        "failed to grow for Append; " \
        SRC_LOCATION \
    ); \
\
    /* use memcpy; length is known */ \
    memcpy( \
        stringPtr->_ptr + originalLength, \
        toAppendPtr, \
        /* copy null */ \
        lengthIncludingNull(toAppendLength) \
            * sizeof(CHARTYPE) \
    ); \
} \
\
/* \
 * Appends the given null terminated C string \
 * onto the back of the given string; do not \
 * attempt to pass in the same string twice \
 */ \
extern inline void PREFIX##AppendC( \
    TYPENAME *stringPtr, \
    const CHARTYPE *toAppendPtr \
){ \
    size_t toAppendLength = _##PREFIX##CLength( \
        toAppendPtr \
    ); \
    _##PREFIX##AppendHelper( \
        stringPtr,  \
        toAppendPtr,  \
        toAppendLength \
    ); \
} \
\
/* \
 * Appends the second given string onto the back \
 * of the first; do not attempt to pass in the \
 * same string twice \
 */ \
extern inline void PREFIX##Append( \
    TYPENAME *stringPtr, \
    TYPENAME *toAppendPtr \
){ \
    assertFalse( \
        stringPtr == toAppendPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
    _##PREFIX##AppendHelper( \
        stringPtr, \
        toAppendPtr->_ptr, \
        toAppendPtr->length \
    ); \
} \
 \
/* \
 * Writes a null terminator at the back of \
 * the given string after its length \
 */ \
extern inline void _##PREFIX##WriteNull( \
    TYPENAME *stringPtr \
){ \
    assertTrue( \
        stringPtr->_capacity > stringPtr->length, \
        "bad capacity in writeNull; " \
        SRC_LOCATION \
    ); \
\
    (stringPtr->_ptr)[stringPtr->length] = 0u; \
} \
\
/* \
 * Pushes the given char onto the back of the \
 * given string \
 */ \
extern inline void PREFIX##PushBack( \
    TYPENAME *stringPtr, \
    CHARTYPE toPush \
){ \
    assertTrue( \
       _##PREFIX##GrowIfNeeded(stringPtr), \
        "failed to grow for PushBack; " \
        SRC_LOCATION \
    ); \
\
    /* overwrite current null terminator */ \
    (stringPtr->_ptr)[stringPtr->length] = toPush; \
    ++(stringPtr->length); \
\
    /* write new null terminator */ \
    _##PREFIX##WriteNull(stringPtr); \
} \
\
/* \
 * Removes the last character of the given \
 * string; error if empty \
 */ \
extern inline void PREFIX##PopBack( \
    TYPENAME *stringPtr \
){ \
    assertFalse( \
        PREFIX##IsEmpty(stringPtr), \
        "empty in PopBack; " SRC_LOCATION \
    ); \
\
    /* write new null terminator */ \
    --(stringPtr->length); \
    _##PREFIX##WriteNull(stringPtr); \
} \
\
/* \
 * Erases the character of the given string \
 * at the given index \
 */ \
extern inline void PREFIX##EraseChar( \
    TYPENAME *stringPtr, \
    size_t index \
){ \
    assertTrue( \
        index < stringPtr->length, \
        "bad index; " SRC_LOCATION \
    ); \
\
    --(stringPtr->length); \
\
    /* move all later characters 1 forward */ \
    /* length is 1 if we remove back */ \
    memmove( \
        stringPtr->_ptr + index, \
        stringPtr->_ptr + (index + 1u), \
        /* move null terminator forward also */ \
        sizeof(CHARTYPE) * lengthIncludingNull( \
            stringPtr->length - index \
        ) \
    ); \
} \
\
/* \
 * Returns the character of the given string \
 * at the given index \
 */ \
extern inline CHARTYPE PREFIX##CharAt( \
    const TYPENAME *stringPtr, \
    size_t index \
){ \
    assertTrue( \
        index < stringPtr->length, \
        "bad index; " SRC_LOCATION \
    ); \
\
    return (stringPtr->_ptr)[index]; \
} \
\
/* \
 * Returns the front character of the given \
 * string \
 */ \
extern inline CHARTYPE PREFIX##Front( \
    const TYPENAME *stringPtr \
){ \
    assertFalse( \
        PREFIX##IsEmpty(stringPtr), \
        "empty in Front; " SRC_LOCATION \
    ); \
\
    return (stringPtr->_ptr)[0u]; \
} \
\
/* \
 * Returns the back character of the given  \
 * string \
 */ \
extern inline CHARTYPE PREFIX##Back( \
    const TYPENAME *stringPtr \
){ \
    assertFalse( \
        PREFIX##IsEmpty(stringPtr), \
        "empty in Back; " SRC_LOCATION \
    ); \
\
    return (stringPtr->_ptr)[ \
        stringPtr->length - 1u \
    ]; \
} \
\
/* \
 * Sets the character of the given string \
 * at the given index to the given value, \
 * overwriting its previous value \
 */ \
extern inline void PREFIX##SetChar( \
    TYPENAME *stringPtr, \
    size_t index, \
    CHARTYPE newValue \
){ \
    assertTrue( \
        index < stringPtr->length, \
        "bad index; " SRC_LOCATION \
    ); \
\
    (stringPtr->_ptr)[index] = newValue; \
} \
\
/* \
 * Inserts the given character into the given \
 * string at the given index by making room \
 * for the new character \
 */ \
extern inline void PREFIX##InsertChar( \
    TYPENAME *stringPtr, \
    size_t index, \
    CHARTYPE toInsert \
){ \
    assertTrue( \
        index <= stringPtr->length, \
        "bad index; " SRC_LOCATION \
    ); \
\
    assertTrue( \
       _##PREFIX##GrowIfNeeded(stringPtr), \
        "failed to grow for InsertChar; " \
        SRC_LOCATION \
    ); \
\
    /* move all later characters 1 back */ \
    /* length is 1 if we insert as new back */ \
    memmove( \
        stringPtr->_ptr + (index + 1u), \
        stringPtr->_ptr + index, \
        /* move null terminator back also */ \
        sizeof(CHARTYPE) * lengthIncludingNull( \
            stringPtr->length - index \
        ) \
    ); \
    ++(stringPtr->length); \
\
    /* insert the new character */ \
    (stringPtr->_ptr)[index] = toInsert;     \
} \
\
/* \
 * Inserts the given null terminated C string \
 * of the specified length into the given \
 * string starting at the given index by making \
 * room for the new characters; do not attempt \
 * to pass the same string as both the base and \
 * the insertion \
 */ \
extern inline void _##PREFIX##InsertHelper( \
    TYPENAME *stringPtr, \
    size_t startingIndex, \
    const CHARTYPE *toInsertPtr, \
    size_t toInsertLength \
){ \
    assertFalse( \
        stringPtr->_ptr == toInsertPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
    assertTrue( \
        startingIndex <= stringPtr->length, \
        "bad starting index; " SRC_LOCATION \
    ); \
\
    if(!toInsertLength){ \
        return; \
    } \
\
    size_t originalLength = stringPtr->length; \
    stringPtr->length += toInsertLength; \
\
    /* allocate more space if needed */ \
    assertTrue( \
        _##PREFIX##GrowToFitNewLength(stringPtr), \
        "failed to grow for Insert; " \
        SRC_LOCATION \
    ); \
\
    /*  \
     * move all later elements backwards to \
     * fit the new string \
     */ \
    /* length is 1 if we insert at the back */ \
    memmove( \
        stringPtr->_ptr  \
            + (startingIndex + toInsertLength), \
        stringPtr->_ptr + startingIndex, \
        /* move null terminator back also */ \
        sizeof(CHARTYPE) * lengthIncludingNull( \
            originalLength - startingIndex \
        ) \
    ); \
\
    /* copy in the insertion */ \
    /* use memmove just to be safe */ \
    memmove( \
        stringPtr->_ptr + (startingIndex), \
        toInsertPtr, \
        toInsertLength * sizeof(CHARTYPE) \
    ); \
} \
\
/* \
 * Inserts the given null terminated C string \
 * into the given string starting at the given \
 * index by making room for the new characters; \
 * do not attempt to pass the same string as \
 * both the base and the insertion \
 */ \
extern inline void PREFIX##InsertC( \
    TYPENAME *stringPtr, \
    size_t startingIndex, \
    const CHARTYPE *toInsertPtr \
){ \
    size_t toInsertLength = _##PREFIX##CLength( \
        toInsertPtr \
    ); \
    _##PREFIX##InsertHelper( \
        stringPtr,  \
        startingIndex,  \
        toInsertPtr,  \
        toInsertLength \
    ); \
} \
\
/* \
 * Inserts the latter given string into the former \
 * starting at the given index by making room for  \
 * the new characters; do not attempt to pass the  \
 * same string as both the base and the insertion \
 */ \
extern inline void PREFIX##Insert( \
    TYPENAME *stringPtr, \
    size_t startingIndex, \
    const TYPENAME *toInsertPtr \
){ \
    assertFalse( \
        stringPtr == toInsertPtr, \
        "do not pass in the same string; " \
        SRC_LOCATION \
    ); \
\
    _##PREFIX##InsertHelper( \
        stringPtr, \
        startingIndex, \
        toInsertPtr->_ptr, \
        toInsertPtr->length \
    ); \
} \
\
/* \
 * Returns the index of the first occurrence \
 * of the given character in the given string, \
 * or returns indexNotFound if no such \
 * character exists \
 */ \
extern inline size_t PREFIX##IndexOfChar( \
    const TYPENAME *stringPtr, \
    CHARTYPE toFind \
){ \
    /* linear search */ \
    for( \
        size_t i = 0u;  \
        i < stringPtr->length;  \
        ++i \
    ){ \
        if((stringPtr->_ptr)[i] == toFind){ \
            return i; \
        } \
    } \
    return indexNotFound; \
} \
\
/* \
 * Returns the index of the last occurrence \
 * of the given character in the given string, \
 * or returns indexNotFound if no such character \
 * exists \
 */ \
extern inline size_t PREFIX##LastIndexOfChar( \
    const TYPENAME *stringPtr, \
    CHARTYPE toFind \
){ \
    /* linear search from back */ \
    /* \
     * Because the following loop is decrementing \
     * an unsigned integral variable to zero, it \
     * must end only when it overflows to its \
     * maximum representable value. If the string \
     * is of length zero, then i starts at SIZE_MAX \
     * thus the behavior of the loop is expected. \
     * If the string is of length SIZE_MAX, then i  \
     * starts at the predecessor integer. \
     */ \
    for( \
        size_t i = stringPtr->length - 1u; \
        i != SIZE_MAX; \
        --i \
    ){ \
        if((stringPtr->_ptr)[i] == toFind){ \
            return i; \
        } \
    } \
    return indexNotFound; \
} \
\
/* \
 * Returns the starting index of the first \
 * occurrence of the given C string of the \
 * specified length in the given string, or  \
 * returns indexNotFound if no such substring  \
 * exists \
 */ \
extern inline size_t _##PREFIX##IndexOfHelper( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *targetPtr, \
    size_t subLength \
){ \
    /* must check to avoid overflow below */ \
    if(subLength > stringPtr->length){ \
        return indexNotFound; \
    } \
\
    /* used in inner loop to iterate over target */ \
    size_t j = 0u; \
\
    /* brute force linear search */ \
    for( \
        size_t i = 0u; \
        /* check until the substring can't fit */ \
        i < stringPtr->length - subLength + 1u; \
        ++i \
    ){ \
        /* check if the first character matches*/ \
        if((stringPtr->_ptr[i]) == *targetPtr){ \
            /* check every other character */ \
            for(j = 1u; j < subLength; ++j){ \
                if(stringPtr->_ptr[i + j]  \
                    != targetPtr[j] \
                ){ \
                    break; \
                } \
            } \
            if(j == subLength){ \
                return i; \
            } \
        } \
    } \
    return indexNotFound; \
    /* for a "better" algorithm see KMP */ \
} \
\
/* \
 * Returns the starting index of the first \
 * occurrence of the given null terminated \
 * C string in the given string, or returns \
 * indexNotFound if no such substring exists \
 */ \
extern inline size_t PREFIX##IndexOfC( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *targetPtr \
){ \
    size_t subLength = \
        _##PREFIX##CLength(targetPtr); \
    return _##PREFIX##IndexOfHelper( \
        stringPtr,  \
        targetPtr,  \
        subLength \
    ); \
} \
\
/* \
 * Returns the starting index of the first \
 * occurrence of the given target string \
 * in the given base string, or returns \
 * indexNotFound if no such substring exists \
 */ \
extern inline size_t PREFIX##IndexOf( \
    const TYPENAME *stringPtr, \
    const TYPENAME *targetPtr \
){ \
    return _##PREFIX##IndexOfHelper( \
        stringPtr, \
        targetPtr->_ptr, \
        targetPtr->length \
    ); \
} \
\
/* \
 * Returns the starting index of the last \
 * occurrence of the given C string of the \
 * specified length in the given string, or \
 * returns indexNotFound if no such substring \
 * exists \
 */ \
extern inline size_t _##PREFIX##LastIndexOfHelper( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *targetPtr, \
    size_t subLength \
){ \
    /* must check to avoid overflow below */ \
    if(subLength > stringPtr->length){ \
        return indexNotFound; \
    } \
\
    /* used in inner loop to iterate over target */ \
    size_t j = 0u; \
\
    /* brute force linear search */ \
    /*  \
     * basically like indexOf algorithm except \
     * the outer loop runs backwards \
     */ \
    for( \
        size_t i = stringPtr->length - subLength; \
        /* see comment in lastIndexOfChar */ \
        i != SIZE_MAX; \
        --i \
    ){ \
        /* check if the first character matches*/ \
        if((stringPtr->_ptr[i]) == *targetPtr){ \
            /* check every other character */ \
            for(j = 1u; j < subLength; ++j){ \
                if(stringPtr->_ptr[i + j]  \
                    != targetPtr[j] \
                ){ \
                    break; \
                } \
            } \
            if(j == subLength){ \
                return i; \
            } \
        } \
    } \
    return indexNotFound; \
    /* for a "better" algorithm see KMP */ \
} \
\
/* \
 * Returns the starting index of the last \
 * occurrence of the given null terminated \
 * C string in the given string, or returns \
 * indexNotFound if no such substring exists \
 */ \
extern inline size_t PREFIX##LastIndexOfC( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *targetPtr \
){ \
    size_t subLength = \
        _##PREFIX##CLength(targetPtr); \
    return _##PREFIX##LastIndexOfHelper( \
        stringPtr,  \
        targetPtr,  \
        subLength \
    ); \
} \
\
/* \
 * Returns the starting index of the last \
 * occurrence of the given target string \
 * in the given base string, or returns \
 * indexNotFound if no such substring exists \
 */ \
extern inline size_t PREFIX##LastIndexOf( \
    const TYPENAME *stringPtr, \
    const TYPENAME *targetPtr \
){ \
    return _##PREFIX##LastIndexOfHelper( \
        stringPtr, \
        targetPtr->_ptr, \
        targetPtr->length \
    ); \
} \
\
/* \
 * Returns true if the given string begins with \
 * the given character, false otherwise \
 */ \
extern inline bool PREFIX##BeginsWithChar( \
    const TYPENAME *stringPtr, \
    CHARTYPE toTest \
){ \
    return stringPtr->length > 0u  \
        && stringPtr->_ptr[0u] == toTest; \
} \
\
/* \
 * Returns true if the given string ends with \
 * the given character, false otherwise \
 */ \
extern inline bool PREFIX##EndsWithChar( \
    const TYPENAME *stringPtr, \
    CHARTYPE toTest \
){ \
    return stringPtr->length > 0u && ( \
        stringPtr->_ptr[stringPtr->length - 1u] \
            == toTest \
    ); \
} \
\
/* \
 * Returns true if the given string begins with \
 * the given null terminated C string of the \
 * specified length, false otherwise \
 */ \
extern inline bool _##PREFIX##BeginsWithHelper( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *toTestPtr, \
    size_t toTestLength \
){ \
    if(stringPtr->length < toTestLength){ \
        return false; \
    } \
    if(toTestLength == 0u){ \
        return false; \
    } \
\
    for(size_t i = 0u; i < toTestLength; ++i){ \
        if(stringPtr->_ptr[i] != toTestPtr[i]){ \
            return false; \
        } \
    } \
    return true; \
} \
\
/* \
 * Returns true if the given string begins with \
 * the given null terminated C string, false \
 * otherwise \
 */ \
extern inline bool PREFIX##BeginsWithC( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *toTestPtr \
){ \
    size_t toTestLength = \
        _##PREFIX##CLength(toTestPtr); \
    return _##PREFIX##BeginsWithHelper( \
        stringPtr, \
        toTestPtr, \
        toTestLength \
    ); \
} \
\
/* \
 * Returns true if the former given string begins \
 * with the latter, false otherwise \
 */ \
extern inline bool PREFIX##BeginsWith( \
    const TYPENAME *stringPtr, \
    const TYPENAME *toTestPtr \
){ \
    return _##PREFIX##BeginsWithHelper( \
        stringPtr, \
        toTestPtr->_ptr, \
        toTestPtr->length \
    ); \
} \
\
/* \
 * Returns true if the given string ends with \
 * the given null terminated C string of the \
 * specified length, false otherwise \
 */ \
extern inline bool _##PREFIX##EndsWithHelper( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *toTestPtr, \
    size_t toTestLength \
){ \
    if(stringPtr->length < toTestLength){ \
        return false; \
    } \
    if(toTestLength == 0u){ \
        return false; \
    } \
\
    /* index to check in the string */ \
    size_t checkIndex  \
        = stringPtr->length - toTestLength; \
    for( \
        size_t i = 0u;  \
        i < toTestLength;  \
        ++i, ++checkIndex \
    ){ \
        if(stringPtr->_ptr[checkIndex]  \
            != toTestPtr[i] \
        ){ \
            return false; \
        } \
    } \
    return true; \
} \
\
/* \
 * Returns true if the given string ends with \
 * the given null terminated C string, false \
 * otherwise \
 */ \
extern inline bool PREFIX##EndsWithC( \
    const TYPENAME *stringPtr, \
    const CHARTYPE *toTestPtr \
){ \
    size_t toTestLength = \
        _##PREFIX##CLength(toTestPtr); \
    return _##PREFIX##EndsWithHelper( \
        stringPtr, \
        toTestPtr, \
        toTestLength \
    ); \
} \
\
/* \
 * Returns true if the former given string ends \
 * with the latter, false otherwise \
 */ \
extern inline bool PREFIX##EndsWith( \
    const TYPENAME *stringPtr, \
    const TYPENAME *toTestPtr \
){ \
    return _##PREFIX##EndsWithHelper( \
        stringPtr, \
        toTestPtr->_ptr, \
        toTestPtr->length \
    ); \
} \
\
/* \
 * Returns a newly created string by value \
 * which points to a copy of a substring \
 * of the given string; the substring is \
 * specified by inclusive start and end \
 * indices \
 */ \
extern inline TYPENAME PREFIX##Substring( \
    const TYPENAME *stringPtr, \
    size_t startIndexInclusive, \
    size_t endIndexInclusive \
){ \
    assertTrue( \
        endIndexInclusive >= startIndexInclusive, \
        "end index >= start index!; " \
        SRC_LOCATION \
    ); \
\
    assertTrue( \
        startIndexInclusive < stringPtr->length, \
        "bad start index; " SRC_LOCATION \
    ); \
\
    assertTrue( \
        endIndexInclusive < stringPtr->length, \
        "bad end index; " SRC_LOCATION \
    ); \
\
    size_t subLength = endIndexInclusive  \
        - startIndexInclusive + 1u; \
\
    TYPENAME toRet = PREFIX##MakeAndReserve( \
        lengthIncludingNull(subLength) \
    ); \
    toRet.length = subLength; \
\
    for(size_t i = 0u; i < subLength; ++i){ \
        toRet._ptr[i] = stringPtr->_ptr[ \
            startIndexInclusive + i \
        ]; \
    } \
\
    return toRet; \
} \
\
/* \
 * Compares the two given null terminated C \
 * strings of the character type and returns  \
 * >0 if the first is greater than the second,  \
 * <0 if the second is greater than the first, \
 * or 0 if the two are equal. \
 */ \
extern inline int _##PREFIX##CCompare( \
    const CHARTYPE *cStringPtr1, \
    const CHARTYPE *cStringPtr2 \
){ \
    assertNotNull( \
        cStringPtr1, \
        "null str1 in _CCompare; " \
        SRC_LOCATION \
    ); \
    assertNotNull( \
        cStringPtr2, \
        "null str2 in _CCompare; " \
        SRC_LOCATION \
    ); \
\
    /* assumes null terminated */ \
    while( \
        *cStringPtr1  \
        && (*cStringPtr1 == *cStringPtr2) \
    ){ \
        ++cStringPtr1; \
        ++cStringPtr2; \
    } \
\
    /* \
     * Since 1 > 2 is disjoint from 2 > 1, \
     * only operand can be 1; if 1 = 2, then \
     * returns 0 \
     */ \
    return (*cStringPtr1 > *cStringPtr2)  \
        - (*cStringPtr2 > *cStringPtr1); \
} \
\
/* \
 * Compares the two given strings and returns  \
 * >0 if the first is greater than the second,  \
 * <0 if the second is greater than the first, \
 * or 0 if the two are equal. \
 */ \
extern inline int PREFIX##Compare( \
    const TYPENAME *stringPtr1, \
    const TYPENAME *stringPtr2 \
){ \
    return _##PREFIX##CCompare( \
        stringPtr1->_ptr, \
        stringPtr2->_ptr \
    ); \
} \
\
/* \
 * Returns true if the two given strings are \
 * equal, false otherwise \
 */ \
extern inline bool PREFIX##Equals( \
    const TYPENAME *stringPtr1, \
    const TYPENAME *stringPtr2 \
){ \
    /* if lengths unequal, clearly unequal */ \
    if(stringPtr1->length  \
        != stringPtr2->length \
    ){ \
        return false; \
    } \
\
    /*  \
     * logical NOT on cStringCompare to \
     * map 0 to true (1) and !0 to false (0) \
     */ \
    return !_##PREFIX##CCompare( \
        stringPtr1->_ptr, \
        stringPtr2->_ptr \
    ); \
} \
\
/* Frees the given string */ \
extern inline void PREFIX##Free( \
    TYPENAME *strPtr \
){ \
    pgFree(strPtr->_ptr); \
    strPtr->length = 0u; \
    strPtr->_capacity = 0u; \
}
/* end of macro */

CONSTRUCTURE_STRING_DECL(
    String, 
    string, 
    char
)
CONSTRUCTURE_STRING_DECL(
    WideString, 
    wideString, 
    wchar_t
)
CONSTRUCTURE_STRING_DECL(
    String16, 
    string16, 
    char16_t
)
CONSTRUCTURE_STRING_DECL(
    String32, 
    string32, 
    char32_t
)

#endif