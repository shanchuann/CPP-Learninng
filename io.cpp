#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
/*******************************************************************************************************************
* 标准输入流
* C 标准输入
* (请在main函数中取消对应注释以运行相关函数)
* 标准输入流及对缓冲区的理解
* stdin是一个文件描述符（Linux）或句柄（Windows），它在 C 程序启动时就被默认分配好
* 在 Linux 中一切皆文件，stdin也相当于一个可读文件，它对应着键盘设备的输入。因为它不断地被输入，又不断地被读取，像流水一样，因此通常称作输入流
* stdin是一种行缓冲I/O。当在键盘上键入字符时，它们首先被存放在键盘设备自身的缓存中（属于键盘硬件设备的一部分）
* 只有输入换行符时，操作系统才会进行同步，将键盘缓存中的数据读入到stdin的输入缓冲区（存在于内存中）
* 所有从stdin读取数据的输入流，都是从内存中的输入缓冲区读入数据。当输入缓冲区为空时，函数将被阻塞
*
* 1. C语言中的输入函数
*   1.1. scanf()：输入函数，按照特定格式从stdin读取输入。
*		char str[100];
*		int a;
*		scanf("%s %d", str, &a);    // 注意，传入的一定是变量的地址
*		1.1.1.对空白字符的处理：
*		1.1.2. 缓冲区开头：丢弃空白字符（包括空格、Tab、换行符），直到第一个非空白字符才认为是第一个数据的开始。
*		1.1.3. 缓冲区中间：开始读取第一个数据后，一旦遇到空白字符（非换行符）， 就认为读取完毕一次。
*			   遇到的空白字符残留在缓冲区，直到下一次被读取或刷新。例如输入字符串this is test，则会被认为是3个字符串。
*		1.1.4. 缓冲区末尾：按下回车键时，换行符\n残留在缓冲区。换行符之前的空格可以认为是中间的空白字符，处理同上。
*   1.2. gets():按下回车键时，从stdin读取一行。
*		char str[100];
*		gets(str);
*		1.3.1 对空白字符的处理：
* 		所有空格、Tab等空白字符均被读取，不忽略。
*		按下回车键时，缓冲区末尾的换行符被丢弃，字符串末尾没有换行符\n，缓冲区也没有残留的换行符\n。
*		注意，gets()不能指定读取上限，因此容易发生数组边界溢出，造成内存不安全。
*		C11 使用了gets_s()代替gets()，但有时编译器未必支持，因此总体来说不建议使用gets()函数来读取输入。
*	1.3. fgets():从指定输入流读取一行，输入可以是stdin，也可以是文件流，使用时需要显式指定。
*       1.3.1 读取文件流
* 			char str[100];
*			memset(str, 0, sizeof(str));
*			int i = 1;
*			FILE *fp = fopen("...test.txt", "r");
*			if (fp == NULL) {
*			    printf("File open Error!\n");
*			    exit(1);
*			}
*			while (fgets(str, sizeof(str), fp) != NULL) // 读取文件流, 每次读取一行, 直到文件结束
*			    printf("line%d [len %d]: %s", i++, strlen(str), str);
*			fclose(fp);
*		1.3.2 读取stdin
* 			char str[100];
*			memset(str, 0, sizeof(str));
*			int i = 1;
*			while (fgets(str, sizeof(str), stdin) != NULL)
*				printf("line%d [len %d]: %s", i++, strlen(str), str);
* 	    1.3.3 对空白字符的处理：
*           所有空格、Tab等空白字符均被读取，不忽略。
*           按下回车键时，缓冲区末尾的换行符也被读取，字符串末尾将有一个换行符\n。
*           例如，输入字符串hello，再按下回车，则读到的字符串长度为6
*   1.4. fgetc() & getc()
*           从指定输入流读取一个字符，输入可以是stdin，也可以是文件流，使用时需要显式指定。
*           这两个函数完全等效，getc()由fgetc()宏定义而来。不同的是，前述的gets()和fgets()相互之间没有关系。
*           char a, b;
*           a = fgetc(stdin);
*           b = getc(stdin);
*   1.5. getchar()
*           从stdin读取一个字符。
*           getchar()实际上也由fgetc()宏定义而来，只是默认输入流为stdin。
*           char a;
*           a = getchar();
*		getchar()常常用于清理缓冲区开头残留的换行符。当知道缓冲区开头有\n残留时，可以调用getchar()但不赋值给任何变量，即可实现冲刷掉\n的效果。
*		getchar()对应的输出函数是putchar()。
* 
* 标准输出流
* C 标准输出
* 标准输出流及对缓冲区的理解
* 相应于输入流的stdin，输出流也有其默认的文件描述符stdout，对应着命令行终端（Windows 中称为控制台）的显示。
* 此外，还有对应错误输出的stderr，默认也是终端的显示。它们都可以被重定向到文件中以便持久保存和查看，在此不作赘述。
* stdout也是行缓冲I/O，它与stdin类似也有三者之间的数据同步
* 从用户程序到stdout的输出缓冲区，由用户程序决定；从stdout的输出缓冲区到终端的显示，只有缓冲区末尾遇到换行符\n才会进行
* 如果输出缓冲区末尾没有换行符\n，是不会打印显示输出的
* 
* 2. C语言中的输出函数
*	2.1 printf()：按照特定格式将stdout缓冲区的内容打印到终端。
*		printf("Number a = %d", a);      // 十进制整数
*		printf("Number b = %.2f", b);    // 浮点数，保留两位小数
*		printf("String s = %s", s);      // 字符串
*	2.2 puts():输出一个字符串到stdout，自动在字符串末尾添加换行符\n。
* 		puts("Hello, C plusplus!");      // 输出Hello, C plusplus!
*	2.3 fputs():输出一个字符串到指定输出流，输出可以是stdout，也可以是文件流，使用时需要显式指定。
* 		fputs("Hello, C plusplus!", stdout); // 输出Hello, C plusplus!
*	2.4 fputc() & putc()
* 		向指定输出流输出一个字符，输出可以是stdout，也可以是文件流，使用时需要显式指定。
* 		这两个函数完全等效，putc()由fputc()宏定义而来。不同的是，前述的puts()和fputs()相互之间没有关系。
* 		char a = 'A', b = 'B';
* 		fputc(a, stdout);
* 		putc(b, stdout);
* 2.5 putchar()
* 		向stdout输出一个字符。
* 		putchar('A');
*		putchar()对应的输入函数是getchar()。
* 2.6 fflush():该函数的功能是强制刷新缓冲区，将数据立即写到对应的文件（或设备）。其参数可以是文件流指针，也可以是stdout。
*		fputs("Hello World!", stdout);
*		fflush(stdout);
*		while (1);
*		上面的程序在进入死循环前，会输出Hello World!字符串到屏幕。
*		注意：不能够将fflush()用于stdin！这可能导致不可预料的后果。
* 
* C++ 标准输入输出
*	C++ 标准输入
*	C++中使用标准输入输出需要包含头文件<iostream>。一般使用iostream类进行流操作，其封装很完善，也比较复杂，本文只介绍一部分。
*		cin
*		cin是 C++ 的标准输入流对象，即istream类的一个对象实例。cin有自己的缓冲区，但默认情况下是与stdin同步的，因此在 C++ 中可以混用 C++ 和 C 风格的输入输出（在不手动取消同步的情况下）。
*		cin与stdin一样是行缓冲，即遇到换行符时才会将数据同步到输入缓冲区。
* 		cin的用法非常多，只列举常用的几种。最常用的就是使用>>符号（我认为该符号形象地体现了“流”的特点）。
*		cin对空白字符的处理与scanf一致。即：跳过开头空白字符，遇到空白字符停止读取，且空白字符（包括换行符）残留在缓冲区。
*		如果不想跳过空白字符，可以使用流控制关键词noskipws（no skip white space），但这只对单个字符有效（类似于scanf中的%c）。
*		注意，cin对象属于命名空间std，如果想使用cin对象，必须在 C++ 文件开头写using namespace std，或者在每次用到的时候写成std::cin。
*	cin.get():读取单个或指定长度的字符，包括空白字符。
*		char a, b;
*		char str[20] 
*		// 读取一个字符，读取失败时返回0，多余字符残留在缓冲区（包括换行符）
*		a = cin.get();
*		// 读取一个字符，读取失败时返回EOF，多余字符残留在缓冲区（包括换行符）
*		cin.get(b);
*		// 在遇到指定终止字符（参数3）前，至多读取n-1个（参数2）字符
*		// 当不指定终止字符时，默认为换行符\n
*		// 如果输入的字符个数小于等于n-1（不含终止字符），则终止字符不残留在缓冲区
*		// 如果输入的字符个数多于n-1（不含终止字符），则余下字符将残留在缓冲区
*		cin.get(str, sizeof(str), '\n');
* 
*		cin.get()读取单个字符时，类似于 C 中的fgetc()，对空白字符的处理也与其一致。cin.get()读取的字符也可以赋值给整型变量
*		cin.get()读取指定长度个字符时，类似于 C 中的fgets()，但在换行符的处理上不同。
*		它们都不会使换行符残留在缓冲区，但fgets()会将缓冲区末尾的换行符\n也写入字符串，而cin.get()会丢弃缓冲区末尾的\n。
*		即：当输入test时，用fgets()读取得到的字符串长度为5，用cin.get()读取得到的字符串长度为4。
*	cin.getline():读取指定长度的字符，包括空白字符。
*		char str[20];
*		cin.getline(str, sizeof(str));    // 第3个参数也可以指定终止字符
* 
*		cin.getline()与cin.get()指定读取长度时的用法几乎一样。
*		区别在于，如果输入的字符个数大于指定的最大长度n-1（不含终止符
*		cin.get()会使余下字符残留在缓冲区，等待下次读取
*		而cin.getline()会给输入流设为 Fail 状态，在主动恢复之前，无法再进行正常输入。
*	getline()
*		getline()并不是标准输入流istream的函数，而是字符串流sstream的函数，只能用于读取数据给string类对象，使用时也需要包含头文件<string>。
*		如果使用getline()读取标准输入流的数据，需要显式指定输入流。
*		string str;
*		getline(cin, str);
* 
*		getline()会读取所有空白字符，且缓冲区末尾的换行符会被丢弃，不残留也不写到字符串结尾。同时，由于string对象的空间是动态分配的，所以会一次性将缓冲区读完，不存在读不完残留在缓冲区的问题。
*		需要注意的是，假如缓冲区开头就是换行符（比如可能是上一次cin残留的），则getline()会直接读取到空字符串并结束，不会给键盘输入的机会。
*		所以这种情况下要注意先清除开头的换行符。
*	C++ 标准输出
*		cout
*			cout是 C++ 的标准输出流对象，即ostream类的一个对象实例。cout有自己的缓冲区，但默认情况下是与stdout同步的，因此在 C++ 中可以混用 C++ 和 C 风格的输入输出（在不手动取消同步的情况下）。
* 			cout与stdout一样是行缓冲I/O，即遇到换行符时才会将数据同步到输出缓冲区。
* 			cout的用法非常多，只列举常用的几种。最常用的就是使用<<符号（我认为该符号形象地体现了“流”的特点）。
* 			cout对空白字符的处理与printf一致。即：遇到空白字符停止输出，且空白字符（包括换行符）不会被输出。
* 			cout << "Number a = " << a;      // 十进制整数
* 			cout << "Number b = " << fixed << setprecision(2) << b;    // 浮点数，保留两位小数
* 			cout << "String s = " << s;      // 字符串
* 			cout << "Hello, C plusplus!";    // 输出Hello, C plusplus!
*			插入endl对象时，将立即清空输出缓冲区并显示，然后输出一个换行符\n。也有cout.put()等函数，不常用。
*		cerr
*			cerr是标准错误流，也是ostream类的一个实例，并默认输出设备为显示屏上的命令行终端。它默认与stderr同步。
*			cerr是非缓冲的，即插入数据时会立即输出。
*			char str[] = "File open FAILED!";
*			cerr << "[Error] " << str;
*		clog
*			clog是标准日志流，也是ostream类的一个实例，并默认输出设备为显示屏上的命令行终端。它默认与stderr同步。
*			clog是缓冲的，即插入数据时不会立即输出，而是等到缓冲区满或遇到换行符时才输出。
*			clog << "Failed!";
*			while(1){}
* 		上面的程序在进入死循环前，会输出Failed!字符串到屏幕。
*	输入字符串
*		输入字符串时，C语言中的输入函数scanf()和gets()，C++中的输入流cin和getline()，以及C++中的字符串流getline()都有各自的特点。
* 		在使用时，需要根据实际情况选择合适的函数，以避免不必要的麻烦。
*		在 C++ 中，cin.getline()和getline()是最常用的输入字符串函数，它们可以读取一行字符串，且不会出现缓冲区溢出的问题。
*		在 C 语言中，fgets()是最常用的输入字符串函数，它可以读取一行字符串，且不会出现缓冲区溢出的问题。
*		在 C 语言中，gets()是最不推荐的输入字符串函数，它容易导致缓冲区溢出，造成内存不安全。
* 		在 C++ 中，cin.get()和cin.getline()是最常用的输入字符串函数，它们可以读取指定长度的字符串，且不会出现缓冲区溢出的问题。
*		char str[100];
*		cin >> str;
*		cout << str;
* 		cin.getline(str, sizeof(str));
*		cout << str;
*		cin.get(str, n, '@');
*		cout << str;
*******************************************************************************************************************/
void i_C()
{
	// 1. C语言中的输入函数
	// 1.1. scanf()：输入函数，按照特定格式从stdin读取输入。
	char str[100]; // 用于存放字符串
	int num;		   // 用于存放整数
	printf("Please input a string and a number: ");
	scanf("%s %d", str, &num); // 注意，传入是变量的地址,注意格式化字符串的格式 Hello 123
	printf("str = %s, a = %d\n", str, num); // 输出结果 str = Hello, a = 123

	getchar(); // 用于清除缓冲区中的换行符

	// 1.2. gets():按下回车键时，从stdin读取一行。
	char str1[100];
	printf("Please input a string: ");
	gets_s(str1); // C11 使用了gets_s()代替gets()
	printf("str = %s\n", str1);
	// 1.3. fgets():从指定输入流读取一行，输入可以是stdin，也可以是文件流，使用时需要显式指定。
	//读取文件流
	char str2[100];
	memset(str2, 0, sizeof(str2));
	int i = 1;
	FILE* fp = fopen("./io.cpp.txt", "r");
	if (fp == NULL) {
		printf("File open Error!\n");
		exit(1);
	}
	printf("Read file io.cpp.txt\n");
	while (fgets(str2, sizeof(str2), fp) != NULL)
		printf("line%d [len %d]: %s", i++, (int)strlen(str2), str2);
	fclose(fp);
	printf("\n");
	// 读取stdin
	char str3[100];
	// 将str3的所有元素初始化为0
	memset(str3, 0, sizeof(str3));
	printf("Please input a string(input'@' to exit): ");
	int j = 1;
	// 从标准输入流中读取一行，输入@结束
	while (fgets(str3, sizeof(str3), stdin) != NULL && str3[0] != '@')
		printf("line%d [len %d]: %s", j++, (int)strlen(str3), str3);
	printf("\n");
	// 1.4. fgetc() & getc()
	// 从指定输入流读取一个字符，输入可以是stdin，也可以是文件流，使用时需要显式指定。
	// 这两个函数完全等效，getc()由fgetc()宏定义而来。不同的是，前述的gets()和fgets()相互之间没有关系。
	char a, b;
	printf("Please input two character: ");
	a = fgetc(stdin);
	b = getc(stdin);
	printf("a = %c, b = %c\n", a, b);
	getchar(); // 用于清除缓冲区中的换行符
	// 1.5. getchar()
	// 从stdin读取一个字符。
	// getchar()实际上也由fgetc()宏定义而来，只是默认输入流为stdin。
	char c;
	printf("Please input a character: ");
	c = getchar();
	printf("c = %c\n", c);
	printf("\n");
}
void o_C()
{
	// 2. C语言中的输出函数
	// 2.1 printf()：按照特定格式将stdout缓冲区的内容打印到终端。
	int a = 123;
	float b = 3.14159;
	char s[] = "Hello, C plusplus!";
	printf("Number a = %d\n", a);	 // 十进制整数
	printf("Number b = %.2f\n", b); // 浮点数，保留两位小数
	printf("String s = %s\n", s);	 // 字符串
	// 2.2 puts():输出一个字符串到stdout，自动在字符串末尾添加换行符\n。
	puts("Hello, C plusplus!"); // 输出Hello, C plusplus!
	// 2.3 fputs():输出一个字符串到指定输出流，输出可以是stdout，也可以是文件流，使用时需要显式指定。
	fputs("Hello, C plusplus!", stdout); // 输出Hello, C plusplus!
	printf("\n");
	// 2.4 fputc() & putc()
	// 向指定输出流输出一个字符，输出可以是stdout，也可以是文件流，使用时需要显式指定。
	// 这两个函数完全等效，putc()由fputc()宏定义而来。不同的是，前述的puts()和fputs()相互之间没有关系。
	char a1 = 'A', b1 = 'B';
	fputc(a1, stdout);
	putc(b1, stdout);
	printf("\n");
	// 2.5 putchar()
	// 向stdout输出一个字符。
	putchar('A');
	printf("\n");
	// 2.6 fflush():该函数的功能是强制刷新缓冲区，将数据立即写到对应的文件（或设备）。其参数可以是文件流指针，也可以是stdout。
	fputs("Hello World!", stdout);
	fflush(stdout);
	int i = 10;
	while (i > 0)
	{
		printf("i = %d\n", i);
		i--;
	}
	printf("END\n");
	// 上面的程序在进入死循环前，会输出Hello World!字符串到屏幕。
}
#include<iomanip>
void i_cpp() {
	//cin
	//cin是 C++ 的标准输入流对象，即istream类的一个对象实例。cin有自己的缓冲区，但默认情况下是与stdin同步的，因此在 C++ 中可以混用 C++ 和 C 风格的输入输出（在不手动取消同步的情况下）。
	//cin与stdin一样是行缓冲，即遇到换行符时才会将数据同步到输入缓冲区。
	//cin的用法非常多，只列举常用的几种。最常用的就是使用>>符号（我认为该符号形象地体现了“流”的特点）。
	std::cout << "Please input two integer and a string(cin): ";
	int a, b;
	std::cin >> a >> b;
	std::cout << "a = " << a << ", b = " << b << std::endl;
	std::cout << "Please input a string: ";
	char str[20];
	std::cin >> str;
	std::cout << "str = " << str << std::endl;
	getchar(); // 用于清除缓冲区中的换行符
	//cin对空白字符的处理与scanf一致。即：跳过开头空白字符，遇到空白字符停止读取，且空白字符（包括换行符）残留在缓冲区。
	//如果不想跳过空白字符，可以使用流控制关键词noskipws（no skip white space），但这只对单个字符有效（类似于scanf中的%c）。
	std::cout << "Please input a character（no skip white space）: ";
	char c,d;
	std::cin >> std::noskipws >> c;
	std::cout << "c = " << c << std::endl;
	std::cout << "Please input a character: ";
	//将cin恢复到默认状态，从而丢弃空白符
	std::cin >> std::skipws>> d;
	std::cout << "d = " << d << std::endl;
	//注意，cin对象属于命名空间std，如果想使用cin对象，必须在 C++ 文件开头写using namespace std，或者在每次用到的时候写成std::cin。
	//cin.get():读取单个或指定长度的字符，包括空白字符。
	getchar(); // 用于清除缓冲区中的换行符
	std::cout << "Please input a character(cin.get()): ";
	using namespace std;
	char e, f;
	char str1[20];

	// 读取一个字符，读取失败时返回0，多余字符残留在缓冲区（包括换行符）
	e = cin.get();
	cout << "e = " << e << endl;
	getchar(); // 用于清除缓冲区中的换行符
	// 读取一个字符，读取失败时返回EOF，多余字符残留在缓冲区（包括换行符）
	std::cout << "Please input a character(cin.get()): ";
	cin.get(f);
	cout << " f = " << f << endl;
	getchar(); // 用于清除缓冲区中的换行符
	// 在遇到指定终止字符（参数3）前，至多读取n-1个（参数2）字符
	// 当不指定终止字符时，默认为换行符\n
	// 如果输入的字符个数小于等于n-1（不含终止字符），则终止字符不残留在缓冲区
	// 如果输入的字符个数多于n-1（不含终止字符），则余下字符将残留在缓冲区
	std::cout << "Please input a string(cin.get()): ";
	cin.get(str1, sizeof(str1), '\n');
	cout << "str1 = " << str1 << endl;
	getchar(); // 用于清除缓冲区中的换行符
	//cin.get()读取单个字符时，类似于 C 中的fgetc()，对空白字符的处理也与其一致。cin.get()读取的字符也可以赋值给整型变量
	//cin.get()读取指定长度个字符时，类似于 C 中的fgets()，但在换行符的处理上不同。
	//它们都不会使换行符残留在缓冲区，但fgets()会将缓冲区末尾的换行符\n也写入字符串，而cin.get()会丢弃缓冲区末尾的\n。
	//即：当输入test时，用fgets()读取得到的字符串长度为5，用cin.get()读取得到的字符串长度为4。
	//cin.getline():读取指定长度的字符，包括空白字符。
	std::cout << "Please input a string(cin.getline()): ";
	char str2[20];
	std::cin.getline(str2, sizeof(str2));
	std::cout << "str2 = " << str2 << std::endl;
	//cin.getline()与cin.get()指定读取长度时的用法几乎一样。
	//区别在于，如果输入的字符个数大于指定的最大长度n-1（不含终止符
	//cin.get()会使余下字符残留在缓冲区，等待下次读取
	//而cin.getline()会给输入流设为 Fail 状态，在主动恢复之前，无法再进行正常输入。
	//getline()
	//getline()并不是标准输入流istream的函数，而是字符串流sstream的函数，只能用于读取数据给string类对象，使用时也需要包含头文件<string>。
	//如果使用getline()读取标准输入流的数据，需要显式指定输入流。
	std::cout << "Please input a string(getline()): ";
	std::string str3;
	std::getline(std::cin, str3);
	std::cout << "str3 = " << str3 << std::endl;
	//getline()会读取所有空白字符，且缓冲区末尾的换行符会被丢弃，不残留也不写到字符串结尾。同时，由于string对象的空间是动态分配的，所以会一次性将缓冲区读完，不存在读不完残留在缓冲区的问题。
	//需要注意的是，假如缓冲区开头就是换行符（比如可能是上一次cin残留的），则getline()会直接读取到空字符串并结束，不会给键盘输入的机会。所以这种情况下要注意先清除开头的换行符。
}
void o_cpp() {
	//C++ 标准输出
	//cout
	//cout是 C++ 的标准输出流对象，即ostream类的一个对象实例。cout有自己的缓冲区，但默认情况下是与stdout同步的，因此在 C++ 中可以混用 C++ 和 C 风格的输入输出（在不手动取消同步的情况下）。
	//cout与stdout一样是行缓冲I/O，即遇到换行符时才会将数据同步到输出缓冲区。
	//cout的用法非常多，只列举常用的几种。最常用的就是使用<<符号（我认为该符号形象地体现了“流”的特点）。
	int a = 123;
	float b = 3.14159;
	char s[] = "Hello, C plusplus!";
	std::cout << "Number a = " << a << std::endl;	 // 十进制整数
	std::cout << "Number b = " << std::fixed << std::setprecision(2) << b << std::endl; // 浮点数，保留两位小数
	std::cout << "String s = " << s << std::endl;	 // 字符串
	std::cout << "Hello, C plusplus!" << std::endl; // 输出Hello, C plusplus!
	//插入endl对象时，将立即清空输出缓冲区并显示，然后输出一个换行符\n。也有cout.put()等函数，不常用。
	//cerr
	//cerr是标准错误流，也是ostream类的一个实例，并默认输出设备为显示屏上的命令行终端。它默认与stderr同步。
	//cerr是非缓冲的，即插入数据时会立即输出。
	char str[] = "File open FAILED!";
	std::cerr << "[Error] " << str << std::endl;
	//clog
	//clog是标准日志流，也是ostream类的一个实例，并默认输出设备为显示
	//屏上的命令行终端。它默认与stderr同步。
	//clog是缓冲的，即插入数据时不会立即输出，而是等到缓冲区满或遇到换行符时才输出。
	std::clog << "Failed!" << std::endl;
	int i = 10;
	while (i > 0)
	{
		std::cout << "i = " << i << std::endl;
		i--;
	}
	std::cout << "END" << std::endl;
	//上面的程序在进入死循环前，会输出Failed!字符串到屏幕。
}
using namespace std;
void str_cpp() {
	const int n = 10;
	char str[n];
	cout << "Please input a string: ";
	cin >> str;
	cout << str << endl;
	getchar(); // 用于清除缓冲区中的换行符
	cout << "Please input a string): ";
	cin.getline(str, sizeof(str));
	cout << str << endl;
	cout << "Please input a string('@' to end): ";
	cin.get(str, n, '@');
	cout << str;
}
void Const() {
	const int a = 10;
	int* p = (int*)&a;
	int b = 0;
	*p = 100;
	b = a;
	printf("a = %d, *p = %d, b = %d\n", a, *p, b);
}
void Hong() {
	cout << __FILE__ << endl;
	cout << __DATE__ << endl;
	cout << __TIME__ << endl; //记录编译时间
	cout << __LINE__ << endl;
	cout << __func__ << endl;
	cout << __cplusplus << endl;
}
#if 0
int main()
{
	std::cout << "Result print" << std::endl;
	std::cout << std::endl;
	//i_C();
	//o_C();
	//i_cpp();
	//o_cpp();
	//str_cpp();
	//Const();
	//Hong();
	return 0;
}
#endif