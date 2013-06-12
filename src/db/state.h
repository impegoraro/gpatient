#ifndef _DBSTATE_H
#define _DBSTATE_H

namespace DB
{
	enum State
	{
		New,
		Modified,
		Commited,
		Unmodified
	};

};

#endif