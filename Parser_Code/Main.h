#ifndef MAIN_H
#define MAIN_H
class Main{
public:
	static std::vector<std::string> takeInput();
	static bool readFile(std::string, std::vector<std::string>*);
	static void handleCode(std::vector<std::string>, std::ostream&);
	static void userMenu();
private:
};
#endif // !MAIN_H
