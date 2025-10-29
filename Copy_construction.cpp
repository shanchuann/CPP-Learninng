#include<iostream>

class Int {
private:
	int value;
public:
	Int() :value(0) { std::cout << "Default create Int" << "at" << this << std::endl; }
	Int(int v) :value(v) { std::cout << "Create Int with value " << value << " at " << this << std::endl; }
	Int(Int& it) :value(it.value) { std::cout << "Copy create Int with value " << value << " at " << this << std::endl; }
	~Int() { std::cout << "Destroy Int with value " << value << " at " << this << std::endl; }

	void Print() const {
		std::cout << "Print value: " << value << std::endl;
	}
};
Int func(Int it) {
	it.Print();
	return it;
}
int main() {
	Int a{ 10 };
	func(a);
	Int c = 0;
	c = func(a);
}
#if 0
class Int {
private:
	int value;
public:
	Int() :value(0) { std::cout << "Default create Int" << "at" << this << std::endl; }
	Int(int v) :value(v) { std::cout << "Create Int with value " << value << " at " << this << std::endl; }

	Int(Int& it) :value(it.value) { std::cout << "Copy create Int with value " << value << " at " << this << std::endl; }

	~Int() { std::cout << "Destroy Int with value " << value << " at " << this << std::endl; }

	void Print() const {
		std::cout << "Print value: " << value << std::endl;
	}
};
void func(Int it) {
	it.Print();
}
int main() {
	Int a{ 10 };
	func(a);
}

class Int {
private:
	int value;
public:
	Int() :value(0) { std::cout << "Default create Int" << "at" << this << std::endl; }
	Int(int v) :value(v) { std::cout << "Create Int with value " << value << " at " << this << std::endl; }

	Int(Int& it) :value(it.value) { std::cout << "Copy create Int with value " << value << " at " << this << std::endl; }

	~Int() { std::cout << "Destroy Int with value " << value << " at " << this << std::endl; }
};

int main() {
	Int a(10);
	Int b(20);
	Int c(b);
	return 0;
}
#endif