#include "gtest/gtest.h"
#include <QtWidgets/QApplication>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type)
	{
		case QtDebugMsg:
			//fprintf(stderr, "Debug: %s\n", localMsg.constData());
			break;
		case QtInfoMsg:
			//fprintf(stderr, "Info: %s\n", localMsg.constData());
			break;
		case QtWarningMsg:
			//fprintf(stderr, "Warning: %s\n", localMsg.constData());
			break;
		case QtCriticalMsg:
			//fprintf(stderr, "Critical: %s\n", localMsg.constData());
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", localMsg.constData());
			abort();
	}
}

int main(int argc, char** argv)
{
	#ifdef _BEING_PLATFORM_WINDOWS_
	Being::SetAssertConfig(Being::APC_UNITTEST);
	Being::SetHandler(Being::MvscHandler);
	#endif
	qInstallMessageHandler(myMessageOutput);
	QApplication a(argc, argv);
	testing::InitGoogleTest(&argc, argv);
	// Get a checkpoint of the memory after Google Test has been initialized.
	//_CrtMemState memoryState = {0};
	//_CrtMemCheckpoint(&memoryState);
	int returnValue = 0;
	returnValue = RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke

	// Check for leaks after tests have run
	//_CrtMemDumpAllObjectsSince(&memoryState);
	return a.exec();
}