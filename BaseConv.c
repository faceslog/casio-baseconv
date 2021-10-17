/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "fxlib.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "EasyInput.h"

typedef unsigned short BOOL

#define TRUE 1
#define FALSE 0

#define STR_MAX 255
#define STR_BASE 3

// ------------------------- Iterative functions to implement `itoa()` function in C ------------------------- 
// Function to swap two numbers
void swap(char *x, char *y) 
{
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) 
        swap(&buffer[i++], &buffer[j--]);
    
    return buffer;
}
 
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    int n, i, r;
    // invalid input
    if (base < 2 || base > 32)
        return buffer;
 
    // consider the absolute value of the number
    n = abs(value);
 
    i = 0;
    while (n)
    {
        r = n % base;
 
        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else 
            buffer[i++] = 48 + r;
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) 
        buffer[i++] = '0';
    
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}
// ------------------------- End `itoa()` function in C ------------------------- 

// Convert arbitrary base to base 10
// - Input must be valid number in the given base
// - Base must be between 2 and 36
// - If input or base are invalid, returns 0
int ConvertTo10(const char* input, int base)
{
    BOOL isNegative;
    int startIndex, endIndex, digitValue, i, value;
    char c;

    if(base < 2 || base > 36)
        return 0;

    isNegative = (input[0] == '-');

    startIndex = (int) strlen(input) - 1;
    endIndex   = isNegative ? TRUE : FALSE;

    value = 0;
    digitValue = 1;

    for(i = startIndex; i >= endIndex; --i)
    {
        c = input[i];

        // Uppercase it - NOTE: could be done with std::toupper
        if(c >= 'a' && c <= 'z')
            c -= ('a' - 'A');

        // Convert char to int value - NOTE: could be done with std::atoi
        // 0-9
        if(c >= '0' && c <= '9')
            c -= '0';
            // A-Z
        else
            c = c - 'A' + 10;

        if(c >= base)
            return 0;

        // Get the base 10 value of this digit
        value += c * digitValue;

        // Each digit has value base^digit position - NOTE: this avoids pow
        digitValue *= base;
    }

    if(isNegative)
        value *= -1;

    return value;
}

// Convert from one base to another
void ConvertBase(const char* input, int baseFrom, int baseTo)
{
    // NOTE: There is probably a more efficient way to convert between two bases.
    // This however is easy to understand and debug.
    char* str = malloc(STR_MAX);
    itoa(ConvertTo10(input, baseFrom), str, baseTo);

    locate(1,7);
    Print((unsigned char*)"Result:");
    locate(1,8);
    Print(str);

    free(str);
}

void move(int x, int y)
{
    EI_manage_config(EI_SET_COLUMN, x);
    EI_manage_config(EI_SET_ROW, y);
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{   
    unsigned int key;
    char* str, *base_from, *base_to;

    while(1)
    {
        Bdisp_AllClr_DDVRAM();

        EI_init();
        EI_manage_config(EI_SET_START_MODE, EI_NORMAL);

        str = malloc(STR_MAX);
        base_from = malloc(STR_BASE);
        base_to = malloc(STR_BASE);
        
        locate(1,1);
        Print((unsigned char*)"Input NB to convert:");
        move(1, 2);
        str = EI_input_string(STR_MAX, (const char*)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        
        locate(1,3);
        Print((unsigned char*)"Input BaseFrom:");
        move(1,4);
        base_from = EI_input_string(STR_BASE, (const char*)"0123456789");

        locate(1,5);
        Print((unsigned char*)"Input BaseTo:");
        move(1,6);
        base_to = EI_input_string(STR_BASE, (const char*)"0123456789");

        ConvertBase(str, atoi(base_from), atoi(base_to));

        free(str);
        free(base_from);
        free(base_to);

        do 
        {
            GetKey(&key);

        } while (key != KEY_CTRL_AC);
    }

    return 1;
}

//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

