/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "gui/simulator_gui.h"

#if defined(_MSC_VER)
	#include "windows.h"
#endif

namespace FastSimDesign {
	Q_DECLARE_LOGGING_CATEGORY(World)
	Q_LOGGING_CATEGORY(World, "World")

	QString currentTime()
	{
		// return ExpTool::World::GetTotalTime().toString("hh:mm:ss''zzz");
		return "";
	}

	QString msgTypeToString(QtMsgType const& oType)
	{
		switch (oType)
		{
			case QtDebugMsg:
				return QLatin1String{"Debug"};
			case QtInfoMsg:
				return QLatin1String{"Info"};
			case QtWarningMsg:
				return QLatin1String{"Warning"};
			case QtCriticalMsg:
				return QLatin1String{"Critical"};
			case QtFatalMsg:
				return QLatin1String{"Fatal"};
			default:
				return QLatin1String{""};
		}
	}

	QString formatFunctionContext(char const* oFunction)
	{
		QRegExp expr{"__cdecl (ExpTool::)?([^:<]*)"};
		expr.setMinimal(false);
		int pos = expr.indexIn(oFunction);
		if (pos == -1)
			return QLatin1String{""};
		QString className = expr.cap(2);

		QRegExp exprSplitWord{"(.)([A-Z])"};
		exprSplitWord.setMinimal(false);
		pos = exprSplitWord.indexIn(className);
		Q_ASSERT_X(exprSplitWord.capturedTexts().size() > 0, "", "Cannot build a custom function context");
		return QLatin1String{"ExpTool::"} % className.replace(exprSplitWord, "\\1-\\2");
	}

	QString formatLog(QtMsgType oType, QMessageLogContext const& oContext, QString const& sMsg)
	{
		// Time|MsgType|Method|Category|Message
		return QString{"%1|%2|%3|%5"}
			.arg(currentTime())
			.arg(msgTypeToString(oType), -10)
			.arg(formatFunctionContext(oContext.function), -45)
			//.arg(oContext.category, -16)
			.arg(sMsg);
	}

	void messageHandler(QtMsgType oType, QMessageLogContext const& oContext, QString const& sMsg)
	{
#if defined(_MSC_VER) && defined(_DEBUG)
		if (IsDebuggerPresent())
		{
			OutputDebugStringA(FormatLog(oType, oContext, sMsg).toUtf8().constData());
			OutputDebugStringA("\r\n");
		} else
		{
			fprintf(stderr, "%s\n", FormatLog(oType, oContext, sMsg).toUtf8().constData());
		}
#else
		fprintf(stderr, "%s\n", formatLog(oType, oContext, sMsg).toUtf8().constData());
#endif
	}
}

int main(int argc, char* argv[])
{
	// --- Configure the logging ---
	qInstallMessageHandler(FastSimDesign::messageHandler);

	// --- Start the gui ---
	QApplication a{argc, argv};
	FastSimDesign::SimulatorGui mainWindow{};
	mainWindow.show();
	return a.exec();
}