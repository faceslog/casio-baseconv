#include "EasyInput.h"

char *EI_input_string(int string_length, const char *chars_allowed)
{
	char *string = (void*)(0);
	EI_config *config = (void*)(0);
	EI_cursor_settings cursor_settings;

	unsigned int key = 0;

	int i;
	int char_deleted = 0;
	int text_displacement = 0;
	int cap = 0;

	char arrow[] = {0xE6, 0x9A, 0};


	// Get actual configuration
	config = (EI_config*)EI_manage_config(EI_GET_ALL, 0);

	// Allocate RAM for string
	string = (char*)malloc(sizeof(char) * string_length + 1);

	// Initialize string
	for(i = 0; i <= string_length; i++) string[i] = 0;

	// Setup cursor flash
	switch(config->align)
	{
		default:
		case EI_ALIGN_LEFT:
			EI_Cursor_SetPosition(config->column, config->row);
			break;

		case EI_ALIGN_CENTER:
			EI_Cursor_SetPosition(config->column + (config->box_length >> 1), config->row);
			break;

		case EI_ALIGN_RIGHT:
			EI_Cursor_SetPosition(config->column + config->box_length - 1, config->row);
			break;
	}

	if(config->start_mode == EI_ALPHA_LOCKED)
	{
		EI_KBD_PutKey(KEY_CTRL_SHIFT, 0);
		GetKey(&key);
	}

	if(config->start_mode >= EI_ALPHA)
	{
		EI_KBD_PutKey(KEY_CTRL_ALPHA, 0);
		GetKey(&key);
		EI_Cursor_SetFlashOn(3);
	}
	else
	{
		EI_Cursor_SetFlashOn(0);
	}


	// Main loop
	i = 0;
	while(key != KEY_CTRL_EXE && key != KEY_CTRL_AC)
	{
		EI_Cursor_GetSettings(&cursor_settings);
		
		if(cursor_settings.flashstyle == 3 && !cap)
			EI_Cursor_SetFlashOn(4);

		GetKey(&key); // Getting key pressed

		switch(key) // Analyse key pressed
		{
			case KEY_CTRL_F1:
				cap = !cap;
				break;

			case KEY_CTRL_DEL:
				if(i > 0)
				{
					i--;
					string[i] = 0;
					char_deleted = 1;
				}
				break;
				
			default:
				// Give correct value to keys with a non-ascii return values
				if(key == KEY_CHAR_PLUS) key = '+';
				if(key == KEY_CHAR_MINUS || key == KEY_CHAR_PMINUS) key = '-';
				if(key == KEY_CHAR_MULT) key = '*';
				if(key == KEY_CHAR_DIV) key = '/';
				if(key == KEY_CHAR_ANS) key = '_';

				if(key == KEY_CTRL_XTT) key = 'A';
				if(key == KEY_CHAR_LOG) key = 'B';
				if(key == KEY_CHAR_LN) key = 'C';
				if(key == KEY_CHAR_SIN) key = 'D';
				if(key == KEY_CHAR_COS) key = 'E';
				if(key == KEY_CHAR_TAN) key = 'F';
				
				if(key > 127) // KEY_CTRL_***, etc.
					break;

				if(key >= 'A' && key <= 'Z') key += !cap * 32; // Switch between caps

				if(EI_check_char(key, chars_allowed) && EI_str_length(string) != string_length) // Add char if we can
				{
					string[i] = key;
					i++;
				}
		}

		switch(config->align) // Print string
		{
			default:
			case EI_ALIGN_LEFT:
				if(EI_str_length(string) < config->box_length)
				{
					EI_Cursor_SetPosition(config->column, config->row);
					Print((unsigned char*)string);
					if(char_deleted && EI_str_length(string) != config->box_length - 1)
					{
						Print((unsigned char*)" ");
						EI_Cursor_SetPosition(cursor_settings.column - 2, config->row);
						char_deleted = 0;
					}
				}
				else
				{
					EI_Cursor_SetPosition(config->column, config->row);
					Print((unsigned char*)arrow);
					Print((unsigned char*)(string + EI_str_length(string) - config->box_length + 2));
				}
				break;

			case EI_ALIGN_CENTER:
				if(EI_str_length(string) < config->box_length)
				{
					if(char_deleted && EI_str_length(string) != config->box_length - 1 && (config->box_length - EI_str_length(string)) >> 1 != ((config->box_length - EI_str_length(string) - 1) >> 1))
					{
						EI_Cursor_SetPosition(config->column + ((config->box_length - EI_str_length(string)) >> 1) - 1, config->row);
						Print((unsigned char*)" ");
						Print((unsigned char*)string);
						char_deleted = 0;
					}
					else if(char_deleted && EI_str_length(string) != config->box_length - 1 && (config->box_length - EI_str_length(string)) >> 1 == ((config->box_length - EI_str_length(string) - 1) >> 1))
					{
						EI_Cursor_SetPosition(config->column + ((config->box_length - EI_str_length(string)) >> 1), config->row);
						Print((unsigned char*)string);
						Print((unsigned char*)" ");
						EI_Cursor_SetPosition(cursor_settings.column - 2, config->row);
						char_deleted = 0;
					}
					else
					{
						EI_Cursor_SetPosition(config->column + ((config->box_length - EI_str_length(string)) >> 1), config->row);
						Print((unsigned char*)string);
					}
				}
				else
				{
					EI_Cursor_SetPosition(config->column, config->row);
					Print((unsigned char*)arrow);
					Print((unsigned char*)(string + EI_str_length(string) - config->box_length + 2));
				}
				break;

			case EI_ALIGN_RIGHT:
				if(EI_str_length(string) < config->box_length)
				{
					EI_Cursor_SetPosition(config->column + config->box_length - EI_str_length(string) - (char_deleted && EI_str_length(string) != config->box_length - 1) - 1, config->row);
					if(char_deleted && EI_str_length(string) != config->box_length - 1)
					{
						Print((unsigned char*)" ");
						char_deleted = 0;
					}
					Print((unsigned char*)string);
				}
				else
				{
					EI_Cursor_SetPosition(config->column, config->row);
					Print((unsigned char*)arrow);
					Print((unsigned char*)(string + EI_str_length(string) - config->box_length + 2));
				}
				break;
		}
	}

	// Set off cursor mode
	EI_Cursor_SetFlashOff();


	// Return NULL in case of an input break
	if(key == KEY_CTRL_AC)
	{
		free(string);
		return (char*)(0);
	}


	// Return the string
	return string;
}

const void *EI_manage_config(int parameter, int value)
{
	static EI_config config;

	if(value)
	{
		switch(parameter)
		{
			case EI_SET_COLUMN: config.column = value - 1;
				break;
			case EI_SET_ROW: config.row = value - 1;
				break;
			case EI_SET_BOX_LENGTH: config.box_length = (value < 3 ? 3 : value); // Minimal size is 3
				break;
			case EI_SET_ALIGN: config.align = value;
				break;
			case EI_SET_START_MODE: config.start_mode = value;
				break;
			case EI_SET_Aa_KEY: config.Aa_key = value;
				break;
		}
	}
	else
	{
		switch(parameter)
		{
			case EI_GET_ALL: return (const void*)(&config);
			case EI_SET_COLUMN: return (const void*)&config.column;
			case EI_SET_ROW: return (const void*)&config.row;
			case EI_SET_BOX_LENGTH: return (const void*)&config.box_length;
			case EI_SET_ALIGN: return (const void*)&config.align;
			case EI_SET_START_MODE: return (const void*)&config.start_mode;
			case EI_SET_Aa_KEY: return (const void*)&config.Aa_key;
		}
	}

	return (void*)(0);
}

void EI_init(void)
{
	EI_manage_config(EI_SET_COLUMN, 1);
	EI_manage_config(EI_SET_ROW, 1);
	EI_manage_config(EI_SET_ALIGN, EI_ALIGN_LEFT);
	EI_manage_config(EI_SET_START_MODE, EI_ALPHA_LOCKED);
	EI_manage_config(EI_SET_BOX_LENGTH, 21);
	EI_manage_config(EI_SET_Aa_KEY, KEY_CTRL_F1);
}

int EI_str_length(const char *str)
{
	int i = 0;

	while(str[i]) i++;
	
	return i;
}

int EI_check_char(char c, const char *chars_allowed)
{
	int i = 0;

	while(chars_allowed[i])
	{
		if(c == chars_allowed[i])
			return 1;
		i++;
	}

	return 0;
}