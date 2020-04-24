#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;
int M[2][50];
int turn = 0;


bool full()
{
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 50; k++)
		{
			if (M[i][k] == 0)
			{
				return false;
			}
		}
	}
	return true;	
}



void Make_Seat_Zero()
{
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 50; k++)
		{
			M[i][k] = 0;
		}
	}
}

void * Seat_Reserver(void *thread_num)
{
	int *tid = (int *) thread_num;
	//cout << "Thread " << *tid << " is working now.\n";
	//cout << " tid of Thread " << *tid << " is" << *tid << ".\n";
	//cout << " turn of Thread " << *tid << " is" << turn << ".\n";
	while ( !full() /*(*tid) == turn -1*/ )
	{
		while ( turn != (*tid) -1 );
		//cout << *tid << " working at " <<"CHECK1\n";
		int random_seat = (rand() % 100 +1);
		int a = random_seat % 2;
		int b;
		if(a==1) // if a is odd, meaning, they are at M[0][X]
		{
			a=0;
			b= (random_seat/2) ;
		}
		else // if a is even, meaning they are at M[1][X]
		{
			a=1;
			b= (random_seat/2) -1;
		}
		while (M [a][b] != 0  && !full() ) // if seat is reserved
		{
			//cout << "Agent " << *tid << " is trying to find new seat " <<"      CHECK2\n";
			random_seat = (rand() % 100 ) +1;
			// generating new a and b values
			a = random_seat % 2;
			if(a==1) // if a is odd, meaning, they are at M[0][X]
			{
				a=0;
				b= (random_seat/2) ;
			}
			else // if a is even, meaning they are at M[1][X]
			{
				a=1;
				b= (random_seat/2) -1;
			}
		}
		M[a][b] = *tid;
		cout << "Seat reservation for seat " << "["<< a <<"]" << "[" << b << "]\t(Seat number = "<< random_seat << " )" << "\thas been made by Agent " << *tid << "\n";
		turn++;
		turn = turn % 3;
		//cout << *tid << " working at " <<"CHECK3\n";
	}
	//cout << "Thread "<< *tid << " exits now.\n";
	//pthread_exit(NULL);
}



int main()
{

	cout << "Welcome! Reservation system is starting now!\n";
	int random_seat;
	Make_Seat_Zero();
	int thread_num1 = 1, thread_num2 = 2, thread_num3 = 3;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	cout << "CALLING THREAD 1\n";
	pthread_create(&thread1, NULL, Seat_Reserver, (void*)&thread_num1);
	cout << "CALLING THREAD 2\n";
	pthread_create(&thread2, NULL, Seat_Reserver, (void*)&thread_num2);
	cout << "CALLING THREAD 3\n";
	pthread_create(&thread3, NULL, Seat_Reserver, (void*)&thread_num3);

	while (!full());
	int count = 1;

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);


	//for (int i = 0; i < 2; i++) // writing to array to see the reservations
	//{
	//	for (int k = 0; k < 50; k++)
	//	{
	//		cout << "i = " << i <<"     k = " << k << "  \tAgent = " << M[i][k] << " count = " << count << "\n";
	//		count++;
	//	}
	//}
	cout << "Seat plan is below:\n";
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 50; k++)
		{
			cout << M[i][k] << " ";
		}
		cout << endl;
	}

	cout << "PROGRAM IS TERMINATING\n\n\n";
	return 0;
}