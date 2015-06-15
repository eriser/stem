//
// Created by Artem Godin on 15/06/15.
//

#ifndef STEM_MODEL_H
#define STEM_MODEL_H

class Model;

#include <string>
#include <ginac/ginac.h>
#include "elements/Element.h"
#include "limits/Limit.h"

using namespace std;
using namespace GiNaC;

class Model {
public:
	Model();

	virtual ~Model();

public:
	void addResistor(const string id);

	void addEnergyStorage(const string id);

	void addVoltageSource(const string id);

	void addNonLinearComponent(const string id, const string reference);

	void addOutput(const string id);

	void addOpAmp(const string id, const string reference);

	void addFormula(const ex symbol, const ex expression);

	void addControl(const symbol variable, const string label, const string format, const string unit,
			const ex conversion, const ex defaults);

	void addNode(const string id);

	void addConnection(const string pinA, const string pinB);

	void addElement(Element *element);

	void apply(const symbol variable, const ex value);

	void stampU(const string id, const ex value);

	void stampNOpAmpInput(const string id, const string positive, const string negative);

	void stampNOpAmpOutput(const string id, const string output);

	void stampNr(const string id, const string terminalA, const string terminalB);

	void stampNx(const string id, const string terminalA, const string terminalB);

	void stampNu(const string id, const string terminal);

	void stampNn(const string id, const string terminalA, const string terminalB);

	void stampModel(const string id, const symbol voltage, const ex model, Limit *limit);

	void stampNo(const string id, const string terminal);

	void stampGr(const string id, const ex value);

	void stampGx(const string id, const ex value);

	void stampZ(const string id, const ex value);

	symbol getHz() { return Hz; }

	symbol getVin() { return vIn; }

	symbol getVout() { return vOut; }

private:
	void apply(matrix &M, const symbol variable, const ex value);

	void build();

	void calculate();

	void apply();

	void applyNonLinear();

	symbol Hz;
	symbol vIn;
	symbol vOut;

	map<string, int> nodes;
	int nodesCount;

	map<string, int> mResistors;
	map<string, int> mEnergyStorages;
	map<string, int> mVoltageSources;
	map<string, int> mNonLinearElements;
	map<string, int> mOutputs;
	map<string, int> mOpAmps;

	map<int, string> refNonLinears;
	map<int, string> refOpAmps;

	map<int, symbol> controls;
	map<int, string> controlLabels;
	map<int, string> controlUnits;
	map<int, string> controlFormats;
	map<int, ex> controlConversions;
	map<int, ex> controlDefaults;

	exmap controlFormulas;

	vector<Element *> elements;

	map<int, ex> models;
	map<int, symbol> voltages;
	map<int, Limit *> limits;

	matrix Nr;
	matrix Nx;
	matrix Nu;
	matrix Nn;
	matrix No;
	matrix NopaI;
	matrix NopaO;

	matrix Gr;
	matrix Gx;
	matrix Z;

	matrix S;
	matrix Sinv;

	matrix A;
	matrix B;
	matrix C;
	matrix D;
	matrix E;
	matrix F;
	matrix G;
	matrix H;
	matrix K;

	matrix U;
	matrix X;
	matrix I;
	matrix V;
	matrix P;

	matrix Y;
	matrix Xn;
	matrix Vn;
	matrix Vop;
	matrix Iop;

	matrix NV;
	matrix Jinv;

	bool mapContains(map<string, int> map, const string basic_string);
};


#endif //STEM_MODEL_H
