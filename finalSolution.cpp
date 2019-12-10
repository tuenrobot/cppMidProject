#include<iostream>
#include<cmath>
using namespace std;

void inputInfo(int numStation, int day, int*& numMachine, int*& fixCostM, int*& costProduce, 
	int*& productivity, int*& rate, int*& costStoreResource, int*& numStoreResource, int*& demand, int*& costBuy); // ��J�D�ظ�T  
int getDailyMaxProduct(const int numStation, const int numMachine[], const int productivity[], const int rate[]); // �o��C�@�ѳ̰���Ͳ��������~�ƶq
void adjustDemand(int day, int dailyMaxP, int*& demand);	 // ���ɭԻݨD�|�W�X�C�@�ѳ̰���Ͳ��������~�ƶq�A�ҥH�n�վ�C�Ѫ��ݨD�q�A�W�X���������e�Ͳ� 	 
void cntNumProduce(int* demand, int numStation, int day, int* rate, int*& numStoreResource, int**& numProduce); // �p��Y�ѦU�Ӥu�@���ݭn�Ͳ����ƶq 
void cntNumOpenM(int numStation, int day, int** numProduce, int productivity[], int**& numOpenM); // �p��Y�ѦU�Ӥu�@���n�}�����x��  
void cntNumBuy(int day, int* rate, int*& numStoreResource, int**& numProduce, int*& numBuy); // �p��Y�ѭn�R����Ƽ� 
void adjustNumBuy(int day, int* numStoreResource, int* costStoreResource, int*&numBuy, int* costBuy); // �̻���վ����ʶR�q 
void printOutcome(int day, int numStation, int** numOpenM, int** numProduce, int* numBuy); //  ��X�p�⵲�G  

int main() 
{
	// ��J�u�@���ơB�ɶ�
	int numStation = 0, day = 0; 
	cin >> numStation >> day;
	
	// ��u�@��0�[�i�h
	int* numMachine = new int[numStation + 1]; // �U�u�@���`���x��
	int* fixCostM = new int[numStation + 1]; // �U���x�}������ 
	int* costProduce = new int[numStation + 1]; // �U���x�s�y����
	int* productivity = new int[numStation + 1]; // �U���x�s�y�W��
	int* rate = new int[numStation + 1]; // �U���x�s���v
	int* costStoreResource =  new int [numStation + 1]; // �U�u�@���s�f����
	int* numStoreResource = new int [numStation + 1]; // �U�u�@����l�s�f
	int* demand = new int[day]; // �C�@�Ѫ��ݨD�q
	int* costBuy = new int[day]; // �C�@�Ѫ���Ʀ���
	int** numProduce = new int*[numStation + 1]; // �U�u�@���C�Ѫ��Ͳ��q
	for (int i = 1; i <= numStation; i++)
		numProduce[i] = new int[day];
	int* numBuy = new int[day]; // ��Ʊ��ʶq	
	int** numOpenM = new int*[numStation + 1]; // �U�u�@���C�ѹ�ڶ}�����x�� 
	for (int i = 1; i <= numStation; i++)
		numOpenM[i] = new int[day];
		
	// numBuy�BnumProduce�BnumOpenM��l�Ƭ�0 
	for (int t = 0; t < day; t++)
	{
		numBuy[t] = 0;
		for (int i = 1; i <= numStation; i++)
		{
			numProduce[i][t] = 0;	
			numOpenM[i][t] = 0;
	
		}	
	}

	inputInfo(numStation, day, numMachine, fixCostM, costProduce, productivity, 
		rate, costStoreResource, numStoreResource, demand, costBuy); // ��J��L��T 
		
	int dailyMaxP = getDailyMaxProduct(numStation, numMachine, productivity, rate); // �p��C�ѳ̤j��s�y�������~�ƶq

	adjustDemand(day, dailyMaxP, demand); // �վ�C�Ѫ��ݨD�q	 


	for (int t = 0; t < day; t++) // ��l�q0�}�l 
	{			
		cntNumProduce(demand, numStation, t, rate, numStoreResource, numProduce);	// �p��Y�@�ѦU�u�@���ݭn�Ͳ��h�� 		
		cntNumOpenM(numStation, t, numProduce, productivity, numOpenM); // �p��Y�@�ѦU�Ӥu�@�����}���x��
		cntNumBuy(t, rate, numStoreResource, numProduce, numBuy); // �p��Y�@�ѭn�R����Ƽƶq
	}
	
	adjustNumBuy(day, numStoreResource, costStoreResource, numBuy, costBuy); // �վ����ʶR�q 

	printOutcome(day, numStation, numOpenM, numProduce, numBuy); // ��X���G 
	 
	return 0;
}

// ��J�D�ظ�T 
void inputInfo(int numStation, int day, int*& numMachine, int*& fixCostM, int*& costProduce, 
	int*& productivity, int*& rate, int*& costStoreResource, int*& numStoreResource, int*& demand, int*& costBuy)
{
	// ��J 
	for (int i = 1; i <= numStation; i++)
		cin >> numMachine[i];	
	for (int i = 1; i <= numStation; i++)
		cin >> fixCostM[i];
	for (int i = 1; i <= numStation; i++)
		cin >> costProduce[i];
	for (int i = 1; i <= numStation; i++)
		cin >> productivity[i];
	for (int i = 1; i <= numStation; i++)
		cin >> rate[i];
	for (int i = 0; i <= numStation; i++)
		cin >> costStoreResource[i];
	for (int i = 0; i <= numStation; i++)
		cin >> numStoreResource[i];
	for (int i = 0; i < day; i++)
		cin >> demand[i];
	for (int i = 0; i < day; i++)
		cin >> costBuy[i];	
}

// �o��C�@�ѳ̰���Ͳ��������~�ƶq	
int getDailyMaxProduct(const int numStation, const int numMachine[], const int productivity[], const int rate[])
{
	// �����]��J�brate���ഫ�L�{�Amax���Ȥ��|�j��Ӥu�@�����W�� 
	// �S��productivity[0]�� 
	// �̲z�Q�����p�O�u�@��1�F����W�� 
	int max = numMachine[1] * productivity[1];
	
	// �q�e�@�Ӥu�@����J�b�s�~ 
	for (int i = 2; i <= numStation; i++)
	{
		max /= rate[i];
		
		if (max > numMachine[i] * productivity[i]) // �p�G��J�ഫ�X�z�צ��~�ƶq > ��ڸӤu�@�����W��
			max = numMachine[i] * productivity[i]; // ��ѴN���Ӥu�@���u�ಣ�X�Ӥu�@�����W��
	} 
	
	return max;
}

// ���ɭԻݨD�|�W�X�C�@�ѳ̰���Ͳ��������~�ƶq�A�ҥH�n�վ�C�Ѫ��ݨD�q�A�W�X���������e�Ͳ� 
void adjustDemand(int day, int dailyMaxP, int*& demand)
{
	for (int i =(day - 1); i >= 0; i--)
	{
		if (demand[i] > dailyMaxP)
		{
			int extra = demand[i] - dailyMaxP;
			demand[i] -= extra;
			
			if (i != 0)
				demand[i - 1] += extra;
		}
	}
}

// �p��Y�ѦU�Ӥu�@���ݭn�Ͳ����ƶq 
void cntNumProduce(int* demand, int numStation, int day, int* rate, int*& numStoreResource, int**& numProduce)
{
	//  individualP = �C�@�Ӥu�@���o�@�ѭn�������ƶq(�s�y + �w�s) 
	int individualP = demand[day];
	
	// �o�Ӥu�@���ݭn��X�h���q(�s�y + �w�s) = �U�@�Ӥu�@�����s�y�������ƶq * r 
	// �q�̫�@�Ӥu�@���^���^�h 
	for (int i = numStation; i >= 1 ; i--)
	{	
		if (i == numStation) // �p�G�O�̫�@�Ӥu�@�� 
			individualP = demand[day];	
		else // ���M�o�Ӥu�@�����ӻs�y���q = (�e�@�Ӥu�@���ݭn�s�y�X�������q) * r 
			individualP = numProduce[i + 1][day] * rate[i + 1]; // �O�ѫ�@�Ӥu�@���ݭn��J���ƶq�M�w 

		numProduce[i][day] = individualP; // �p�G�o�Ӥu�@���S���w�s 
			
		if (numStoreResource[i] > 0) // �p�G�u�@���|���w�s�A�o�Ӥu�@�����ΥͲ���쥻�w�p���ƶq 
		{	
			if (numProduce[i][day] <= numStoreResource[i])	
			{
				// �o��Ӷ��Ǥ����A��!!! 
				// ��s�s�f
				numStoreResource[i] -= numProduce[i][day]; 
				// �o�@�ѳo�Ӥu�@�����ΥͲ� 
				numProduce[i][day] = 0;
			}
			
			else
			{
				// �o��Ӷ��Ǥ����A��!!! 
				// �o�@�ѳo�Ӥu�@���֥Ͳ��@��
				numProduce[i][day] -= numStoreResource[i];	
				// ��s�s�f�A�s�f�����Q���� 
				numStoreResource[i] = 0; 

			}
		}			
	}
}


// �p��Y�ѦU�Ӥu�@���n�}�����x�� 
void cntNumOpenM(int numStation, int day,  int** numProduce,  int* productivity, int**& numOpenM)
{
	for (int i = 1; i <= numStation; i++)
	{
		// �ѦU�u�@���C�Ѫ����X�A�p��U�u�@���C�Ѷ}�F�X�x����	
		numOpenM[i][day] = numProduce[i][day] / productivity[i];  
		if (numProduce[i][day] % productivity[i] != 0)
			numOpenM[i][day] += 1;			
	}
}

// �p��Y�ѭn�R����Ƽ�
void cntNumBuy(int day, int* rate, int*& numStoreResource, int**& numProduce, int*& numBuy)
{

	// �p�G���ƪ��a��A�S����Ʈw�s�A���ӶR�o��h��� 
	numBuy[day] = numProduce[1][day] * rate[1]; 	

	if (numStoreResource[0] > 0) // �p�G����ƪ��w�s
	{
		if (numBuy[day] > numStoreResource[0])
		{
			numBuy[day] -= numStoreResource[0];
			numStoreResource[0] = 0;
		}
		
		else // ��ܭ�ƪ��w�s���A��Ѥ��ζR��� 
		{
			numStoreResource[0] -= numBuy[day];
			numBuy[day] = 0; 
		}
	}
}

// �̻���վ����ʶR�q 
void adjustNumBuy(int day, int* numStoreResource, int* costStoreResource, int*&numBuy, int* costBuy)
{
	// ���а��o��� (����ƯS�O�K�y�����ѡA�b���٦��������p�U�A�R����h����� 
	for (int i = 0; i < (day - 1); i++)
	{
		// �p�G���R���ѭ�ƪ�(�ʶR + �s�f����) < ���ѶR��ƪ���Ʀ����A�N���ѥ��R���Ѫ���� 
		for (int t = 1; t < day; t++)
		{
			if ((numBuy[t] * (costBuy[t - 1] + costStoreResource[0])) < (numBuy[t] * costBuy[t]))
			{
				numBuy[t - 1] += numBuy[t];
				numBuy[t] = 0;
			}
		}		
	}
} 

//  ��X�p�⵲�G 
void printOutcome(int day, int numStation, int** numOpenM, int** numProduce, int* numBuy)
{
	// ��X�C��U�u�@���}���x�� 
	for (int i = 1; i <= numStation; i++)
	{
		for (int t = 0; t < day; t++)
		{
			cout << numOpenM[i][t];
			
			if (t != (day - 1))
				cout << " ";
		}
		
		cout << "\n";
	}
	
	// ��X�C��U�u�@�����q
	for (int i = 1; i <= numStation; i++)
	{
		for (int t = 0; t < day; t++)
		{
			cout << numProduce[i][t];
			
			if (t != (day - 1))
				cout << " ";
		}
		
		cout << "\n";
	}
	
	// ��X�C�����ʶR�q
	for (int t = 0; t < day; t++)
	{
		cout << numBuy[t];
		
		if (t != (day - 1))
			cout << " ";	
	} 
}
