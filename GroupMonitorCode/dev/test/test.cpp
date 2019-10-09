// test.cpp : 定义控制台应用程序的入口点。
//

#include <CQMsgCode.h>
#include <iostream>
using namespace std;
using namespace CQ;


int main()
{
	CodeMsgs m("123456[CQ:sign,title=aaa:bbb,image=ccc=ddd]654321");
	m.debug();

	for each (auto&code in m)
	{
		if (code.isCQcode()) {
			auto keys = m.keys();

			cout << "{code:" << code.get() << ", args:{";
			for each (auto key in keys)
			{
				cout << "'" << key << "': '" << code.get(key) << "'" << ", ";
			}
			cout << "}";
		}
		else {
			cout << "[" << code.get() << "]";
		}
	}
	cout << endl;
	if (m[0].find("sign")) {
		cout << "sign find" << endl;
		cout << "sign find" << endl;
	}
	else {
		cout << "sign no find" << endl;
	}

	/*for (auto t = m.begin(); t < m.end(); t++) {

		if (t->isCode) {
			cout << "{code:" << m.at(t->key) << ", args:{";
			for (auto p = t->begin(); p < t->end(); p++) {
				cout << "'" << m.at(p->key) << "': '" << m.at(p->value) << "'";
				if (p + 1 != t->end()) cout << ", ";

			}
			cout << "}";
		}
		else {
			cout << "[" << m.at(t->key) << "]";
		}
	}*/
	getchar();
	return 0;
}

