/*
 * metrics_collector.h
 *
 *  Created on: Jan 30, 2017
 *      Author: tang526
 */

#ifndef _METRICS_COLLECTOR_H_
#define _METRICS_COLLECTOR_H_

#include "tape.h"
#include "json/json.h" //jsoncpp library
#include "../powerflow/link.h"


EXPORT void new_metrics_collector(MODULE *);

#ifdef __cplusplus

// struct containing two return values for voltage violation results
typedef struct vol_violation {
	double durationViolation; 		     //integrator state variable
	int countViolation;	 //angle measurement
} VIOLATION_RETURN;

class metrics_collector{
public:
	static metrics_collector *defaults;
	static CLASS *oclass, *pclass;

	metrics_collector(MODULE *);
	int create();
	int init(OBJECT *);
	int isa(char *);
	TIMESTAMP postsync(TIMESTAMP, TIMESTAMP);

	int commit(TIMESTAMP);

public:
	double interval_length_dbl;			//Metrics output interval length
	TIMESTAMP next_time;
	TIMESTAMP dt;
	TIMESTAMP last_t;

	friend class metrics_collector_writer;

private:

	int read_line(OBJECT *obj);
	int write_line(TIMESTAMP, OBJECT *obj);

	double findMax(double array[], int size);
	double findMin(double array[], int size);
	double findAverage(double array[], int size);
	double findMedian(double array[], int size);
	vol_violation findOutLimit(double array[], bool checkAbove, double limitVal, int size);

private:
	FILE *rec_file;
	FINDLIST *items;
	PROPERTY *prop_ptr;
	int obj_count;
	int write_count;
	TIMESTAMP next_write;
	TIMESTAMP last_write;
	TIMESTAMP last_flush;
	TIMESTAMP write_interval;
	TIMESTAMP flush_interval;
	int32 write_ct;
	TAPESTATUS tape_status; // TS_INIT/OPEN/DONE/ERROR
	char *prev_line_buffer;
	char *line_buffer;
	size_t line_size;
	bool interval_write;

	char* parent_string;

	Json::Value metrics_Output;

	// Parameters related to triplex_meter object
	double *real_power_array;		//array storing real power measured at the triplex_meter
	double *reactive_power_array;		//array storing reactive power measured at the triplex_meter
	double *voltage_mag_array;		//array storing voltage12 measured at the triplex_meter
	double *voltage_average_mag_array;		//array storing voltage12/2 measured at the triplex_meter
	double *voltage_unbalance_array;		//array storing (voltage[0]-voltage[1])/(voltage12/2) measured at the triplex_meter
	double price_parent; 			// Price of thr triplex_meter

	// Parameters related to houe object
	double *total_load_array; 		//array storing total_load measured at the house
	double *hvac_load_array; 		//array storing hvac_load measured at the house
	double *air_temperature_array; 		//array storing air_temperature measured at the house

	// Parameters related to waterheater object
	double *actual_load_array; 		//array storing actual_load measured at the house
	char waterheaterName[64];				// char array storing names of the waterheater

	// Parameters related to inverter object
	// No new arrays defined for inverter object,
	// since real_power_array and reactive_power_array have been defined for triplex_meter already

	// Parameters related to Swing-bus meter object
	FINDLIST *link_objects;
	double *real_power_loss_array;		//array storing real power losses for the whole feeder
	double *reactive_power_loss_array;		//array storing real power losses for the whole feeder

	int interval_length;			//integer averaging length (seconds)

	TIMESTAMP perform_average_time;	//Timestamp to perform the average
	TIMESTAMP last_update_time;		//Last time array updated
	int curr_avg_pos_index;			//Index for current position of averaging array

};

#endif // C++

#endif // _METRICS_COLLECTOR_H_

// EOF