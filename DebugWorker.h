/*
 *  DebugWorker.h
 *
 *  Created on Nov 26, 2014
 *  Author: zoli
 */
#ifndef DEBUG_WORKER_H_
#define DEBUG_WORKER_H_

//Be able to become an official worker !
#include "Worker.h"

//Be able to use the CameraEvent class
#include "L0.pb.h"

//Use the basic definitions from CTATools
#include "BasicDefs.hpp"

namespace RTA
{

/**
 *  @class DebugWorker
 *  @brief A dummy worker class, that simply takes cameraEvents as an input and forwards them downstream as they are
 *
 *  If you want to be used in a processingNode, you MUST derive from worker.
 *  There are then two functions that must be implemented:
 *  - static string const workerName() so that your worker has a nice shiny name displayed in the Gui.
 *  If you do not implement it, the name will simply be "worker"
 *  - unique_ptr<Message> doWork(unique_ptr<YourType>): as many function as you want to handle of data types.
 *  If you want, you can take unique_ptr<Message> as input and figure out the message type by yourself.
 *
 */
class DebugWorker : public CTATools::Core::Worker
{
    public:
        ///@brief default constructor. Does nothing
        DebugWorker();

        ///@brief returns the typename of this worker
        static string const workerName() { return "DebugWorker";}

        /**
         *  @brief the actual working function.
         *
         *  You can create as many as you want if more than one type of data must be handled by your worker
         *
         *  @return the product of the processing. In this case, also a CameraEvent (see DebugWorker2 to have
         *  a different return type.)
         */
        std::unique_ptr<Message> doWork(std::unique_ptr<CTADataModel::CameraEvent> event);
};

}; //namespace Tutorials
#endif
