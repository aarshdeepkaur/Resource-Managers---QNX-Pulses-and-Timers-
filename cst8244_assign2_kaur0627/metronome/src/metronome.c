
#include "metronome.h"

metronome_t Metronome;
name_attach_t* attach;
my_message_t msg;
char data[255];
int server_coid;

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle,
		void *extra) {
	if ((server_coid = name_open("metronome", 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}

	return (iofunc_open_default(ctp, msg, handle, extra));

}

void metronome_thread()
{

	struct sigevent event;
	struct itimerspec itime;
	timer_t timer_id;
	int rcvid = 0;
	char *tp;

	int timer_status;
	int intervals_from_table = 0;
	if ((attach = name_attach(NULL, "metronome", 0)) == NULL)
	{
		printf("Error connecting to device\n");
		exit(EXIT_FAILURE);
	}

	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, attach->chid,
	_NTO_SIDE_CHANNEL, 0);

	event.sigev_priority = SIGEV_PULSE_PRIO_INHERIT;
	event.sigev_code = MY_PULSE_CODE;

	timer_create(CLOCK_REALTIME, &event, &timer_id);

	intervals_from_table = get_intervals_value(&Metronome);

	set_timer_properties(&Metronome, intervals_from_table);

	start_timer(&itime, timer_id, &Metronome);

	tp = t[intervals_from_table].pattern;

	for (;;)
	{
		if ((rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL)) == ERROR)
		{
			printf("Error - MessageReceive() - ./metronome\n");
			exit(EXIT_FAILURE);
		}


		if (rcvid == 0)
		{
			switch (msg.pulse.code)
			{
			case (MY_PULSE_CODE):
				if (*tp == '|')
				{
					printf("%.2s", tp);
					tp = tp + 2;
				} else if (*tp == '\0')

				{
					printf("\n");
					tp = t[intervals_from_table].pattern;
				} else

				{
					printf("%c", *tp++);
				}
				break;
			case (PAUSE_PULSE_CODE):

				if (timer_status == 0)
				{
					itime.it_value.tv_sec = msg.pulse.value.sival_int;
					timer_settime(timer_id, 0, &itime, NULL);
				}

				break;

			case QUIT_PULSE_CODE:
				timer_delete(timer_id);
				name_detach(attach, 0);
				name_close(server_coid);
				exit(EXIT_SUCCESS);
				break;

			case SET_PULSE_CODE:
				intervals_from_table = get_intervals_value(&Metronome);
				tp = t[intervals_from_table].pattern;
				set_timer_properties(&Metronome, intervals_from_table);
				start_timer(&itime, timer_id, &Metronome);
				printf("\n");
				break;

			case START_PULSE_CODE:
				if (timer_status == 1)
				{
					start_timer(&itime, timer_id, &Metronome);
					timer_status = START;
				}
				break;

			case STOP_PULSE_CODE:
				if (timer_status == 0 || timer_status == 2)
				{
					stop_timer(&itime, timer_id);
					timer_status = 1;
				}
				break;

			}
		}

		fflush(stdout);

	}

}

void stop_timer(struct itimerspec * itime, timer_t timer_id)
{
	itime->it_value.tv_sec = 0;
	timer_settime(timer_id, 0, itime, NULL);
}

//here we used to start the timer
void start_timer(struct itimerspec * itime, timer_t timer_id,
		metronome_t* Metronome)
{

	itime->it_value.tv_sec = 1;
	itime->it_value.tv_nsec = 0;
	itime->it_interval.tv_sec = Metronome->timer_properties_t.spacing_interval;
	itime->it_interval.tv_nsec = Metronome->timer_properties_t.nano_seconds;
	timer_settime(timer_id, 0, itime, NULL);

}

void set_timer_properties(metronome_t* Metronome, int interval)
{
	Metronome->timer_properties_t.beats_per_second = (double) 60
			/ Metronome->metronome_properties_t.beats_per_minute;
	Metronome->timer_properties_t.line_interval =
			Metronome->timer_properties_t.beats_per_second
					* Metronome->metronome_properties_t.time_signature_top;

	Metronome->timer_properties_t.spacing_interval =
			Metronome->timer_properties_t.line_interval / interval;
	Metronome->timer_properties_t.nano_seconds =
			Metronome->timer_properties_t.spacing_interval * 1e+9;

}

int get_intervals_value(metronome_t* Metronome)
{
	int intervals_from_table = 0;
	for (int i = 0; i < 8; i++)
	{
		if ((t[i].tst == Metronome->metronome_properties_t.time_signature_top)
				&& (t[i].tsb
						== Metronome->metronome_properties_t.time_signature_bottom))
		{
			intervals_from_table = t[i].intervals;

		}
	}

	return intervals_from_table;
}


int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	int nb = 0;

	if (ocb->ocb.attr->device == METRONOME_HELP_DEVICE)
	{
		printf("\nError: Cannot Write to device /dev/local/metronome-help\n");
		nb = msg->i.nbytes;
		_IO_SET_WRITE_NBYTES(ctp, nb);
		return (_RESMGR_NPARTS(0));
	}

	if (msg->i.nbytes == ctp->info.msglen - (ctp->offset + sizeof(*msg)))
	{

		char *buf;
		char *pause_msg;
		char *set_msg;
		int i, small_integer = 0;
		buf = (char *) (msg + 1);

		if (strstr(buf, "pause") != NULL)
		{
			for (i = 0; i < 2; i++)
			{
				pause_msg = strsep(&buf, " ");
			}
			small_integer = atoi(pause_msg);
			if (small_integer >= 1 && small_integer <= 9)
			{
				//FIXME :: replace getprio() with SchedGet()
				MsgSendPulse(server_coid, SchedGet(0, 0, NULL),
				PAUSE_PULSE_CODE, small_integer);
			} else
			{
				printf("Integer is not between 1 and 9.\n");
			}

		} else if (strstr(buf, "quit") != NULL)
		{
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL),
			QUIT_PULSE_CODE, small_integer);

		} else if (strstr(buf, "start") != NULL)
		{
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL),
			START_PULSE_CODE, small_integer);

		} else if (strstr(buf, "stop") != NULL)
		{
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), STOP_PULSE_CODE,
					small_integer);

		} else if (strstr(buf, "set") != NULL)
		{
			for (i = 0; i < 4; i++)
			{
				set_msg = strsep(&buf, " ");

				if (i == 1)
				{
					Metronome.metronome_properties_t.beats_per_minute = atoi(
							set_msg);
				} else if (i == 2)
				{
					Metronome.metronome_properties_t.time_signature_top = atoi(
							set_msg);
				} else if (i == 3)
				{
					Metronome.metronome_properties_t.time_signature_bottom =
							atoi(set_msg);
				}
			}

			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), SET_PULSE_CODE,
					small_integer);

		} else {
			strcpy(data, buf);
		}

		nb = msg->i.nbytes;
	}

	_IO_SET_WRITE_NBYTES(ctp, nb);

	if (msg->i.nbytes > 0)
		ocb->ocb.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS(0));
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, Metronome_ocb_t *mocb)
{
	int intervals_from_table = 0;
	int nb;
	if (data == NULL)
		return 0;

	if (mocb->ocb.attr->device == METRONOME_HELP_DEVICE)
	{
		sprintf(data,
				"Metronome Resource Manager (ResMgr) \n \n"
						" Usage: metronome <bpm> <ts-top> <ts-bottom>\n\n"
						" API:\n\n"
						"  pause [1-9]\t\t\t- pause the metronome for 1-9 seconds\n"
						"  quit\t\t\t- quit the metronome\n"
						"  set <bpm> <ts-top> <ts-bottom>\t\t\t- set the metronome to <bpm> ts-top/ts-bottom\n"
						"  start\t\t\t- start the metronome from stopped state\n"
						"  stop\t\t\t- stop the metronome; use ‘start’ to resume\n");
	}

	if (mocb->ocb.attr->device == METRONOME_HELP_DEVICE)
	{
		intervals_from_table = get_intervals_value(&Metronome);
		sprintf(data,
				"[metronome: %d beats/min, time signature %d/%d, sec-per-interval: %.2f, nanoSecs: %.0lf]\n",
				Metronome.metronome_properties_t.beats_per_minute,
				Metronome.metronome_properties_t.time_signature_top,
				Metronome.metronome_properties_t.time_signature_bottom,
				Metronome.timer_properties_t.spacing_interval,
				Metronome.timer_properties_t.nano_seconds);
	}
	nb = strlen(data);

	if (mocb->ocb.offset == nb)
		return 0;

	nb = min(nb, msg->i.nbytes);
	_IO_SET_READ_NBYTES(ctp, nb);
	SETIOV(ctp->iov, data, nb);
	mocb->ocb.offset += nb;

	if (nb > 0)
		mocb->ocb.flags |= IOFUNC_ATTR_ATIME;

	return (_RESMGR_NPARTS(1));
}


Metronome_ocb_t * metro_ocb_calloc(resmgr_context_t *ctp, ioattr_t *mtattr)
{
	Metronome_ocb_t *tocb;
	tocb = calloc(1, sizeof(Metronome_ocb_t));
	tocb->ocb.offset = 0;
	return (tocb);
}

void metronome_ocb_free(Metronome_ocb_t *ocb)
{
	free(ocb);
}

int main(int agrc, char* argv[])
{
	dispatch_t * dpp;
	resmgr_io_funcs_t io_funcs;
	resmgr_connect_funcs_t conn_funcs;
	ioattr_t ioattr[NUM_DEVICES];
	pthread_attr_t thread_attrib;
	dispatch_context_t * ctp;

	int id, i;
	int server_coid;

	if (agrc != 4) {

		printf("ERROR: You must provide all required arguments.\n");
		printf(
				"Command: ./metronome <beats/minute> <time-signature-top> <time-signature-bottom>\n");
		exit(EXIT_FAILURE);
	}

	iofunc_funcs_t metro_ocb_funcs = { _IOFUNC_NFUNCS, metro_ocb_calloc,
			metronome_ocb_free, };
	iofunc_mount_t metro_mount = { 0, 0, 0, 0, &metro_ocb_funcs };


	Metronome.metronome_properties_t.beats_per_minute = atoi(argv[1]);
	Metronome.metronome_properties_t.time_signature_top = atoi(argv[2]);
	Metronome.metronome_properties_t.time_signature_bottom = atoi(argv[3]);
	if ((dpp = dispatch_create()) == NULL)
	{
		fprintf(stderr, "%s:  Unable to allocate dispatch context.\n", argv[0]);
		return (EXIT_FAILURE);
	}

	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &conn_funcs, _RESMGR_IO_NFUNCS,
			&io_funcs);
	conn_funcs.open = io_open;
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	for (i = 0; i < NUM_DEVICES; i++)
	{
		iofunc_attr_init(&ioattr[i].attr, S_IFCHR | 0666, NULL, NULL);
		ioattr[i].device = i;
		ioattr[i].attr.mount = &metro_mount;

		if ((id = resmgr_attach(dpp, NULL, devices[i], _FTYPE_ANY, 0,
				&conn_funcs, &io_funcs, &ioattr[i])) == ERROR)
		{
			fprintf(stderr, "%s:  Unable to attach name.\n", argv[0]);
			return (EXIT_FAILURE);
		}
	}

	ctp = dispatch_context_alloc(dpp);
	pthread_attr_init(&thread_attrib);
	pthread_create(NULL, &thread_attrib, &metronome_thread, &Metronome);

	while (1)
	{
		if ((ctp = dispatch_block(ctp)))
		{
			dispatch_handler(ctp);
		} else
			printf("ERROR \n");

	}

	pthread_attr_destroy(&thread_attrib);
	name_detach(attach, 0);
	name_close(server_coid);
	return EXIT_SUCCESS;


	return EXIT_SUCCESS;
}

