#include <stdio.h>
#include <windows.h>
#include "reminder.h"

const char * info = {
	"�¼����� v1.2 \n"
	"���� Sun, Mon, Tues, Wed, Thu, Fir, Sat \n"
	"�·� Jan, Feb, Mar, Apl, May, Jnu, Jul, Aug, Sep, Oct, Nov, Dec \n"
};

int main(int argc, char * argv[])
{
	reminder_t reminder;

	printf("%s\n", info);

	reminder_create(&reminder);
	reminder_load_config(&reminder, "config.ini");
	reminder_load_remind(&reminder, "events.ini");
	
	reminder_exec(&reminder);
	
	return 0;
}