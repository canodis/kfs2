#ifndef TERMINAL_COMMANDS_H
# define TERMINAL_COMMANDS_H

#include "types.h"

void parse_command(uint8_t* Command);
void ClearCommand(char *argument);
void ColorCommand(char *argument);
void HelpCommand(char *argument);
void ReadStackCommand(char *argument);
void WriteStackCommand(char *argument);
void RebootCommand(char *argument);
void HaltCommand(char *argument);

#endif