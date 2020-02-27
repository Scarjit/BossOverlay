#include "BossTimer.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include "date.h"

#include "imgui/imgui.h"

using namespace std;
using namespace std::chrono;

quicktype::BossTimers dt;
static bool initialized;
void read_config()
{
	if (!initialized) {
		std::ifstream inFile;
		inFile.open("config.json");
		std::stringstream strStream;
		strStream << inFile.rdbuf();
		quicktype::BossTimers data = nlohmann::json::parse(strStream.str());
		dt = data;
		initialized = true;

	}
}

std::vector<std::string> split(std::string strToSplit, char delimeter)
{
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}


void BossTimerTab()
{
	read_config();

	auto current_time = std::chrono::system_clock::now();
	date::sys_days now_in_days{ std::chrono::time_point_cast<date::days>(current_time) };
	date::weekday weekday{ now_in_days };

	time_t tt = system_clock::to_time_t(current_time);
	tm local_tm = *localtime(&tt);

	ImVec4 color_red = ImVec4(255, 0, 0, 255);
	ImVec4 color_green = ImVec4(0, 255, 0, 255);
	ImVec4 color_yellow = ImVec4(255, 255, 0, 255);
	

	int c_hour = local_tm.tm_hour;
	int c_minute = local_tm.tm_min;
	int c_weekday = weekday.c_encoding();
	bool next_day = false;
	
	if (c_hour == 23 && c_minute > 15)
	{
		c_weekday = (c_weekday + 1) % 7;
		c_hour = 0;
		c_minute = 0;
		next_day = true;
	}
	
	for (std::pair<const std::basic_string<char>, quicktype::BossTimersValue> pair : dt)
	{
		std::vector<basic_string<char>> current_day;
		switch (c_weekday)
		{
		case 1:
			current_day = pair.second.get_monday();
			break;
		case 2:
			current_day = pair.second.get_tuesday();
			break;
		case 3:
			current_day = pair.second.get_wednesday();
			break;
		case 4:
			current_day = pair.second.get_thursday();
			break;
		case 5:
			current_day = pair.second.get_friday();
			break;
		case 6:
			current_day = pair.second.get_saturday();
			break;
		case 7:
			current_day = pair.second.get_sunday();
			break;
		default:
			current_day = pair.second.get_monday();
			break;
		}

		if (!current_day.empty()) {
			std::vector<string> ts_splits = split(pair.first, ':');
			int hour = std::stoi(ts_splits[0]);
			int minute = std::stoi(ts_splits[1]);


			if (c_hour > hour)
			{
				ImGui::TextColored(color_red, pair.first.c_str());
			}
			else if (c_hour < hour)
			{
				ImGui::TextColored(color_yellow, pair.first.c_str());
			}
			else
			{
				if (c_minute > minute)
				{
					ImGui::TextColored(color_red, pair.first.c_str());
				}
				else
				{
					int minute_till = local_tm.tm_min - minute;
					if (next_day)
					{
						minute_till = minute + (60 - local_tm.tm_min);
					}

					string soon = pair.first + " [T-" + std::to_string(minute_till) + " Minuten]";
					ImGui::TextColored(color_green, soon.c_str());
				}
			}

			ImGui::Indent(10);

			for (std::vector<basic_string<char>>::value_type daily_boss : current_day)
			{
				if (daily_boss.length() == 0)
				{
					ImGui::Text("--");					
				}
				else
				{
					ImGui::Text(daily_boss.c_str());
				}
			}

			ImGui::Unindent(10);
		}
	}
}