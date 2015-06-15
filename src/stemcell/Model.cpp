//
// Created by Artem Godin on 15/06/15.
//

#include "Model.h"

Model::Model() {
	Hz = symbol("Hz");
	vIn = symbol("Vin");
	vOut = symbol("Vout");

	// Ground node has index of -1
	nodesCount = 0;
	nodes["_"] = -1;
}

Model::~Model() {
}

void Model::addResistor(const string id) {
	if (!mapContains(mResistors, id)) {
		int i = (int) mResistors.size();
		mResistors[id] = i;
	}
}

bool Model::mapContains(map<string, int> map, const string key) {
	return map.count(key) == 0;
}


void Model::addEnergyStorage(const string id) {
	if (!mapContains(mEnergyStorages, id)) {
		int i = (int) mEnergyStorages.size();
		mEnergyStorages[id] = i;
	}
}

void Model::addVoltageSource(const string id) {
	if (!mapContains(mVoltageSources, id)) {
		int i = (int) mVoltageSources.size();
		mVoltageSources[id] = i;
	}
}

void Model::addNonLinearComponent(const string id, const string reference) {
	if (!mapContains(mNonLinearElements, id)) {
		int i = (int) mNonLinearElements.size();
		mNonLinearElements[id] = i;
		refNonLinears[i] = reference;
	}
}

void Model::addOutput(const string id) {
	if (!mapContains(mOutputs, id)) {
		int i = (int) mOutputs.size();
		mOutputs[id] = i;
	}
}

void Model::addOpAmp(const string id, const string reference) {
	if (!mapContains(mOpAmps, id)) {
		int i = (int) mOpAmps.size();
		mOpAmps[id] = i;
		refOpAmps[i] = reference;
	}
}

void Model::addFormula(const ex symbol, const ex expression) {
	controlFormulas[symbol] = expression;
}

void Model::addControl(const symbol variable, const string label, const string format, const string unit,
		       const ex conversion, const ex defaults) {
	for (unsigned int i = 0; i < controls.size(); i++) {
		if (variable.get_name() == controls[i].get_name()) return;
	}

	int i = (int) controls.size();
	controls[i] = variable;
	controlLabels[i] = label;
	controlFormats[i] = format;
	controlUnits[i] = unit;
	controlConversions[i] = conversion;
	controlDefaults[i] = defaults;
}

void Model::addNode(const string id) {
	if (!mapContains(nodes, id)) {
		nodes[id] = nodesCount++;
	}
}

void Model::addConnection(const string pinA, const string pinB) {
	int pinApos = -2;
	int pinBpos = -2;
	int newPos = -2;

	if (mapContains(nodes, pinA)) {
		pinApos = nodes[pinA];
	}

	if (mapContains(nodes, pinB)) {
		pinBpos = nodes[pinB];
	}

	if (pinApos == -2 && pinBpos == -2) {
		newPos = nodesCount++;
	} else if (pinApos != -2 && pinBpos == -2) {
		newPos = pinApos;
	} else if (pinBpos != -2 && pinApos == -2) {
		newPos = pinBpos;
	} else {
		// They both exists but different -- join them
		int oldPos;
		if (pinApos < pinBpos) {
			newPos = pinApos;
			oldPos = pinBpos;
		} else {
			newPos = pinBpos;
			oldPos = pinApos;
		}

		for (map<string, int>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (it->second == oldPos) {
				it->second = newPos;
			} else if (it->second > oldPos) {
				it->second = it->second - 1;
			}
		}

		nodesCount--;
	}

	nodes[pinA] = newPos;
	nodes[pinB] = newPos;
}

void Model::addElement(Element *element) {
	elements.push_back(element);
}

void Model::apply(const symbol variable, const ex value) {
	apply(A, variable, value);
	apply(B, variable, value);
	apply(C, variable, value);
	apply(D, variable, value);
	apply(E, variable, value);
	apply(F, variable, value);
	apply(G, variable, value);
	apply(H, variable, value);
	apply(K, variable, value);
	apply(I, variable, value);
	apply(U, variable, value);
}

void Model::stampU(const string id, const ex value) {
	int row = mVoltageSources[id];
	U((unsigned int) row, 0) = value;
}

void Model::stampNOpAmpInput(const string id, const string positive, const string negative) {
	int row = mOpAmps[id];
	int colPos = nodes[positive];
	int colNeg = nodes[negative];

	if (colPos >= 0) {
		NopaI((unsigned int) row, (unsigned int) colPos) = numeric(1);
	}
	if (colNeg >= 0) {
		NopaI((unsigned int) row, (unsigned int) colNeg) = numeric(-1);
	}
}

void Model::stampNOpAmpOutput(const string id, const string output) {
	int row = mOpAmps[id];
	int colOut = nodes[output];

	if (colOut >= 0) {
		NopaO((unsigned int) row, (unsigned int) colOut) = numeric(1);
	}


}

void Model::stampNr(const string id, const string terminalA, const string terminalB) {
	int row = mResistors[id];
	int colA = nodes[terminalA];
	int colB = nodes[terminalB];

	if (colA >= 0) {
		Nr((unsigned int) row, (unsigned int) colA) = numeric(1);
	}
	if (colB >= 0) {
		Nr((unsigned int) row, (unsigned int) colB) = numeric(-1);
	}
}

void Model::stampNx(const string id, const string terminalA, const string terminalB) {
	int row = mEnergyStorages[id];
	int colA = nodes[terminalA];
	int colB = nodes[terminalB];

	if (colA >= 0) {
		Nx((unsigned int) row, (unsigned int) colA) = numeric(1);
	}
	if (colB >= 0) {
		Nx((unsigned int) row, (unsigned int) colB) = numeric(-1);
	}

}

void Model::stampNu(const string id, const string terminal) {
	int row = mVoltageSources[id];
	int col = nodes[terminal];
	Nu((unsigned int) row, (unsigned int) col) = numeric(1);
}

void Model::stampNn(const string id, const string terminalA, const string terminalB) {
	int row = mNonLinearElements[id];
	int colA = nodes[terminalA];
	int colB = nodes[terminalB];

	if (colA >= 0) {
		Nn((unsigned int) row, (unsigned int) colA) = numeric(1);
	}
	if (colB >= 0) {
		Nn((unsigned int) row, (unsigned int) colB) = numeric(-1);
	}

}

void Model::stampModel(const string id, const symbol voltage, const ex model, Limit *limit) {
	int i = -1;
	if (mapContains(mNonLinearElements, id)) {
		i = mNonLinearElements[id];
	} else if (mapContains(mOpAmps, id)) {
		i = (int) (mOpAmps[id] + mNonLinearElements.size());
	}

	if (i >= 0) {
		models[i] = model;
		voltages[i] = voltage;
		limits[i] = limit;
	}
}

void Model::stampNo(const string id, const string terminal) {
	int row = mOutputs[id];
	int col = nodes[terminal];
	No((unsigned int) row, (unsigned int) col) = numeric(1);

}

void Model::stampGr(const string id, const ex value) {
	int i = mResistors[id];
	Gr((unsigned int) i, (unsigned int) i) = value;

}

void Model::stampGx(const string id, const ex value) {
	int i = mEnergyStorages[id];
	Gx((unsigned int) i, (unsigned int) i) = value;

}

void Model::stampZ(const string id, const ex value) {
	int i = mEnergyStorages[id];
	Z((unsigned int) i, (unsigned int) i) = value;

}

void Model::apply(matrix &M, const symbol variable, const ex value) {
	for (unsigned int i = 0; i < M.rows(); i++) {
		for (unsigned j = 0; j < M.cols(); j++) {
			M(i, j) = M(i, j).subs(variable == value);
		}
	}
}

void Model::build() {
	// Attach
	for (vector<Element *>::iterator it = elements.begin(); it != elements.end(); ++it) {
		//TODO (*it)->attach(this);
	}

	unsigned int columns = (unsigned int) (nodesCount + mVoltageSources.size() + mOpAmps.size());

	Nr = matrix((unsigned int) mResistors.size(), columns);
	Nx = matrix((unsigned int) mEnergyStorages.size(), columns);
	Nu = matrix((unsigned int) mVoltageSources.size(), columns);
	Nn = matrix((unsigned int) mNonLinearElements.size(), columns);
	No = matrix((unsigned int) mOutputs.size(), columns);
	NopaI = matrix((unsigned int) mOpAmps.size(), columns);
	NopaO = matrix((unsigned int) mOpAmps.size(), columns);

	Gr = matrix((unsigned int) mResistors.size(), (unsigned int) mResistors.size());
	Gx = matrix((unsigned int) mEnergyStorages.size(), (unsigned int) mEnergyStorages.size());
	Z = matrix((unsigned int) mEnergyStorages.size(), (unsigned int) mEnergyStorages.size());

	X = matrix((unsigned int) mEnergyStorages.size(), 1);
	Xn = matrix((unsigned int) mEnergyStorages.size(), 1);
	U = matrix((unsigned int) mVoltageSources.size(), 1);
	V = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()), 1);
	P = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()), 1);
	Vn = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()), 1);
	I = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()), 1);

	for (unsigned int i = 0; i < mEnergyStorages.size(); i++) {
		string x = string("X[");
		x.append(to_string(i)).append("]");
		X(i, 0) = symbol(x);
	}

	// Stamp
	for (vector<Element *>::iterator it = elements.begin(); it != elements.end(); ++it) {
		//TODO (*it)->set(this);
	}

	for (unsigned int i = 0; i < mNonLinearElements.size() + mOpAmps.size(); i++) {
		V(i, 0) = voltages[i];
		I(i, 0) = -models[i];
	}

}

void Model::calculate() {
	matrix s1 = Nr.transpose().mul(Gr).mul(Nr);
	matrix s2 = Nx.transpose().mul(Gx).mul(Nx);
	S = s1.add(s2);
	for (unsigned int i = 0; i < mVoltageSources.size(); i++) {
		for (unsigned int j = 0; j < nodesCount; j++) {
			S(i + nodesCount, j) = Nu(i, j);
			S(j, nodesCount + i) = Nu(i, j);
		}
	}
	for (unsigned int i = 0; i < mOpAmps.size(); i++) {
		for (unsigned int j = 0; j < nodesCount; j++) {
			S((unsigned int) (i + nodesCount + mVoltageSources.size()), j) = NopaO(i, j);
			S(j, (unsigned int) (nodesCount + mVoltageSources.size() + i)) = NopaO(i, j);
		}
	}
	Sinv = S.inverse();
	matrix ZGx = Z.mul(Gx).mul_scalar(numeric(2));
	A = ZGx.mul(Nx).mul(Sinv).mul(Nx.transpose()).sub(Z);
	matrix UI = matrix((unsigned int) mVoltageSources.size(),
			   (unsigned int) (nodesCount + mVoltageSources.size() + mOpAmps.size()));
	for (unsigned int i = 0; i < mVoltageSources.size(); i++) {
		UI(i, nodesCount + i) = 1;
	}
	matrix Nn2 = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()),
			    (unsigned int) (nodesCount + mVoltageSources.size() + mOpAmps.size()));
	for (unsigned int i = 0; i < mNonLinearElements.size(); i++) {
		for (unsigned int j = 0; j < nodesCount; j++) {
			Nn2(i, j) = Nn(i, j);
		}
	}
	for (unsigned int i = 0; i < mOpAmps.size(); i++) {
		for (unsigned int j = 0; j < nodesCount; j++) {
			Nn2((unsigned int) (i + mNonLinearElements.size()), j) = NopaI(i, j);
		}
	}
	matrix NNI = matrix((unsigned int) (mNonLinearElements.size() + mOpAmps.size()),
			    (unsigned int) (nodesCount + mVoltageSources.size() + mOpAmps.size()));
	for (unsigned int i = 0; i < mNonLinearElements.size(); i++) {
		for (unsigned int j = 0; j < nodesCount; j++) {
			NNI(i, j) = Nn(i, j);
		}
	}
	for (unsigned int i = 0; i < mOpAmps.size(); i++) {
		NNI((unsigned int) (i + mNonLinearElements.size()),
		    (unsigned int) (i + nodesCount + mVoltageSources.size())) = 1;
	}
	B = ZGx.mul(Nx).mul(Sinv).mul(UI.transpose());
	C = ZGx.mul(Nx).mul(Sinv).mul(NNI.transpose());
	D = No.mul(Sinv).mul(Nx.transpose());
	E = No.mul(Sinv).mul(UI.transpose());
	F = No.mul(Sinv).mul(NNI.transpose());
	G = Nn2.mul(Sinv).mul(Nx.transpose());
	H = Nn2.mul(Sinv).mul(UI.transpose());
	K = Nn2.mul(Sinv).mul(NNI.transpose());

}

void Model::apply() {
	for (vector<Element *>::iterator it = elements.begin(); it != elements.end(); ++it) {
		//TODO (*it)->apply(this);
	}
}

void Model::applyNonLinear() {
	P = G.mul(X).add(H.mul(U));
	NV = P.sub(V).add(K.mul(I));

	matrix J = matrix(
		(unsigned int) (mNonLinearElements.size() + mOpAmps.size()),
		(unsigned int) (mNonLinearElements.size() + mOpAmps.size()));
	// Calculate Jacobian for N-R
	for (unsigned int i = 0; i < mNonLinearElements.size() + mOpAmps.size(); i++) {
		for (unsigned int j = 0; j < mNonLinearElements.size() + mOpAmps.size(); j++) {
			J(i, j) = NV(i, 0).diff(voltages[j]);
		}
	}
	Jinv = J.inverse();
	// Note, Real Vnew = V + Vn
	Vn = Jinv.mul(NV).mul_scalar(numeric(-1));
	Vop = V;
}
