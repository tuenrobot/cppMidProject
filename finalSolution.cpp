#include<iostream>
#include<cmath>
using namespace std;

void inputInfo(int numStation, int day, int*& numMachine, int*& fixCostM, int*& costProduce, 
	int*& productivity, int*& rate, int*& costStoreResource, int*& numStoreResource, int*& demand, int*& costBuy); // 輸入題目資訊  
int getDailyMaxProduct(const int numStation, const int numMachine[], const int productivity[], const int rate[]); // 得到每一天最高能生產的完成品數量
void adjustDemand(int day, int dailyMaxP, int*& demand);	 // 有時候需求會超出每一天最高能生產的完成品數量，所以要調整每天的需求量，超出的部分提前生產 	 
void cntNumProduce(int* demand, int numStation, int day, int* rate, int*& numStoreResource, int**& numProduce); // 計算某天各個工作站需要生產的數量 
void cntNumOpenM(int numStation, int day, int** numProduce, int productivity[], int**& numOpenM); // 計算某天各個工作站要開的機台數  
void cntNumBuy(int day, int* rate, int*& numStoreResource, int**& numProduce, int*& numBuy); // 計算某天要買的原料數 
void adjustNumBuy(int day, int* numStoreResource, int* costStoreResource, int*&numBuy, int* costBuy); // 依價格調整原料購買量 
void printOutcome(int day, int numStation, int** numOpenM, int** numProduce, int* numBuy); //  輸出計算結果  

int main() 
{
	// 輸入工作站數、時間
	int numStation = 0, day = 0; 
	cin >> numStation >> day;
	
	// 把工作站0加進去
	int* numMachine = new int[numStation + 1]; // 各工作站總機台數
	int* fixCostM = new int[numStation + 1]; // 各機台開機成本 
	int* costProduce = new int[numStation + 1]; // 各機台製造成本
	int* productivity = new int[numStation + 1]; // 各機台製造上限
	int* rate = new int[numStation + 1]; // 各機台製成率
	int* costStoreResource =  new int [numStation + 1]; // 各工作站存貨成本
	int* numStoreResource = new int [numStation + 1]; // 各工作站初始存貨
	int* demand = new int[day]; // 每一天的需求量
	int* costBuy = new int[day]; // 每一天的原料成本
	int** numProduce = new int*[numStation + 1]; // 各工作站每天的生產量
	for (int i = 1; i <= numStation; i++)
		numProduce[i] = new int[day];
	int* numBuy = new int[day]; // 原料採購量	
	int** numOpenM = new int*[numStation + 1]; // 各工作站每天實際開的機台數 
	for (int i = 1; i <= numStation; i++)
		numOpenM[i] = new int[day];
		
	// numBuy、numProduce、numOpenM初始化為0 
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
		rate, costStoreResource, numStoreResource, demand, costBuy); // 輸入其他資訊 
		
	int dailyMaxP = getDailyMaxProduct(numStation, numMachine, productivity, rate); // 計算每天最大能製造的完成品數量

	adjustDemand(day, dailyMaxP, demand); // 調整每天的需求量	 


	for (int t = 0; t < day; t++) // 日子從0開始 
	{			
		cntNumProduce(demand, numStation, t, rate, numStoreResource, numProduce);	// 計算某一天各工作站需要生產多少 		
		cntNumOpenM(numStation, t, numProduce, productivity, numOpenM); // 計算某一天各個工作站的開機台數
		cntNumBuy(t, rate, numStoreResource, numProduce, numBuy); // 計算某一天要買的原料數量
	}
	
	adjustNumBuy(day, numStoreResource, costStoreResource, numBuy, costBuy); // 調整原料購買量 

	printOutcome(day, numStation, numOpenM, numProduce, numBuy); // 輸出結果 
	 
	return 0;
}

// 輸入題目資訊 
void inputInfo(int numStation, int day, int*& numMachine, int*& fixCostM, int*& costProduce, 
	int*& productivity, int*& rate, int*& costStoreResource, int*& numStoreResource, int*& demand, int*& costBuy)
{
	// 輸入 
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

// 得到每一天最高能生產的完成品數量	
int getDailyMaxProduct(const int numStation, const int numMachine[], const int productivity[], const int rate[])
{
	// 先假設投入在rate的轉換過程，max的值不會大於該工作站的上限 
	// 沒有productivity[0]啦 
	// 最理想的狀況是工作站1達產能上限 
	int max = numMachine[1] * productivity[1];
	
	// 從前一個工作站投入在製品 
	for (int i = 2; i <= numStation; i++)
	{
		max /= rate[i];
		
		if (max > numMachine[i] * productivity[i]) // 如果投入轉換出理論成品數量 > 實際該工作站的上限
			max = numMachine[i] * productivity[i]; // 當天就那個工作站只能產出該工作站的上限
	} 
	
	return max;
}

// 有時候需求會超出每一天最高能生產的完成品數量，所以要調整每天的需求量，超出的部分提前生產 
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

// 計算某天各個工作站需要生產的數量 
void cntNumProduce(int* demand, int numStation, int day, int* rate, int*& numStoreResource, int**& numProduce)
{
	//  individualP = 每一個工作站這一天要完成的數量(製造 + 庫存) 
	int individualP = demand[day];
	
	// 這個工作站需要交出去的量(製造 + 庫存) = 下一個工作站的製造完成的數量 * r 
	// 從最後一個工作站回推回去 
	for (int i = numStation; i >= 1 ; i--)
	{	
		if (i == numStation) // 如果是最後一個工作站 
			individualP = demand[day];	
		else // 不然這個工作站應該製造的量 = (前一個工作站需要製造出的完成量) * r 
			individualP = numProduce[i + 1][day] * rate[i + 1]; // 是由後一個工作站需要投入的數量決定 

		numProduce[i][day] = individualP; // 如果這個工作站沒有庫存 
			
		if (numStoreResource[i] > 0) // 如果工作站尚有庫存，這個工作站不用生產到原本預計的數量 
		{	
			if (numProduce[i][day] <= numStoreResource[i])	
			{
				// 這兩個順序不能顛倒!!! 
				// 更新存貨
				numStoreResource[i] -= numProduce[i][day]; 
				// 這一天這個工作站不用生產 
				numProduce[i][day] = 0;
			}
			
			else
			{
				// 這兩個順序不能顛倒!!! 
				// 這一天這個工作站少生產一些
				numProduce[i][day] -= numStoreResource[i];	
				// 更新存貨，存貨全部被扣完 
				numStoreResource[i] = 0; 

			}
		}			
	}
}


// 計算某天各個工作站要開的機台數 
void cntNumOpenM(int numStation, int day,  int** numProduce,  int* productivity, int**& numOpenM)
{
	for (int i = 1; i <= numStation; i++)
	{
		// 由各工作站每天的產出，計算各工作站每天開了幾台機器	
		numOpenM[i][day] = numProduce[i][day] / productivity[i];  
		if (numProduce[i][day] % productivity[i] != 0)
			numOpenM[i][day] += 1;			
	}
}

// 計算某天要買的原料數
void cntNumBuy(int day, int* rate, int*& numStoreResource, int**& numProduce, int*& numBuy)
{

	// 如果放原料的地方，沒有原料庫存，應該買這麼多原料 
	numBuy[day] = numProduce[1][day] * rate[1]; 	

	if (numStoreResource[0] > 0) // 如果有原料的庫存
	{
		if (numBuy[day] > numStoreResource[0])
		{
			numBuy[day] -= numStoreResource[0];
			numStoreResource[0] = 0;
		}
		
		else // 表示原料的庫存夠，當天不用買原料 
		{
			numStoreResource[0] -= numBuy[day];
			numBuy[day] = 0; 
		}
	}
}

// 依價格調整原料購買量 
void adjustNumBuy(int day, int* numStoreResource, int* costStoreResource, int*&numBuy, int* costBuy)
{
	// 反覆做這件事 (讓原料特別便宜的那天，在有省成本的情況下，買比較多的原料 
	for (int i = 0; i < (day - 1); i++)
	{
		// 如果先買明天原料的(購買 + 存貨成本) < 明天買原料的原料成本，就今天先買明天的原料 
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

//  輸出計算結果 
void printOutcome(int day, int numStation, int** numOpenM, int** numProduce, int* numBuy)
{
	// 輸出每日各工作站開機台數 
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
	
	// 輸出每日各工作站產量
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
	
	// 輸出每日原料購買量
	for (int t = 0; t < day; t++)
	{
		cout << numBuy[t];
		
		if (t != (day - 1))
			cout << " ";	
	} 
}
