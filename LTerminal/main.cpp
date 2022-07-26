#include <conio.h>
#include <windows.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <lmcons.h>
#include <LMaccess.h>
#include <lmerr.h>

#include "Helper.h"
#include "Commands.h"

int main()
{
	//clear and set cursor to bottom
	clear();
	char cusername[UNLEN + 1];
	wchar_t lpcusername[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(cusername, &username_len);
	mbstowcs(lpcusername, cusername, username_len);
	std::string username = std::string(cusername);
	USER_INFO_1* uinfo;
	if (NetUserGetInfo(NULL, lpcusername, 1, (LPBYTE*)&uinfo) != NERR_Success)
	{
		MessageBox(NULL, "Getting userinfo failed.", "ERROR", 0);
	}
	bool isUserAdmin = ((uinfo->usri1_priv == USER_PRIV_ADMIN) ? true : false);

	std::filesystem::path cwd = std::filesystem::current_path();
	std::filesystem::path pcwd = std::filesystem::current_path();
	while (1)
	{
		setColor(DEFAULT);

		std::cout << (isUserAdmin ? "~" : "") << cwd.string() << ">";
		std::string input;
		std::getline(std::cin, input);
		std::vector<std::string> splInput = split(input);
		std::string command = toLower(splInput[0]);
		std::vector<std::string> args(splInput.begin() + 1, splInput.end());

		if (command == LISTDIRECTORY)
		{
			for (const auto& file : std::filesystem::directory_iterator(cwd.string()))
			{
				setColor(GREEN_TEXT);
				std::cout << file.path().filename() << std::endl;
			}
		}
		else if (command == EXIT) break;
		else if (command == CHANGEDIRECTORY)
		{
			std::filesystem::path oldPath = cwd;
			if (args.size() > 0)
			{
				if (args[0].length() == 0) continue;
				if (args[0].length() <= 1) continue;

				bool pass = true;
				if (args[0] == "..\\")
				{
					changePath(&cwd, &pcwd, cwd.parent_path().string());
				}
				for (int i = 0; i < args[0].length(); i += 2)
				{
					if (i + 1 <= args[0].length())
					{
						if (args[0][i] == '.' && args[0][i + 1] == '.')
						{
							pass = false;
						}
					}
				}
				if (!pass) continue;

				else if (args[0] == SUB_PREVDIRECTORY) {
					std::string _tmppcwd = pcwd.string();
					cwd = pcwd;
					pcwd = _tmppcwd;
				}
				else {
					std::filesystem::path ncwd;
					if (contains(args[0], ':') && contains(args[0], '\\'))
					{
						ncwd = args[0];
						if (std::filesystem::exists(ncwd))
							changePath(&cwd, &pcwd, ncwd.string());
						else {
							std::cout << "Incorrect Drive.\n";
						}
					}
					else {
						ncwd = cwd.string() + "\\" + args[0];
						if (std::filesystem::exists(ncwd))
							changePath(&cwd, &pcwd, ncwd.string());
						else {
							std::cout << "Incorrect path.\n";
						}
					}
				}
			}
		}
		else
		{
			std::string executeString = command;
			if (args.size() > 0)
			{
				executeString += " ";
				for (std::string str : args)
				{
					executeString += str + " ";
				}
			}
			if(executeString[executeString.length()] == ' ') executeString.resize(executeString.length() - 1);
			system(executeString.c_str());
		}

		// End
		splInput.clear();
		input = "";
	}

	return 0;
}