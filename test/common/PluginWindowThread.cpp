#include "PluginWindowThread.hpp"

PluginWindowThread::PluginWindowThread(QObject* parent): window_()
{
	//
}

PluginWindowThread::~PluginWindowThread()
{
	//
}

void PluginWindowThread::run()
{
}

QWindow* PluginWindowThread::window()
{
	return &window_;
}

