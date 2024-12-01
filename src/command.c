#include "inc/command.h"
#include "inc/kmalloc.h"
#include "inc/terminalCommands.h"

tCommand *gCommandList = NULL;

tCommand *CreateCommand(char *name, char *description, void (*function)(char *argument));
void PushBackCommand(tCommand *newCommand);

void InitAllCommands(void)
{
	PushBackCommand(CreateCommand("clear", "", &ClearCommand));
	PushBackCommand(CreateCommand("color", "[red, blue, lcyan...]", &ColorCommand));
	PushBackCommand(CreateCommand("help", "", &HelpCommand));
	PushBackCommand(CreateCommand("readstack", "[all, spesicif address]", &ReadStackCommand));
	PushBackCommand(CreateCommand("writestack", "[spesific address] [value]", &WriteStackCommand));
	PushBackCommand(CreateCommand("reboot", "", &RebootCommand));
	PushBackCommand(CreateCommand("halt", "(dont use !)", &HaltCommand));
}

tCommand *CreateCommand(char *name, char *description, void (*function)(char *argument))
{
	tCommand *newCommand = (tCommand *)kmalloc(sizeof(tCommand));
	newCommand->name = name;
	newCommand->description = description;
	newCommand->function = function;
	newCommand->next = NULL;
	return newCommand;
}

void PushBackCommand(tCommand *newCommand)
{
	tCommand *tmp = gCommandList;
	if (tmp == NULL)
	{
		gCommandList = newCommand;
		return;
	}
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = newCommand;
}