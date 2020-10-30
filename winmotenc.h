#ifndef __VITAL_H
#define __VITAL_H


// Number of axes in system 
#define VITAL_MAX_AXIS 8

// flags defined bit-exclusive for OR'ing if board can do multiple ways
#define EXT_ENCODER_INDEX_MODEL_MANUAL 0x00000001
#define EXT_ENCODER_INDEX_MODEL_AUTO   0x00000002

long __stdcall vitalInit(void);
long __stdcall vitalAioReset(void);	//set all DACs ro zero
long __stdcall vitalQuit(void);

long __stdcall vitalGetMaxEncoders(void);
long __stdcall vitalEncoderRead(int axis, long * pCounts);
long __stdcall vitalResetCounter( int nAxis );

long __stdcall vitalReadIndexLevel(int nAxis, int * flag);
long __stdcall vitalEncoderIndexModel(void);
long __stdcall vitalEncoderSetIndexModel(unsigned int model);
long __stdcall vitalEncoderResetIndex(int nAxis);
long __stdcall vitalEncoderReadLatch(int nAxis, int * flag);

long __stdcall vitalDacNum(void);
long __stdcall vitalDacWrite(int nAxis, double volts);
long __stdcall vitalDacWriteAll(int max, double * volts);

long __stdcall vitalReadAnalogInputs(unsigned long nBank, double *value);

long __stdcall vitalDioWrite(int index, int value);
long __stdcall vitalDioReset(void); // Turn off all outputs (open collector)
long __stdcall vitalDioCheck(int index, int *value);
long __stdcall vitalDioRead(int index, int *value);

// Read or Write Direct 32bit Registers on the MOTENC Board
long __stdcall vitalReadReg(int nReg, long * pRegData);
long __stdcall vitalWriteReg(int nReg, long RegData);



#endif /* __VITAL_H */

