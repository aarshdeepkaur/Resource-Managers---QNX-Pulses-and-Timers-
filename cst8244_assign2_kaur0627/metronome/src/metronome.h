#ifndef SRC_METRONOME_H_
#define SRC_METRONOME_H_

struct ioattr_t;
#define IOFUNC_ATTR_T struct ioattr_t
struct metro_ocb;
#define IOFUNC_OCB_T struct metro_ocb

#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <sys/types.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL
#define PAUSE_PULSE_CODE (_PULSE_CODE_MINAVAIL +1)
#define START_PULSE_CODE (_PULSE_CODE_MINAVAIL +2)
#define STOP_PULSE_CODE  (_PULSE_CODE_MINAVAIL +3)
#define QUIT_PULSE_CODE  (_PULSE_CODE_MINAVAIL +4)
#define SET_PULSE_CODE   (_PULSE_CODE_MINAVAIL +5)

#define START 0
#define STOPPED 1
#define PAUSED 2
#define ERROR -1


typedef union
{
	struct _pulse pulse;
	char msg[255];
} my_message_t;


#define NUM_DEVICES 2
#define METRONOME_DEVICE 0
#define METRONOME_HELP_DEVICE 1

char* devices[NUM_DEVICES] = { "/dev/local/metronome",
		"/dev/local/metronome-help" };


typedef struct ioattr_t
{
	iofunc_attr_t attr;
	int device;
} ioattr_t;



typedef struct metro_ocb
{
	iofunc_ocb_t ocb;
	char buffer[50];
} Metronome_ocb_t;



struct DataTableRow
{
	int tst;
	int tsb;
	int intervals;
	char pattern[20];
};
struct DataTableRow t[] = { { 2, 4, 4, "|1&2&" }, { 3, 4, 6, "|1&2&3&" }, { 4,
		4, 8, "|1&2&3&4&" }, { 5, 4, 10, "|1&2&3&4-5-" },
		{ 3, 8, 6, "|1-2-3-" }, { 6, 8, 6, "|1&a2&a" },
		{ 9, 8, 9, "|1&a2&a3&a" }, { 12, 8, 12, "|1&a2&a3&a4&a" } };


struct Metronome_Properties
{
	int beats_per_minute;
	int time_signature_top;
	int time_signature_bottom;
}typedef metronome_properties;


struct Timer_Properties
{
	double beats_per_second;
	double line_interval;
	double spacing_interval;
	double nano_seconds;
}typedef timer_properties;



struct Metronome
{
	metronome_properties metronome_properties_t;
	timer_properties timer_properties_t;
}typedef metronome_t;


int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
int io_read(resmgr_context_t *ctp, io_read_t *msg, Metronome_ocb_t *mocb);

Metronome_ocb_t * metro_ocb_calloc(resmgr_context_t *ctp, ioattr_t *mtattr);

void metronome_ocb_free(Metronome_ocb_t *ocb);
void stop_timer(struct itimerspec * itime, timer_t timer_id);
void start_timer(struct itimerspec * itime, timer_t timer_id, metronome_t* Metronome);
void set_timer_properties(metronome_t* Metronome, int interval);

int get_intervals_value(metronome_t* Metronome);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);

#endif
