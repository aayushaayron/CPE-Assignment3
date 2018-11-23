//git try
//libraries
#include <iostream>
#include <string>
#include <fstream>
//#include <Windows.h>

using namespace std;

//function prototypes
void numDataPoints(string fileName, int &numTemp, int &numWSpeed);
void dataInArray(string fileName, int& numTemp, int& numWSpeed, int* temps, int* windSpeed, double** tempDrop);
void userInput(int& apprTemp, int& apprWSpeed, double& tempInput, double& wSpeedInput, int& numTemp, int& numWindSpeed, int* temps, int* windSpeed);
void indexTracing(int apprTemp, int apprWSpeed, int& indexTemps,int& indexWindSpeed,int* temps, int* windSpeed, int numTemp, int numWSpeed);
void temperatureDrop (double tempInput,int indexTemps, int indexWindSpeed, double** tempDrop, double& WindChillTemperature);

int main()
{
	//variables
	string fileName="chill.txt";
	int exit = 0;
	string exitStrat;
	int numTemp=0, numWSpeed=0, apprTemp=0, apprWSpeed=0;
	double tempInput=0, wSpeedInput=0;
	int indexTemps=0, indexWindSpeed=0;
	double WindChillTemperature;
	
	//asks for filename
	// cout << "Enter the filename: ";
	// cin >> fileName;
	
	//calls the function to count the number of datapoint in first two lines
	//will be used to dynamically allocate arrays
	numDataPoints(fileName, numTemp, numWSpeed);

	//dynamically allocating arrays
	//1d array
	int* temps;
	temps = new int[numTemp];
	int* windSpeed;
	windSpeed = new int[numWSpeed];

	//dynamically allocates 2d array
	double** tempDrop;
	tempDrop = new double*[numWSpeed];
	for (int i = 0; i < numWSpeed; i++)
	{
		tempDrop[i] = new double[numTemp];
	}
	
	//loops and checks for temp drop in the same file until exit is entered and "exit" is set to -1
	while (exit!=-1)
	{
		dataInArray(fileName, numTemp, numWSpeed, temps, windSpeed, tempDrop);
			
		// for (int i = 0; i < numTemp; i++)
		// {
		// 	cout << temps[i] << "  ";
		// }
		// cout << endl;
		//gets temperature and windspeed value from user
		userInput(apprTemp, apprWSpeed, tempInput, wSpeedInput, numTemp, numWSpeed, temps, windSpeed);
		
		//gets the correct index
		indexTracing(apprTemp, apprWSpeed, indexTemps, indexWindSpeed, temps,windSpeed, numTemp, numWSpeed);
		// cout << indexTemps <<endl;
		// cout << indexWindSpeed <<endl;

		temperatureDrop(tempInput,indexTemps,indexWindSpeed,tempDrop, WindChillTemperature);

		cout << "The Wind Chill Temperature Is : " <<WindChillTemperature <<endl;

		// cout << apprTemp <<endl;	
		// cout <<apprWSpeed <<endl;
		cout << "Do you wish to continue?\n";
		cout << "Enter anything to continue.\n";
		cout << "Enter \"exit\" to exit the program.\n";
		cout << "Your choice : ";
		cin >> exitStrat;
		if (exitStrat=="exit")
			exit=-1;

	}
	//memory deallocation	
	delete [] temps;
	delete [] windSpeed;
	for (int i = 0; i < numTemp; i++)
	{
		delete[] tempDrop[i];
	}
	delete[] tempDrop;

	
	
	//system("pause");
	return(0);
}

void numDataPoints(string fileName, int &numTemp, int &numWSpeed)
{
	//local variables
	char charac = 'a';

	//reading from file
	ifstream inFile;
	inFile.open(fileName, ios::in);
	
	//checks if file opening has failed
	if (inFile.fail())
	{
		cerr << "Error Opening File";
		//Sleep(2000);
		exit(-1);
	}

	//get every character and temporarily stores them into a variable until end of line is reached
	while (charac != '\n')
	{
		inFile.get(charac);
		if (charac == ' ' || charac=='\n')
			numTemp++; //incremented everytime a space is encountered and when end of line is reached. This value denotes the number of data values in the line
	}
	

	charac = 'a';

	//get every character and temporarily stores them into a variable until end of line is reached
	while (charac != '\n')
	{
		inFile.get(charac);
		if (charac == ' ' || charac == '\n')
			numWSpeed++; //incremented everytime a space is encountered and when end of line is reached. This value denotes the number of data values in the line
	}

	inFile.close();
}

void dataInArray(string fileName, int& numTemp, int& numWSpeed, int* temps, int* windSpeed, double** tempDrop)
{
	//creates input filestream "inFile for reading"
	ifstream inFile;
	inFile.open(fileName, ios::in);
	if (inFile.fail())
	{
		cerr << "Error Opening File." << endl;
		exit(-1);
		//Sleep(2000);
	}

	// puts values from the first line into an array
	for (int i=0; i < numTemp;i++)
	{
		inFile >> temps[i];
	}
	// puts values from the second line into an array
	for (int i=0; i < numWSpeed; i++)
	{
		inFile >> windSpeed[i];
	}

	//put the temperature values from the remainder of the lines into a 2d array
	for (int i=0; i < numWSpeed;i++)
	{
		for (int j=0; j< numTemp; j++)
		{
			inFile >> tempDrop[i][j];
		}
	}

	inFile.close();
}

void userInput(int& apprTemp, int& apprWSpeed, double& tempInput, double& wSpeedInput, int& numTemp, int& numWindSpeed, int* temps, int* windSpeed)
{
	// do while loop - asks the user for a temperature value until a valid value is provided
	do
	{
		cout << "Enter A Temperature Value Between " << temps[numTemp - 1] << " and " << temps[0] << " : ";
		/*for (int i = 0; i<numTemp; i++)
			cout << "temps[" << i << "] = " << temps[i] << endl;*/
		cin >> tempInput;
		if (tempInput>temps[0] || tempInput<temps[numTemp - 1])
		{
			cout << "Temperature Value Entered Is Out Of Range\n";
		}
	} while (tempInput>temps[0] || tempInput < temps[numTemp - 1]);
	

	// do while loop - operates until a valid windSpeed is provided
	do
	{
		cout << "Enter A Wind Speed Between " << windSpeed[0] << " and " << windSpeed[numWindSpeed - 1] << " : ";
		cin >> wSpeedInput;
		if (wSpeedInput < windSpeed[0] || wSpeedInput>windSpeed[numWindSpeed - 1])
		{
			cout << "Wind Speed Entered Is Out Of Range.\n";
		}
	} while (wSpeedInput < windSpeed[0] || wSpeedInput >windSpeed[numWindSpeed - 1]);

	//selects the value from the array whose value is closer to the input value
	for (int i = 0; i < numTemp-1; i++)
	{
		if (tempInput<temps[i] && tempInput>temps[i + 1])
		{
			double avg = (temps[i] + temps[i + 1]) / 2.0;
			if (tempInput >= avg)
			{
				apprTemp = temps[i];
			}
			else
			{
				apprTemp = temps[i + 1];
			}
		}
		else if (tempInput==temps[i])
			apprTemp=temps[i];
		else if (tempInput==temps[i+1])
			apprTemp=temps[i+1];
	}

	//selects the value from the array whose value is closer to the input value
	for (int i=0; i< numWindSpeed-1; i++)
	{
		if (wSpeedInput >windSpeed[i] && wSpeedInput< windSpeed[i+1])
		{
			int avg= (windSpeed[i]+windSpeed[i+1])/2.0;
			if (wSpeedInput < avg)
				apprWSpeed=windSpeed[i];
			else if (wSpeedInput>=avg)
				apprWSpeed=windSpeed[i+1];
		}
		else if (wSpeedInput==windSpeed[i])
			apprWSpeed=windSpeed[i];
		else if (wSpeedInput==windSpeed[i+1])
			apprWSpeed=windSpeed[i+1];
	}
}

void indexTracing(int apprTemp, int apprWSpeed, int& indexTemps,int& indexWindSpeed,int* temps, int* windSpeed, int numTemp, int numWSpeed)
{
	//looks for the index of the appropriate temperature value in the temperature array and updates in the main function
	for (int i=0; temps[i]==apprTemp || i<numTemp; i++)
	{
		if (temps[i]==apprTemp)
			indexTemps=i;
	}
	
	//checks for which value in the array matches the appropriate wind speed and updates in the main function
	for (int i=0;windSpeed[i]==apprWSpeed || i<numWSpeed;i++)
	{
		if (windSpeed[i]==apprWSpeed)
			indexWindSpeed=i;
	}
}

void temperatureDrop (double tempInput,int indexTemps, int indexWindSpeed, double** tempDrop, double& WindChillTemperature)
{
	double corresValue;
	double computed;

	corresValue=tempDrop[indexWindSpeed][indexTemps];


	computed=corresValue-tempInput;
	WindChillTemperature=computed;

}