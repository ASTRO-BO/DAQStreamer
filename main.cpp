#include "CameraEventStreamer.h"
#include "DebugWorker.h"
#include "Processor.h"

int main(int argc, char* argv[])
{
	if(argc <= 2)
	{
		std::cout << "Wrong arguments, please provide the .xml or .stream and the raw input file." << std::endl;
		std::cout << "Example: ./packetToMessage rta_fadc_all.xml Aar.raw" << std::endl;
		return 1;
	}

	RTA::CameraEventStreamer streamer(argv[1], argv[2], 10000);
    streamer.addConnection(ZMQ_PUSH, "tcp://*:12345");
	streamer.start();
	streamer.join();

/*    CTATools::Core::Processor<RTA::DebugWorker> dbgProc("0", 1);
	CTATools::Core::Streamer::connect(streamer, dbgProc);

	streamer.start();
	dbgProc.start();

	streamer.join();
	dbgProc.join();*/

	return 0;
}
