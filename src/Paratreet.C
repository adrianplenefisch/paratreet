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
#include "NewMain.h"

/* readonly  CProxy_Reader readers;*/
/* readonly  CProxy_TreeSpec treespec;*/
/* readonly  CProxy_ThreadStateHolder thread_state_holder;*/
/* readonly  int n_readers;*/

using namespace paratreet;

MainChare::MainChare(CkArgMsg* m)
{
    StartMessage* mm = new StartMessage(m->argc, m->argv,true);
    CProxy_NewMain firstmain = CProxy_NewMain::ckNew(mm);
    
    
}





namespace paratreet {

    CsvDeclare(registration_list_type_, registration_list_);
    //CsvDeclare(main_type_, main_);

    //main_type_ main_;

    std::intptr_t __addRegistrationFn(const registration_fn_& fn, const char* name) {
        auto& list = *(&CsvAccess(registration_list_));
        list = new registration_node_(list, fn, name);
        return reinterpret_cast<intptr_t>(list);
    }
}

#include "paratreet.def.h"

