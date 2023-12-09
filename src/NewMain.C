#include "CoreFunctions.h"

#include "BoundingBox.h"
#include "ParticleMsg.h"
#include "Reader.h"
#include "ThreadStateHolder.h"
#include "Subtree.h"
#include "Partition.h"
#include "Configuration.h"

#include "ProxyPackSeparate.h"

#include "paratreet.decl.h"
#include "Paratreet.h"
#include "NewMain.h"


void NewMain::start(StartMessage* m)
{
    //auto& main = CsvAccess(main_);
    auto& main = main_;
    if(m->useInputFile)
    {
        main->setDefaults();
        (main->configuration()).parse(m->argc, m->argv);
        CkArgMsg* mm = new CkArgMsg(); 
        mm->argc = m->argc;
        mm->argv = m->argv;
        main->main(mm);

        CkCallback runCB(CkIndex_NewMain::run(), thisProxy);
        main->initializeDriver(runCB);
    }
    else
    {
        main->setDefaults();
        (main->configuration()).parse(m->argc, m->argv);
        CkArgMsg* mm = new CkArgMsg(); 
        mm->argc = m->argc;
        mm->argv = m->argv;
        main->main(mm);

        CkCallback runCB(CkIndex_NewMain::run(), thisProxy);
        main->initializeDriver(runCB);
    }
    
}

void NewMain::run() {
    main_->run();
}





void NewMain::setConfiguration(std::shared_ptr<paratreet::Configuration>&& cfg) {
    main_->setConfiguration(std::move(cfg));
}

void NewMain::getConfiguration(CkCallback cb)
{


    auto returnPointer = &(main_->configuration());

    cb.send(CkPointer<paratreet::Configuration>(returnPointer));
}
void NewMain::getTimestep(BoundingBox box, Real real, CkCallback cb)
{
    Real time_step = main_->getTimestep(box, real);
    //this->contribute(sizeof(Real), &time_step, CkReduction::nop, cb);
    cb.send(&time_step);
}


