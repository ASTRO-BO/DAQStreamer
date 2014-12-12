#include "DebugWorker.h"

using namespace CTADataModel;

namespace RTA
{

DebugWorker::DebugWorker()
{

}

std::unique_ptr<Message> DebugWorker::doWork(std::unique_ptr<CTADataModel::CameraEvent> event)
{
//	event->PrintDebugString();
	std::cout << "a" << std::endl;
    return nullptr;
}

};
