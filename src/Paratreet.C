#include "Paratreet.h"

#include "Driver.h"
#include "Reader.h"
#include "Splitter.h"
#include "Subtree.h"
#include "Partition.h"
#include "TreeCanopy.h"
#include "BoundingBox.h"
#include "BufferedVec.h"
#include "Utility.h"
#include "CacheManager.h"
#include "Resumer.h"

/* readonly  CProxy_Reader readers;*/
/* readonly  CProxy_TreeSpec treespec;*/
/* readonly  CProxy_ThreadStateHolder thread_state_holder;*/
/* readonly  int n_readers;*/

using namespace paratreet;

MainChare::MainChare(CkArgMsg* m)
{
    CProxy_NewMain firstmain = CProxy_NewMain::ckNew();
    StartMessage* mm = new StartMessage(m->argc, m->argv,true);
    firstmain.start(mm);
}

NewMain::NewMain() {
    /*auto& main = CsvAccess(main_);

    main->setDefaults();
    (main->configuration()).parse(argc, argv);
    CkArgMsg* mm = new CkArgMsg(); 
    mm->argc = argc;
    mm->argv = argv;
    main->main(mm);

    CkCallback runCB(CkIndex_NewMain::run(), thisProxy);
    main->initializeDriver(runCB);*/
}

void NewMain::start(StartMessage* m)
{
    auto& main = CsvAccess(main_);
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
    CsvAccess(main_)->run();
}

namespace paratreet {

    CsvDeclare(registration_list_type_, registration_list_);
    CsvDeclare(main_type_, main_);

    std::intptr_t __addRegistrationFn(const registration_fn_& fn, const char* name) {
        auto& list = *(&CsvAccess(registration_list_));
        list = new registration_node_(list, fn, name);
        return reinterpret_cast<intptr_t>(list);
    }
}

#include "paratreet.def.h"
