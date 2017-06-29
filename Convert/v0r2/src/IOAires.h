#ifndef __IOAIRES_H
#define __IOAIRES_H

#include "PartGrnd_ROOT.h"
#include "HeaderAires_ROOT.h"
#include "RotToAugerFr.h"

enum RecordType {PARTICLERECORDTYPE=1,STARTOFSHOWERRECORDTYPE=2,ENDOFSHOWERRECORDTYPE=3,OTHERRECORDTYPE=4};

HeaderAires_ROOT *IOAiresOpen(const char filename[80]);

bool IOAiresSimpleOpen(const char filename[80]);

PartGrnd_ROOT *IOAiresNext();

int IOAiresNext(PartGrnd_ROOT *p);
int IOAiresShutdown();
void IOAiresSummary();

int ReadAiresBeginOfShower(HeaderAires_ROOT * sh);	// Reads the begin of shower (internal)
int ReadAiresEndOfShower(HeaderAires_ROOT * sh);	// Updates with the end of shower
int ReReadAiresBeginOfShower(HeaderAires_ROOT * sh);	// Reads the begin of shower (internal)
int ReReadAiresEndOfShower(HeaderAires_ROOT * sh);	// Updates with the end of shower

#endif
