#ifndef	_DOLIXON_CLOCK_H_
#define	_DOLIXON_CLOCK_H_

struct Clock
{
	static void int_handler(int irq);
	static void init();
};

#endif
