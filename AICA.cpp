//Zachary Ables
//creating AICA and calculating values
//2/24/2020

#include<stdio.h>
#include<cstdlib>
#include<cmath>
#include<time.h>
#include<string.h>
#include<string>
using namespace std;

//creates a visible table
void table_disp(int table[][30]);

//calculates the distance between two cells when given their coordinates
int distance_between(int i1, int j1,int i2, int j2);

//continuously updates the table until it reaches a stable point
void update_table(int table[][30], double J1, double J2, double h, double R1, double R2);

//calculates the number of cells that have a distance < R1 from current location
int calc_num_close_cells(int table[][30], int i1, int j1, double R1);

//calculates the number of cells that have a distance >= R1 but less than R2 from current location
int calc_num_far_cells(int table[][30],int i1, int j1, double R1, double R2);

//calculates correlation
void calc_corr(int table[][30]);

//calculates basic entropy
double calc_entropy(int table[][30]);

//calculates joint entropy
void calc_joint_entropy(int table[][30]);

//uses entropy and joint entropy to calculate mutual information
void calc_mut_info(int table[][30]);

//executes a series of tests that are necessary for assignment
void execute_experiment3(int table[][30],double J1, double J2, double h, double R1, double R2);

//makes the table for the experiment
void make_table(int table[][30]);

//for testing purposes of experiment
void test(int table[][30], double J1, double J2, double h, double R1, double R2);

//creates a pgm of the experiment
void create_pgm(int table[][30], char * name);

double mut_info[15];
double corr[15];
double jentropy[15];



int main(int argc, char *argv[])
{
	double J1, J2, h, R1, R2;
	J1 = atof(argv[1]);
	J2 = atof(argv[2]);
	h = atof(argv[3]);
	R1 = atof(argv[4]);
	R2 = atof(argv[5]);
	srand(time(0));

	int table[30][30];
	execute_experiment3(table, J1, J2, h, R1, R2);

	//test(table,J1,J2,h,R1,R2);
	/*
	  char * name;
	  name = new char;
	  strcpy(name,"test");
	  create_pgm(table, name);
	  */
}
void test(int table[][30],double J1, double J2, double h, double R1, double R2)
{
	make_table(table);
	printf("Initial State:\n\n");
	table_disp(table);
	update_table(table,J1,J2,h,R1,R2);
	printf("\n\n\n");
	printf("Final State:\n\n");
	table_disp(table);
	calc_corr(table);
	calc_joint_entropy(table);
	calc_mut_info(table);
	printf("length, correlation, jentropy, mutual information\n");
	for(int i = 0; i < 15; i++)
		printf("%d, %f, %f, %f\n", i,corr[i], jentropy[i], mut_info[i]);
}
//makes the table that I will be using for this experiment
void make_table(int table[][30])
{
	for(int i = 0; i < 30;i++)
	{
		for(int j = 0;j < 30;j++)
		{
			if(rand() % 2 == 1)
				table[i][j] = 1;
			else
				table[i][j] = -1;
		}
	}
}

//this is a basic display of the table
void table_disp(int table[][30])
{
	for(int i = 0; i < 30;i++)
	{
		for(int j = 0;j < 30;j++)
		{
			if(table[i][j] == 1)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
}
//this calculates the distance between two points on a coordinate plane as specified in writeup
int distance_between(int i1, int j1, int i2, int j2)
{
	int dist;
	int horiz_dist;
	int vert_dist;
	int temp = abs(i1-i2);

	//the horizontal distance and vertical distance wrap around the edge of the screen so we use 30-temp when abs > 15;
	if(temp <= 15)
		horiz_dist = temp;
	else
		horiz_dist = 30-temp;

	temp = abs(j1-j2);

	if(temp <= 15)
		vert_dist = temp;
	else
		vert_dist = 30-temp;

	dist = horiz_dist + vert_dist;
	return dist;
}
void update_table(int table[][30], double J1, double J2, double h, double R1, double R2)
{
	int num_close_cells;
	int num_far_cells;
	double result;
	int changed[30][30];
	int flag = 0;
	int flag2 = 1;
	int rand_i;
	int rand_j;
	int num_changed = 0;
	for(int i = 0; i < 30; i++)
	{
		for(int j = 0; j < 30;j++)
		{
			changed[i][j] = 0;
		}
	}
	//while change is detected
	while(flag2 == 1)
	{
		flag = 0;
		flag2 = 0;
		//while cells need to be updated
		while(flag == 0)
		{
			//if all cells have updated, reset the double array and set num_changed to 0
			if(num_changed == 900)
			{
				for(int i = 0; i < 30; i++)
				{
					for(int j = 0; j < 30;j++)
					{
						changed[i][j] = 0;
					}
				}
				num_changed = 0;
			}
			//find a random cell to change
			do
			{
				rand_i = rand() % 30;
				rand_j = rand() % 30;
			}while(changed[rand_i][rand_j] == 1);

			num_changed++;
			if(num_changed == 900)
				flag = 1;
			//update the changed table
			changed[rand_i][rand_j] = 1;
			//here are the calculations for updating the cell
			num_close_cells = calc_num_close_cells(table,rand_i,rand_j,R1);
			num_far_cells = calc_num_far_cells(table,rand_i,rand_j,R1,R2);


			result = h + (J1 * num_close_cells) + (J2 * num_far_cells);
			//if positive
			if(result >= 0)
			{
				//if not the same as it was before
				if(table[rand_i][rand_j] != 1)
				{
					table[rand_i][rand_j] = 1;
					flag2 = 1;
				}	
			}
			//if negative
			else
			{
				//if not the same as it was before
				if(table[rand_i][rand_j] != -1)
				{
					table[rand_i][rand_j] = -1;
					flag2 = 1;
				}
			}
		}
		//uncomment these for step by step updates
		//printf("\n\n\n");
		//table_disp(table);
	}
}
//this calculates the total of cells nearby that are either positive or negative
int calc_num_close_cells(int table[][30], int i1, int j1, double R1)
{
	int cells = 0;
	for(int i2 = 0; i2 < 30;i2++)
	{
		for(int j2 = 0; j2 < 30;j2++)
		{
			//if not itself && closer than R1
			if(!(i1 == i2 && j1 == j2) && distance_between(i1,j1,i2,j2) < R1) 
				cells += table[i2][j2];
		}
	}
	return cells;
}
//this calculates the total of cells far away that are either positive or negative
int calc_num_far_cells(int table[][30],int i1, int j1, double R1, double R2)
{
	int cells = 0;
	int distance;
	for(int i2 = 0; i2 < 30;i2++)
	{
		for(int j2 = 0; j2 < 30;j2++)
		{
			if(!(i1 == i2 && j1 == j2)) 
			{
				distance = distance_between(i1,j1,i2,j2);
				if(distance >= R1 && distance < R2)
					cells += table[i2][j2];
			}
		}
	}
	return cells;
}
//calculates the correlation
void calc_corr(int table[][30])
{
	//Pl = correlation per distance l
	//Pl = (((2/((30^2) * 4l) * (sum of what the fuck is this)) - (1/(30^2) * (sum of table values))^2
	int sum_i = 0;
	int sum_ij;

	for(int i = 0; i < 30;i++)
	{
		for(int j = 0; j < 30;j++)
		{
			sum_i += table[i][j];
		}
	}
	//outer for loop that goes for each distance
	for(int k = 1; k < 15;k++)
	{
		sum_ij = 0;
		//i and j are si
		for(int i = 0; i < 30;i++)
		{
			for(int j = 0; j < 30;j++)
			{
				//l and m are sj
				for(int l = i; l < 30;l++)
				{
					for(int m = 0; m < 30;m++)
					{
						//If the two row indices are the same, make sure that the column index of cell j is greater than the column index for cell i.
						if((m > j) || (l > i))
						{
							//If it's the same as the current distance of the outer for loop
							if(distance_between(i,j,l,m) == k)
							{
								sum_ij += table[i][j] * table[l][m];
							}
						}
					}
				}
			}
		}

		double firstCoeff = 2.0/(900.0 * 4.0 * (double)k);
		double secondCoeff = 1.0/900.0;
		corr[k] = abs((firstCoeff * sum_ij) - pow(secondCoeff * (double)sum_i, 2));
		corr[0] = abs(1.0 - pow(secondCoeff * (double)sum_i, 2));
	}
}
//calculates entropy
double calc_entropy(int table[][30])
{
	double entropy;

	double prob; // probability of 1
	double antiprob; //probability of -1

	int converted_cells = 0;

	for(int i = 0; i < 30;i++)
	{
		for(int j = 0; j < 30;j++)
		{
			converted_cells += ((table[i][j]+1)/2);
		}
	}
	prob = (1.0/900.0) * converted_cells;
	antiprob = 1.0 - prob;

	if(prob <= 0)
		entropy = -(0 + (antiprob * log2(antiprob)));
	else if(antiprob <= 0)
		entropy = -((prob * log2(prob)) + 0);
	else
		entropy = -((prob * log2(prob)) + (antiprob * log2(antiprob)));

	return entropy;
}
//calculates joint entropy
void calc_joint_entropy(int table[][30])
{
	//B(s) = (1+s)/2
	int Bi = 0;
	int Bj = 0;
	int converted_total = 0;
	for(int k = 1; k < 15;k++)
	{
		converted_total = 0;
		//i and j are si
		for(int i = 0; i < 30;i++)
		{
			for(int j = 0; j < 30;j++)
			{
				Bi = (1 + table[i][j])/2;
				//l and m are sj
				for(int l = i; l < 30;l++)
				{
					for(int m = 0; m < 30;m++)
					{
						Bj = (1 + table[l][m])/2;
						//If the two row indices are the same, make sure that the column index of cell j is greater than the column index for cell i.
						if((m > j) || (l > i))
						{
							//If it's the same as the current distance of the outer for loop
							if(distance_between(i,j,l,m) == k)
							{
								converted_total += Bi * Bj;
							}
						}
					}
				}
			}
		}
		double prob = ((2.0/(900.0 * 4.0 * (double)k)) * converted_total);
		converted_total = 0;
		for(int i = 0; i < 30;i++)
		{
			for(int j = 0; j < 30;j++)
			{
				Bi = (-1 + table[i][j])/2;
				//l and m are sj
				for(int l = i; l < 30;l++)
				{
					for(int m = 0; m < 30;m++)
					{
						Bj = (-1 + table[l][m])/2;
						//If the two row indices are the same, make sure that the column index of cell j is greater than the column index for cell i.
						if((m > j) || (l > i))
						{
							//If it's the same as the current distance of the outer for loop
							if(distance_between(i,j,l,m) == k)
							{
								converted_total += Bi * Bj;
							}
						}
					}
				}
			}
		}

		double antiprob = (2.0/(900.0 * 4.0 * (double)k)) * converted_total;
		double midprob = 1.0 - prob - antiprob;
		double val1,val2,val3;
		if(prob <= 0.0)
			val1 = 0;
		else
			val1 = prob * log2(prob);
		if(antiprob <= 0.0)
			val2 = 0;
		else
			val2 = antiprob * log2(antiprob);
		if(midprob <= 0.0)
			val3 = 0;
		else
			val3 = midprob * log2(midprob);

		jentropy[k] = -(val1 + val2 + val3);
	}

}
//calculates mutual info
void calc_mut_info(int table[][30])
{
	//mut_info = (2 * entropy) - joint entropy
	double entropy = calc_entropy(table);
	for(int i = 0; i < 15; i++)
	{
		mut_info[i] = (2.0 * entropy) - jentropy[i];
	}
}
//creating a pgm file
void create_pgm(int table[][30], char * name)
{
	FILE* pgmimg;
	strcat(name, ".pgm");
	pgmimg = fopen(name, "wb"); //write the file in binary mode
	fprintf(pgmimg, "P2\n"); // Writing Magic Number to the File
	fprintf(pgmimg, "%d %d\n", 30, 30); // Writing Width and Height into the file

	fprintf(pgmimg, "255\n"); // Writing the maximum gray value

	for (int i = 0; i < 30; i++) 
	{
		for (int j = 0; j < 30; j++) 
		{
			if(table[i][j] == 1)
				fprintf(pgmimg, "255 "); //Copy gray value fromarray to file
			else
				fprintf(pgmimg, "0 ");
		}
		fprintf(pgmimg, "\n");
	}
	fclose(pgmimg);
}
//creating experiment 3
void execute_experiment3(int table[][30],double J1, double J2, double h, double R1, double R2)
{
	FILE * csv = fopen("Experiment.csv","wb");
	fprintf(csv, "Your, Name,\n");
	fprintf(csv, "Wrap:, true\n");
	fprintf(csv, "Random Seed:, 147");
	fprintf(csv,"\n\n");
	int count = 0;
	//R1
	//for(int i = 0; i < 7; i+= 2)
	for(int i = 0; i < 8; i+= 2)
	{
		R1 = (double)i;
		//R2
		for(int j = i+1; j < 15; j+=2)
		{
			R2 = (double)j;
			//h
			for(int k = -5; k <= 5;k+=2)
			{
				h = (double)k;
				
				fprintf(csv, "Experiment #:,%d\n",count);
				fprintf(csv, "J1: %f,J2: %f,h: %f,R1: %f,R2: %f\n",J1, J2, h, R1, R2);
				fprintf(csv, "length,Correlation,Joint Entropy,Mutual Information\n");
				double corravg[15] = {0};
				double jentropyavg[15] = {0};
				double mut_infoavg[15] = {0};
				for(int l = 0; l < 3;l++)
				{
					make_table(table);
					update_table(table,J1,J2,h,R1,R2);
					printf("J1: %f J2: %f h: %f R1: %f R2: %f\n", J1, J2, h, R1, R2);
					printf("Final State of run %d:\n\n", l);
					table_disp(table);
					calc_corr(table);
					calc_joint_entropy(table);
					calc_mut_info(table);
					for(int m = 0; m < 15; m++)
					{
						corravg[m] += corr[m];
						jentropyavg[m] += jentropy[m];
						mut_infoavg[m] += mut_info[m];
					}
				}
				for(int l = 0; l < 15;l++)
				{	
					corravg[l] /= 3;
					jentropyavg[l] /= 3;
					mut_infoavg[l] /= 3;
					corr[l] = corravg[l];
					jentropy[l] = jentropyavg[l];
					mut_info[l] = mut_infoavg[l];
				}
				
				for(int l = 0; l < 15; l++)
				{
					fprintf(csv,"%d,%f,%f,%f\n", l, corr[l], jentropy[l], mut_info[l]);
				}
				char * name = new char;
				sprintf(name, "%d%d%d",(int)h,(int)R1,(int)R2);
				create_pgm(table,name);
				count++;

			}
		}
	}
	fclose(csv);
}
