#ifndef NEW_MAIN_H
#define NEW_MAIN_H

#include "paratreet.decl.h"
#include "Configuration.h"
#include "Paratreet.h"
#include "ParticleMsg.h"


class NewMain: public CBase_NewMain {
  public:
    using main_type_ = std::unique_ptr<paratreet::MainBase>;
    main_type_ main_;
    int depth;
    int group_index;
    Particle* pm;
    int n_particles;

    
    NewMain(StartMessage* mm);

    template <typename Data>
    void passArrayProxies(CProxy_TreeCanopy<Data> tc,CProxy_Subtree<Data> st,
            CProxy_Partition<Data> p)
    {
        static_cast<paratreet::Main<Data>*>(main_.get())->p = p;
        static_cast<paratreet::Main<Data>*>(main_.get())->st = st;
        static_cast<paratreet::Main<Data>*>(main_.get())->tc = tc;
        CkCallback runCB(CkIndex_NewMain::run(), thisProxy);
        main_->initializeDriver(runCB, pm,n_particles);
    }

    void start(StartMessage* m);
    void run();

    template<typename T>
    void preTraversalFn(ProxyPack<T> pack, CkCallback cb) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedPreTraversalFn(cb);
    }

    template<typename T>
    void traversalFn(BoundingBox box, ProxyPack<T> pack, int iter, CkCallback cb) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedTraversalFn(box, iter,cb);
    }

    template<typename T>
    void postIterationFn(BoundingBox box, ProxyPack<T> pack, int iter, CkCallback cb) {
      static_cast<paratreet::Main<T>*>(main_.get())->pp = pack;
      main_->ErasedPostIterationFn(box, iter,cb);
    }

    template<typename T>
    void perLeafFn(int indicator, SpatialNode<T> node, CProxy_Partition<T> partition) {
      static_cast<paratreet::Main<T>*>(main_.get())->n = node;
      static_cast<paratreet::Main<T>*>(main_.get())->p = partition;
      main_->ErasedPerLeafFn(indicator);
    }

    void setConfiguration(std::shared_ptr<paratreet::Configuration>&& cfg);

    void getConfiguration(CkCallback cb);

    void getDepth(CkCallback cb);

    void getTimestep(BoundingBox box, Real real, CkCallback cb);
};


#endif

#include "templates.h"

