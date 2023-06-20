#ifndef __ESCAPE_H__
#define __ESCAPE_H__


enum enum_ESCRETURN
{
	enum_ESCRETURN_exit,
	enum_ESCRETURN_continue,
	enum_ESCRETURN_restart,
	enum_ESCRETURN_ranks
};

int Call_Escape(void);

#endif // __ESCAPE_H__