#include<iostream>
#include<vector>
#include<queue>
#include<deque>
#include<string>
#include<fstream>
#include<cstring>
#include<cstdlib>
#define CH1_TS 5
#define CH2_TS 5
#define CH3_TS 30

using namespace std;

class PCB{
	
	public :
	int Job_id;
	int TTL,TTC,LLC,TLL;	
	vector<int> CodePtr,DataPtr;
	PCB(){
		
		}
	PCB(string card){
		cout<<"in PCB\n"<<card<<"\n";
		Job_id = atoi(card.substr(4,4).c_str());
		TTL = atoi(card.substr(8,4).c_str());
		TLL = atoi(card.substr(12,4).c_str());
		TTC=0;
		TLL=0;		
		CodePtr.clear();
		DataPtr.clear();
		}
		
		
	};

PCB temp,*ptr;
class SuperVisiorMem{

 
	public:
	queue<int> EmptyBuffers;
	queue<int> InputFullBuffers;
	queue<int> OutputFullBuffers;
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

	int returnIFB(){
			if(!InputFullBuffers.empty()){
				int t;
			t=InputFullBuffers.front();
			InputFullBuffers.pop();
			return t;
				}
			return -1;
		}

};

class DrumMem{
	
	public :
	char drum[1000][41];
	queue<int> EmptyTracks;
	DrumMem(){
			drum[0][0] = 0;
			for(int i=0;i<1000;i++)
				EmptyTracks.push(i);
		}
	
	int allocateTrack(){
			int t;
			t=EmptyTracks.front();
			EmptyTracks.pop();
			return t;
		}
	
	void FreeTrack(int tno){
			EmptyTracks.push(tno);
		}
	
	
	};

struct CPU
{
    char IR[4];
    char R[4];
    bool T;
    int SI,PI,TI;
    char IC[2];
    char Mem[300][4];
    SuperVisiorMem sm;
    DrumMem dm;
    char error[100];
    ifstream inputCard;
    deque<PCB*> loadQ,readyQ;
    vector<int> v;
    string TASK;
}c;


void channel1IR(){
		
		for(int i=0;i<CH1_TS;i++){
		int buff_no = c.sm.allotEmptyBuffer();
		c.inputCard.getline(c.sm.buffer[buff_no],41);
		cout<<buff_no<<" "<<c.sm.buffer[buff_no]<<"\n";
		c.sm.addInputBuffer(buff_no);
		}
		
	}
void channel3IR(){
		bool code;
		for(int i=0;i<CH3_TS;i++){
					//cout<<"here";
				if(c.TASK.compare("IS")==0){
					
					if(!c.sm.InputFullBuffers.empty())
							{
								string card;
								int k;
								char buffer[41];
								int bno = c.sm.returnIFB();
								for(k=0;c.sm.buffer[bno][k]&&k<40;k++){
									buffer[k]=c.sm.buffer[bno][k];
									}
								buffer[k]='\0';
								card = string(buffer);
								cout<<card<<endl;
								c.sm.addEmptyBuffer(bno);
								if(card.find("$AMJ")!=-1){
										temp = PCB(card);
										ptr=&temp;
										c.loadQ.push_back(ptr);
										code = true;
									}
								else if(card.find("$DTA")!=-1){
										code =false;
										return;
									}
								else if (card.find("$END")!=-1){
									
									}
								else{
										PCB *pcb = c.loadQ.front();
										//c.loadQ.pop_front();
										int t = c.dm.allocateTrack();
										strcpy(c.dm.drum[t],card.c_str());
										if(code)						
											pcb->CodePtr.push_back(t);
										else
											pcb->DataPtr.push_back(t);
										//c.loadQ.push_front(pcb);
									}
							}
					
					
					
					}
			
			
			}
		
		
	}
int main(){
		c.inputCard.open("ip.txt");
		channel1IR();
		c.TASK = string("IS");
		channel3IR();
		cout<<c.loadQ.front()->CodePtr.size();
		for(int i=0;i<c.loadQ.front()->CodePtr.size();i++)
		{
			cout<<c.dm.drum[c.loadQ.front()->CodePtr[i]]<<endl;
			
			}
	}
