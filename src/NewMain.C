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

template<typename T>
void NewMain::preTraversalFn(ProxyPack<T> pack) {
    static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
    main_->ErasedPreTraversalFn();
}

template<typename T>
void NewMain::traversalFn(BoundingBox box, ProxyPack<T> pack, int iter) {
    static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
    main_->ErasedTraversalFn(box, pack, iter);
}

template<typename T>
void NewMain::postIterationFn(BoundingBox box, ProxyPack<T> pack, int iter) {
    static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
    main_->ErasedPostIterationFn(box, pack, iter);
}

template<typename T>
void NewMain::perLeafFn(int indicator, SpatialNode<T> node, CProxy_Partition<T> partition) {
    //main_->perLeafFn(indicator, node, partition);
}

void NewMain::setConfiguration(std::shared_ptr<paratreet::Configuration>&& cfg) {
    main_->setConfiguration(std::move(cfg));
}

void NewMain::getConfiguration(CkCallback cb)
{
    this->contribute(sizeof(main_->configuration()), &(main_->configuration()), CkReduction::nop, cb);
}
void NewMain::getTimestep(BoundingBox box, Real real, CkCallback cb)
{
    Real time_step = main_->getTimestep(box, real);
    this->contribute(sizeof(Real), &time_step, CkReduction::nop, cb);
}


