#ifndef ILOGGER_H
#define ILOGGER_H

#include "DefinitionsAndIncludes.h"

#define LOG_FILENAME L"log.txt"

class ILogger
{
	bool			logEnabled; //�������� �� ����������������
	bool			logSuccessful; // ��������������� �������� ��������
	bool			logErrors; // ��������������� ������
	bool			fileOpenSuccessful; // ���� ������� ������
	bool			fileOpenAttempt; // ������� ������� ����
	bool			supressFileOutput; // ��������� ����� � ��������� ����
	bool			supressDialogOutput; // ��������� ����� � ���������� ����
	FILE			*outputFile;
	wchar_t			buffer[512]; //������ ��� �������� ��������������� ������
	wchar_t			tbuffer[64];
	time_t			rawTime;
	struct tm		timeInfo;
	int				iteratorI;
	int				iteratorJ;
	friend void		LOG(wchar_t *format, ...);
	friend void		eLOG(wchar_t *format, ...);
public:
	void			EnableLog(bool enable = true) { this->logEnabled = enable; }
	void			LogSucc(bool enable = true) { this->logSuccessful = enable; }
	void			LogErrors(bool enable = true) { this->logErrors = enable; }
	void			SuppresFileOutput(bool enable = true){ this->supressFileOutput = enable; }
	void			SuppessDialogOutput(bool enable = true) { this->supressDialogOutput = enable; }
	ILogger();
	~ILogger();
};

extern ILogger logger;

void LOG(wchar_t *format, ...);
void eLOG(wchar_t *format, ...);

#endif