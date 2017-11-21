#ifndef MAIN_H
#define MAIN_H
class Main{
public:
	static std::vector<std::string> takeInput();
	static std::vector<std::string> readFile(std::string);
	static void handleCode(std::vector<std::string>);
	static void userMenu();
private:
};
#endif // !MAIN_H
