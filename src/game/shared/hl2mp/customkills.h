#ifndef CUSTOMKILLS_H
#define CUSTOMKILLS_H

enum
{
	TELEFRAG = 1,
};
static const char *g_ppszCustomKills[] = 
{
	"custom_telefrag"
};
#define CUSTOMKILL_NONE 0
#define CUSTOMKILL_MAX ARRAYSIZE( g_ppszCustomKills )
#endif