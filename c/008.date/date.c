#include <string.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char **argv) {
	struct tm t_tm;
	time_t t_time;

	if (argc == 1) {
		puts("AA");
		memset((void *)&t_tm , 0, sizeof(t_tm));
		puts("BB");
		time(&t_time);

		puts("CC");
		printf("get %s", ctime(&t_time));		
	} else if (strcmp(argv[1], "-s") == 0) {
		int year, mon, day, hour, min;

		if (sscanf(argv[2], "%d-%d-%dT%d:%d",
				   &year, &mon, &day, &hour, &min) != 5) {
			printf("argument for set error\n");
			return -1;
		}

		memset((void *)&t_tm , 0, sizeof(t_tm));
		t_tm.tm_year = year - 1900; /* year from 1900 */
		t_tm.tm_mon = mon - 1; /* 0-11 */
		t_tm.tm_mday = day; /* 1-31 */
		t_tm.tm_hour = hour;/* 0-23 */
		t_tm.tm_min = min ;/* 0-59 */
		t_tm.tm_sec = 0;

		t_time = mktime(&t_tm);

		if (t_time == -1) {
			printf("mktime error\n");
			return -1;
		}

		printf("set %s", ctime(&t_time));

		stime(&t_time);
	} else {
		printf("set date & time : %s -s [yyyy]-[mm]-[dd]T[hh]:[dd]\n", argv[0]);
		printf("get             : %s\n", argv[0]);
	}

	return 0;
}
