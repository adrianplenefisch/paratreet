#ifndef NEW_MAIN_H
#define NEW_MAIN_H

#include "paratreet.decl.h"
#include "Configuration.h"
#include "Paratreet.h"


class NewMain: public CBase_NewMain {
  public:
    using main_type_ = std::unique_ptr<paratreet::MainBase>;
    main_type_ main_;
    
    NewMain(StartMessage* mm);
    void start(StartMessage* m);
    void run();

    template<typename T>
    void preTraversalFn(ProxyPack<T> pack) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedPreTraversalFn();
    }

    template<typename T>
    void traversalFn(BoundingBox box, ProxyPack<T> pack, int iter) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedTraversalFn(box, iter);
    }

    template<typename T>
    void postIterationFn(BoundingBox box, ProxyPack<T> pack, int iter) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedPostIterationFn(box, iter);
    }

    template<typename T>
    void perLeafFn(int indicator, SpatialNode<T> node, CProxy_Partition<T> partition) {
      //main_->perLeafFn(indicator, node, partition);
    }

    void setConfiguration(std::shared_ptr<paratreet::Configuration>&& cfg);

    void getConfiguration(CkCallback cb);

    void getTimestep(BoundingBox box, Real real, CkCallback cb);
};


#endif

#include "templates.h"

