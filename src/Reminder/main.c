#include <stdio.h>
#include <windows.h>
#include "reminder.h"

const char * info = {
	"事件提醒 v2.1 \n"
	"星期 Sun, Mon, Tues, Wed, Thu, Fir, Sat \n"
	"月份 Jan, Feb, Mar, Apl, May, Jnu, Jul, Aug, Sep, Oct, Nov, Dec \n"
	"增加日期的处理，修改可能因为执行延迟造成事件丢失的问题\n"
};

int main(int argc, char * argv[])
{
	reminder_t * reminder;

	printf("%s\n", info);

	reminder = reminder_create();
	reminder_load_config(reminder, "config.ini");
	reminder_load_remind(reminder, "events.ini");
	
	reminder_exec(reminder);
	reminder_delete(reminder);

	return 0;
}
