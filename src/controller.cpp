#include "controller.h"

namespace SDI
{
	controller::controller()
	{
		//Load in existing
		populateListOfProjects();
	}

	controller::~controller()
	{
		//Save existing
		saveListOfProjectsToDisk();
		//Free up memory held by list
		for (unsigned int i = 0; i < toBeRemoved.size(); i++)
		{
			delete toBeRemoved.at(i);
		}
		toBeRemoved.clear();
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			delete projectList.at(i);
		}
		projectList.clear();
		
	}

	//Load in existing projects from file
	void controller::populateListOfProjects()
	{
		std::ifstream projectsIn(projectsFilename);
		string parser;
		getline(projectsIn, parser);
		if (parser == "")
		{
			return;
		}
		string deliminator = ",";
		size_t position = 0;
		string projectIn;
		while ((position = parser.find(deliminator)) != std::string::npos) 
		{
			projectIn = parser.substr(0, position);
			project* projectRead = new project(std::stol(projectIn),true);
			projectList.push_back(projectRead);
			parser.erase(0, position + deliminator.length());
		}
		projectsIn.close();
	}

	//Save existing projects to file
	void controller::saveListOfProjectsToDisk()
	{
		std::ofstream projectsOut(projectsFilename);
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			projectsOut << std::to_string(projectList.at(i)->getProjectId()) << ",";
			
		}
		projectsOut.close();
	}

	vector<unsigned long long> controller::getAlphabeticProjects()
	{
		vector<unsigned long long> ids;
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			ids.push_back(projectList.at(i)->getProjectId());
		}
		if (ids.size() == 0)
		{
			return ids;
		}
		//Bubble sort ids alphabetically from title
		bool flag = 0;
		while (flag == 0)
		{
			bool change = 0;
			for (int i = 0; i < ids.size() - 1; i++)
			{
				if (getNameFromId(ids.at(i)) > getNameFromId(ids.at(i + 1)))
				{
					std::iter_swap(ids.begin() + i, ids.begin() + i + 1);
					change = 1;
				}
			}
			if (change == 0)
			{
				flag = 1;
			}
		}
		return ids;
	}

	vector<unsigned long long> controller::getProjectsTitleFilter(string titleFilter, vector<unsigned long long>& listIn)
	{
		vector<unsigned long long> filteredIds;
		for (unsigned int i = 0; i < listIn.size(); i++)
		{
			if (getNameFromId(listIn.at(i)).find(titleFilter) != string::npos)
			{
				filteredIds.push_back(listIn.at(i));
			}
		}
		return filteredIds;
	}

	vector<unsigned long long> controller::getProjectsActorFilter(string actorFilter, vector<unsigned long long>& listIn)
	{
		vector<unsigned long long> filteredIds;
		for (unsigned int i = 0; i < listIn.size(); i++)
		{
			vector<string> cast = getActorsFromId(listIn.at(i));
			unsigned long long added;
			for (unsigned int j = 0; j < cast.size(); j++)
			{
				if ((cast.at(j).find(actorFilter) != string::npos)&&(added!= listIn.at(i)))
				{
					filteredIds.push_back(listIn.at(i));
					added = listIn.at(i);
				}
			}
		}
		return filteredIds;
	}

	vector<unsigned long long> controller::getProjectsMaterialFilter(unsigned int materialFilter, vector<unsigned long long>& listIn)
	{
		vector<unsigned long long> filteredIds;
		for (unsigned int i = 0; i < listIn.size(); i++)
		{
			if (checkProjectHasMaterialType(listIn.at(i), materialFilter))
			{
				filteredIds.push_back(listIn.at(i));
			}
		}
		return filteredIds;
	}

	string controller::getNameFromId(unsigned long long id)
	{
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			if (projectList.at(i)->getProjectId() == id)
			{
				return projectList.at(i)->getTitle();
			}
		}
		return "";
	}

	vector<string> controller::getActorsFromId(unsigned long long id)
	{
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			if (projectList.at(i)->getProjectId() == id)
			{
				return projectList.at(i)->getCast();
			}
		}
		return vector<string>();
	}

	bool controller::checkProjectHasMaterialType(unsigned long long id, unsigned int type)
	{
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			if (projectList.at(i)->getProjectId() == id)
			{
				if (projectList.at(i)->hasMaterialType(type))
				{
					return true;
				}
			}
		}
		return false;
	}

	void controller::removeProject(unsigned long long id)
	{
		int indexToRemove = -1;
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			if (projectList.at(i)->getProjectId() == id)
			{
				projectList.at(i)->removeThisProject();
				indexToRemove = i;
				toBeRemoved.push_back(projectList.at(i));
			}
		}
		projectList.erase(projectList.begin() + indexToRemove);
	}

	//Calculations:
	unsigned long long controller::getNextProjectId()
	{
		if (projectList.size() > 0)
		{
			return (projectList.at(projectList.size() - 1)->getProjectId()) + 1000;
		}
		else
		{
			return 1;
		}
	}

	void controller::setCurrentProject(unsigned long long projectId)
	{
		for (unsigned int i = 0; i < projectList.size(); i++)
		{
			if (projectList.at(i)->getProjectId() == projectId)
			{
				currentProjectIndex = i;
			}
		}
	}

	void controller::setCurrentProjectIndex(int index)
	{
		currentProjectIndex = index;
	}

}