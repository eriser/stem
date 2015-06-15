//
// Created by Artem Godin on 15/06/15.
//

#ifndef STEM_ELEMENT_H
#define STEM_ELEMENT_H

class Element;

#include "../Model.h"

class Element {
	friend class Model;

public:
	Element() { }

	virtual ~Element() { }

protected:
	virtual void attach(Model *model) = 0;

	virtual void set(Model *model) = 0;

	virtual void apply(Model *model) = 0;
};


#endif //STEM_ELEMENT_H
