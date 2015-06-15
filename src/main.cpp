#include <iostream>
#include <ginac/ginac.h>
#include <libxml/parser.h>

using namespace std;
using namespace GiNaC;

int main() {
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	doc = xmlReadFile("../glops/test-diode-clipper.sch", NULL, 0);
	if (doc == NULL) {
		cout << "Cannot open file" << endl;
		return 1;
	}

	ex a = symbol("test") + 5;
	cout << "Hello, World!" << endl;
	cout << a;
	return 0;
}