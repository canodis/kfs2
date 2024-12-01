#ifndef COMMAND_H
# define COMMAND_H

#include "types.h"

typedef struct sCommand
{
	char *name;
	char *description;
	void (*function)(char *argument);
	struct sCommand *next;
}	tCommand;

extern tCommand *gCommandList;

void InitAllCommands(void);

#endif // !COMMAND_H