#ifndef _PID_H_
#define _PID_H_

//Constants used in some of the functions below
#define AUTOMATIC   1
#define MANUAL      0
#define DIRECT      0
#define REVERSE     1

//commonly used functions **************************************************************************
typedef struct PID
{
    float*      input;
    float*      output;
    float*      setpoint;
    float*      gain_p;
    float*      gain_i;
    float*      gain_d;
    uint8_t     direction;
} PID;

// * sets PID to either Manual (0) or Auto (non-0)
void pid_setMode(uint8_t mode);

// * performs the PID calculation.  it should be
//   called every time loop() cycles. ON/OFF and
//   calculation frequency can be set using SetMode
//   SetSampleTime respectively
uint8_t pid_compute(void);

// Clamps the output to a specific range, 0-255 by default
void pid_setOutputLimits(uint8_t min, uint8_t max);


//available but not commonly used functions ************************************

// * While most users will set the tunings once in the
//   constructor, this function gives the user the option
//   of changing tunings during runtime for Adaptive control
void SetTunings(float newGain_p, float newGain_i, float newGain_d);

// * Sets the Direction, or "Action" of the controller. DIRECT
//   means the output will increase when error is positive. REVERSE
//   means the opposite.  it's very unlikely that this will be needed
//   once it is set in the constructor.
void SetControllerDirection(int);

// * sets the frequency, in Milliseconds, with which
//   the PID calculation is performed.  default is 100
void SetSampleTime(int);

//Display functions ************************************************************
// These functions query the pid for interal values.
// They were created mainly for the pid front-end,
// where it's important to know what is actually
// inside the PID.
float GetKp();
float GetKi();
float GetKd();
uint8_t GetMode();
uint8_t GetDirection();

void pid_initialize();
double _pid_gain_p;                  // * (P)roportional Tuning Parameter
double _pid_gain_i;                  // * (I)ntegral Tuning Parameter
double _pid_gain_d;                  // * (D)erivative Tuning Parameter

uint8_t pid_controllerDirection;

    double *myInput;              // * Pointers to the Input, Output, and Setpoint variables
    double *myOutput;             //   This creates a hard link between the variables and the
    double *mySetpoint;           //   PID, freeing the user from having to constantly tell us
                                  //   what these values are.  with pointers we'll just know.

	unsigned long lastTime;
	double ITerm, lastInput;

	unsigned long SampleTime;
	double outMin, outMax;
	bool inAuto;
};
#endif
