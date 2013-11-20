#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#define CH1_TS 5
#define CH2_TS 5
#define CH3_TS 5

using namespace std;

class PCB{
	
	public :
	int Job_id;
	int TTL,TTC,LLC,TLL,ptr;
	int datacount;	
	vector<int> CodePtr,DataPtr,OutputPtr;
	PCB(){
		
		}
	PCB(string card){
		cout<<"in PCB\n"<<card<<"\n";
		Job_id = atoi(card.substr(4,4).c_str());
		TTL = atoi(card.substr(8,4).c_str());
		TLL = atoi(card.substr(12,4).c_str());
		TTC=0;
		datacount=0;
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
			if(!EmptyBuffers.empty()){
			t=EmptyBuffers.front();
			EmptyBuffers.pop();
			return t;
			}
			else 
			return -1;
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
			for(int i=0;i<100;i++)
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


int no_of_pages=0;

void clearbuffer(char buffer[][40],int n);

void loadInMain(PCB *pcb){
		int n;
		bool code;
		char temp[40],ch;
		int b=0;
		int k=-1,i=0,j,currP=0,x=0,l,uptr,m,currentPage;
   		bool new_block = true,flag=false;
			pcb->ptr=-1;
			code = !pcb->CodePtr.empty();
			while(!pcb->CodePtr.empty() || !pcb->DataPtr.empty())
			{
				for (int q = 0; q < 40; ++q)
					temp[q]='\0';
				
				if(!pcb->CodePtr.empty())
				{
					strcpy(temp,c.dm.drum[pcb->CodePtr[0]]);
					pcb->CodePtr.erase(pcb->CodePtr.begin());		
				}
				else 
				{	
					if(!pcb->DataPtr.empty())
					{	
						strcpy(temp,c.dm.drum[pcb->DataPtr[0]]);	
						pcb->DataPtr.erase(pcb->DataPtr.begin());
					}
					else
						return;
				}
				if (pcb->ptr ==-1)
				{
					pcb->ptr = rand() % 30;
					if(pcb->ptr==-1){
						exit(0);
					}
					uptr = pcb->ptr*10;
					for(int n=0;n<10;n++)
						for(int o=0;o<4;o++)
							c.Mem[uptr+n][o]='#';
				}
				b=-1;
				k=-1;
				i=0;
				while(b!=40)
				{

					b++;
					ch=temp[b];
				
					if(isprint(ch))
				  	{
						if(i%4==0){
							k++;
							if(k%10==0)
								flag=true;
						}
							if(flag)
							{
								flag = false;
								k=0;
								while(1)
								{
									currentPage = rand() %30;
									for(j=0;j<c.v.size();j++)
									{
										if(c.v[j]==currentPage)
											break;
									}
										if(j>=c.v.size())
										break;
								}
									no_of_pages++;
									c.v.push_back(currentPage);	
							}
							
							c.Mem[pcb->ptr*10+no_of_pages-1][0] = '0'+currentPage/10;
							c.Mem[pcb->ptr*10+no_of_pages-1][1] = '0'+currentPage%10;
						
						  c.Mem[currentPage*10+k][i%4] = ch;
						  i++;
				
				  	}
				  	else
				  		break;
			  	}
			}
		
		
	}

int PD_function(int block_no,PCB *pcb){
		char ch;
		int t=c.dm.allocateTrack();
		pcb->OutputPtr.push_back(t);
		pcb->LLC++;
		if(pcb->LLC>pcb->TLL){
			 return 2;
			}
		
		for(int i=0;i<10;i++)
		for(int j=0;j<4;j++){
			ch=c.Mem[block_no+i][j];		
			c.dm.drum[t][i*10+j] = ch;
		}       
		return 0;
	}

int GD_function(int block_no,PCB *pcb){
	
		int k=0;
		char line[41];
		if(pcb->datacount>=pcb->DataPtr.size()){
			 return 1;
			}
		strcpy(line,c.dm.drum[pcb->DataPtr[pcb->datacount]]);
		pcb->datacount++;
		for(int i=0;i<10;i++)
		for(int j=0;j<4;j++){
			if(k>=strlen(line))
				break;
			c.Mem[block_no+i][j]=line[k++];
		}       
		return 0;
	}
void channel1IR(){
		
		for(int i=0;i<CH1_TS;i++){
		if(c.inputCard.eof())
			return;
		int buff_no = c.sm.allotEmptyBuffer();
		clearbuffer(c.sm.buffer,buff_no);
		if(buff_no==-1)
			return;
		c.inputCard.getline(c.sm.buffer[buff_no],41);
		cout<<buff_no<<" "<<c.sm.buffer[buff_no]<<"\n";
		c.sm.addInputBuffer(buff_no);
		}
		
	}

void printmsg(char str[][40],int n)
{
	ofstream linePrinter;
    char ch;
    linePrinter.open("LinePrinter.txt",ios::app);
    linePrinter.put('\n');
    for(int i=0;i<strlen(str[n]);i++)
		   linePrinter.put(str[n][i]);	

	linePrinter.close();
}

void clearbuffer(char buffer[][40],int n)
{
	for (int i = 0; i < strlen(buffer[n]); ++i)
	{
		buffer[n][i] = '\0';
	}
}
void channel2IR()
{
	int temp;
	for(int i=0;i<CH1_TS;i++){
			if(!c.sm.OutputFullBuffers.empty())
			{
					temp = c.sm.OutputFullBuffers.front();
					printmsg(c.sm.buffer,temp);
			}
			clearbuffer(c.sm.buffer,temp);
			c.sm.OutputFullBuffers.pop();
			c.sm.EmptyBuffers.push(temp);
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
										
									}
								else if (card.find("$END")!=-1){

										loadInMain(ptr);

										return;
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
		ofstream o;
		o.open("Mem.txt");
		for (int i = 0; i < 10; ++i)
		{
			clearbuffer(c.sm.buffer,i);
		}
		c.inputCard.open("ip.txt");
		for(int i=0;i<10;i++){
		channel1IR();
		c.TASK = string("IS");
		channel3IR();
	}
		cout<<"size:"<<c.loadQ.front()->DataPtr.size();
		for(int i=0;i<c.loadQ.front()->DataPtr.size();i++)
		{
			cout<<c.dm.drum[c.loadQ.front()->DataPtr[i]]<<endl;
			
			}
	
		for(int i=0;i<300;i++){
		for(int j=0;j<4;j++)
				o<< c.Mem[i][j]<<"";
	    o<<endl;

	}
		
	}
