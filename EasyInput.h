#ifndef _EASYINPUT
#define _EASYINPUT

#include "fxlib.h"

enum
{
	EI_NORMAL = 1,
	EI_ALPHA,
	EI_ALPHA_LOCKED
};

enum
{
	EI_ALIGN_LEFT = 1,
	EI_ALIGN_CENTER,
	EI_ALIGN_RIGHT
};

enum
{
	EI_GET_ALL = -1,
	EI_SET_COLUMN,
	EI_SET_ROW,
	EI_SET_BOX_LENGTH,
	EI_SET_ALIGN,
	EI_SET_START_MODE,
	EI_SET_Aa_KEY
};

typedef struct
{
	unsigned int column;
	unsigned int row;
	unsigned int box_length;
	unsigned int align;
	unsigned int start_mode;
	unsigned int Aa_key;
} EI_config;

typedef struct
{
    int flashstyle;    // -1 if not flashing, else flashmode
    int column; // starts at 1
    int row;    // starts at 1
    int graphic_mode;
} EI_cursor_settings;


char *EI_input_string(int string_length, const char *chars_allowed);
const void *EI_manage_config(int parameter, int value);
void EI_init(void);
int EI_str_length(const char *str);
int EI_check_char(char c, const char* chars_allowed);

void EI_KBD_PutKey(unsigned int keycode, int mode);
int EI_Cursor_SetPosition(char column, char row);
int EI_Cursor_SetFlashOn(char flashstyle);
void EI_Cursor_SetFlashOff(void);
int EI_Cursor_GetSettings(EI_cursor_settings *cursor_settings);

#endif