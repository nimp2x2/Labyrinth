#include "ILogger.h"

ILogger logger;

ILogger::ILogger()
{
	this->logEnabled = true; 
	this->logSuccessful = true;
	this->logErrors = true; 
	this->fileOpenSuccessful = false;
	this->fileOpenAttempt = false;
	this->supressFileOutput = false;
	this->supressDialogOutput = false; 
}

ILogger::~ILogger()
{
	if(this->fileOpenSuccessful)
		fclose(this->outputFile);
}

void LOG(wchar_t *format, ...)
{	
	if(!logger.logEnabled || (!logger.fileOpenSuccessful && logger.fileOpenAttempt))
		return;
	if(!logger.logSuccessful)
		return;
	if(!logger.fileOpenAttempt)
	{
		if(_wfopen_s(&logger.outputFile,L"log.txt",L"w") != NULL)
			logger.fileOpenSuccessful = false;
		else
		{
			logger.fileOpenSuccessful = true;
		}
		logger.fileOpenAttempt = true;
	}

	logger.iteratorI = 0;
	logger.iteratorJ = 0;
	logger.rawTime = time ( NULL );
	localtime_s(&logger.timeInfo, &logger.rawTime );

	if(!logger.supressFileOutput)
	{
		fwprintf_s(logger.outputFile,L"[");
		_wasctime_s(logger.buffer,512,&logger.timeInfo);
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		int j = 0;
		while(logger.buffer[logger.iteratorI] != ' ')
		{
			logger.tbuffer[logger.iteratorJ] = logger.buffer[logger.iteratorI];
			logger.iteratorI++;
			logger.iteratorJ++;
		}
		logger.tbuffer[logger.iteratorJ] = 0;

		fwprintf_s(logger.outputFile,logger.tbuffer);
		fwprintf_s(logger.outputFile,L"] ");

		va_list args;
		va_start (args, format);
		vswprintf_s (logger.buffer,512,format, args);
		va_end (args);

		fwprintf_s(logger.outputFile,logger.buffer);
		fwprintf_s(logger.outputFile,L"\n");
	}
}

void eLOG(wchar_t *format, ...)
{
	if(!logger.logEnabled || !logger.fileOpenSuccessful)
		return;
	if(!logger.logErrors)
		return;
	if(!logger.fileOpenAttempt)
	{
		if(_wfopen_s(&logger.outputFile,L"log.txt",L"w") != NULL)
			logger.fileOpenSuccessful = false;
		else
			logger.fileOpenSuccessful = true;
		logger.fileOpenAttempt = true;
	}

	logger.iteratorI = 0;
	logger.iteratorJ = 0;
	logger.rawTime = time ( NULL );
	localtime_s(&logger.timeInfo, &logger.rawTime );

	if(!logger.supressFileOutput)
	{
		fwprintf_s(logger.outputFile,L"[");
		_wasctime_s(logger.buffer,512,&logger.timeInfo);
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		while(logger.buffer[logger.iteratorI] != ' ')
			logger.iteratorI++;
		logger.iteratorI++;
		int j = 0;
		while(logger.buffer[logger.iteratorI] != ' ')
		{
			logger.tbuffer[logger.iteratorJ] = logger.buffer[logger.iteratorI];
			logger.iteratorI++;
			logger.iteratorJ++;
		}
		logger.tbuffer[logger.iteratorJ] = 0;

		fwprintf_s(logger.outputFile,logger.tbuffer);
		fwprintf_s(logger.outputFile,L"] ");

		va_list args;
		va_start (args, format);
		vswprintf_s (logger.buffer,512,format, args);
		va_end (args);

		fwprintf_s(logger.outputFile,logger.buffer);
		fwprintf_s(logger.outputFile,L"\n");
	}
	if(!logger.supressFileOutput && !logger.supressDialogOutput)
	{
		MessageBox(NULL,logger.buffer,L"ERROR",MB_ICONERROR);
	}
	if(!logger.supressDialogOutput && logger.supressFileOutput)
	{
		va_list args;
		va_start (args, format);
		vswprintf_s (logger.buffer,512,format, args);
		va_end (args);

		fwprintf_s(logger.outputFile,logger.buffer);
		fwprintf_s(logger.outputFile,L"\n");
		MessageBox(NULL,logger.buffer,L"ERROR",MB_ICONERROR);
	}
}