#ifndef SIGN_PREDICTOR_H
#define SIGN_PREDICTOR_H

#include "ctmsnet.h"
#include <Snap.h>
#include "ml.h"

class TSignPredictor {
protected:	
	PCtmsNet network;	
public:
	TSignPredictor(const PCtmsNet& net) : network(net) {}
	virtual void build() = 0;
	virtual int predict(const TInt srcNId, const TInt desNId) = 0;
};

class TNaivePredictor : public TSignPredictor {
protected:
	const std::string pType;
public:
	TNaivePredictor(const PCtmsNet& net, const char* type) : TSignPredictor(net), pType(type) {}
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};

class TBalanceBasedPredictor : public TSignPredictor {
public:
	TBalanceBasedPredictor(const PCtmsNet& net) : TSignPredictor(net) {}
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};

class TStatusBasedPredictor : public TSignPredictor {
public:
	TStatusBasedPredictor(const PCtmsNet& net) : TSignPredictor(net) {}
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};

class TLogisticRegression : public TSignPredictor {
protected:
	//THashSet<TChA> features; //TChA is feature string. Its corresponding Int ID is hash KeyId (int KeyId -> TChA Feature String)
	TFltV theta;
	THash<TChA, TInt> fe2ix;
	THash<TInt, TChA> ix2fe;
	void extractFeatures(const TIntPr& edge, THash<TChA, TInt>& edgeFeaValues);
	void mapFeatureToIndex();
public:
	TLogisticRegression(const PCtmsNet& net) : TSignPredictor(net) { mapFeatureToIndex(); }
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};

class TCTMSProbabilisticInference : public TSignPredictor {	
protected:	
	static TTriadEqClasH ctmsEqClasses;
	THash<TChA, TFlt> theta;
	void extractFeatures(const TIntPr& edge, THash<TChA, TInt>& edgeFeaValues);
public:
	TCTMSProbabilisticInference(const PCtmsNet& net) : TSignPredictor(net) {
		if (ctmsEqClasses.Empty())
			TCtmsNet::GenTriadEquivClasses(ctmsEqClasses);
	}
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};

class TCTMSProbabilisticInferenceLocal : public TSignPredictor {
	// Not included in the paper
protected:	
	void extractFeatures(const TIntPr& edge, THash<TChA, TInt>& edgeFeaValues);
public:
	TCTMSProbabilisticInferenceLocal(const PCtmsNet& net) : TSignPredictor(net) {}
	void build();
	int predict(const TInt srcNId, const TInt desNId);
};
#endif // !SIGN_PREDICTION2_H