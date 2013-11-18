#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<fstream>

#define CH1_TS 5
#define CH2_TS 5
#define CH3_TS 2

using namespace std;

struct program_control_block
{
	int Job_id,ptr, LLC, TTC, TTL, TLL;

}pcb;

class SuperVisiorMem{

 queue<int> EmptyBuffers;
 queue<int> InputFullBuffers;
 queue<int> OutputFullBuffers;
	public:
	char buffer[10][40];
	
	SuperVisiorMem(){
		 buffer[0][0] = 0;
		 for(int i=0;i<10;i++)
				EmptyBuffers.push(i);
		}
	
	int allotEmptyBuffer(){
			int t;
			t=EmptyBuffers.front();
			EmptyBuffers.pop();
			return t;
		} 
	
	void addInputBuffer(int buff_no){
			InputFullBuffers.push(buff_no);
		} 
	
	void addOutputBuffer(int buff_no){
			OutputFullBuffers.push(buff_no);
		} 
	void addEmptyBuffer(int buff_no){
			EmptyBuffers.push(buff_no);
		} 


};

class DrumMem{
	
	public :
	char drum[1000][40];
	
	
	
	}

struct CPU
{
    char IR[4];
    char R[4];
    bool T;
    int SI,PI,TI;
    char IC[2];
    char Mem[300][4];
    SuperVisiorMem sm;
    char error[100];
    ifstream inputCard;
    vector<int> v;
    string TASK;
}c;


void channel1IR(){
		
		for(int i=0;i<CH1_TS;i++){
		int buff_no = c.sm.allotEmptyBuffer();
		c.inputCard.getline(c.sm.buffer[buff_no],40);
		c.sm.addInputBuffer(buff_no);
		}
		//cout<<c.sm.buffer[buff_no];
	}
void channel3IR(){
	
		for(int i=0;i<CH3_TS;i++){
					
				switch(c.TASK.c_str()){
					case "IS" :
						
					
					
					
					}
			
			
			}
		
		
	}
int main(){
		c.inputCard.open("ip.txt");
		channel1IR();
	
	
	}
