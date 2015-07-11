
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace std;


/** maximum number of works**/
const int MAX_WORK = 8;
const int MAX_PRIORITY = 5;
const int MAX_ACHIEVE_TIME = 5;
const int MAX_PROCEEDING_TIME = 5;

class Job
{
    public:
        char *name;
        int proceedingTime;
        int priority;
        int achieveTime; // relative time

        Job *next;

        Job(void)
        {
            name = new char[30];
            proceedingTime = 0;
            achieveTime = 0;
            priority = 0;
            next = NULL;
        }

        // has next
        Job(char *name, int priority, int proceedingTime, int achieveTime,
                Job *next)
        {
            this->name = name;
            this->priority = priority;
            this->proceedingTime = proceedingTime;
            this->achieveTime = achieveTime;
            this->next = next;
        }

        // no next
        Job(char *name, int priority, int proceedingTime, int achieveTime)
        {
            this->name = name;
            this->priority = priority;
            this->proceedingTime = proceedingTime;
            this->achieveTime = achieveTime;
        }

        // copy of old
        Job(const Job &old)
        {
            this->name = new char[30];
            strcpy(this->name, old.name);
            this->proceedingTime = old.proceedingTime;
            this->achieveTime = old.achieveTime;
            this->priority = old.priority;
            this->next = old.next;
        }

        void output(void)
        {
            printf("Name: %8s | proceedingTime: %8d | achieveTime: %8d \n", this->name,
                    this->proceedingTime, this->achieveTime);
        }


        /** add new Job **/
        void addBack(Job *next)
        {
            if(this->next != NULL)
            {
                next = this->next;
                this->next = next;
            }
            else
            {
                this->next = next;
            }
        }

        Job getNext(void)
        {
            return *(this->next);
        }

        /** generate New Name **/
        void genNewName(char *name, int i)
        {
            char prefix[] = "Job_";
            strcpy(name, prefix);
            int len = strlen(name);
            while(i >= 10)
            {
                name[len++] = i / 10 + '0';
                i %= 10;
            }
            name[len++] = i + '0';
            name[len] = '\0';
        }

        /** Generate New Random Job **/
        void genRandJob(int i)
        {
            this -> proceedingTime  = (int) rand() % MAX_PROCEEDING_TIME + 1;
            this -> achieveTime     = (int) rand() % MAX_ACHIEVE_TIME;
            this -> priority        = (int) rand() % MAX_PRIORITY;
            this -> next            = NULL;
            genNewName(this->name, i);

            //cout << randProceedingTime << endl;
        }
};

class DispatchList
{
    public:
        Job list[MAX_WORK];
        Job *front, *rear;
        int workNum;
        int curTime;
        int curRunningTime;

        DispatchList()
        {
            front = rear = NULL;
            workNum = MAX_WORK;
            curTime = 0;
        }

        /** generate random work, sort by achievetime**/
        void genRandList()
        {
            puts("Generate Random List (outside memory):");
            for(int i = 0; i < MAX_WORK; i++) {
                list[i].genRandJob(i);
                list[i].output();
            }

            /** sort the randlist **/
            for(int i = 0; i < MAX_WORK; i ++)
            {
                for(int j = 0 ; j < MAX_WORK-i-1; j++)
                {
                    Job &a = list[j];
                    Job &b = list[j+1];
                    Job temp;
                    if(a.achieveTime > b.achieveTime)
                    {
                        temp = a;
                        a = b;
                        b = temp;
                    }
                    else if(a.achieveTime == b.achieveTime &&
                            a.proceedingTime > b.proceedingTime)
                    {
                        temp = a;
                        a = b;
                        b = temp;
                    }
                }
            }
        }

        // check rest work in dispatch list
        void checkList(int &finished)
        {
            puts("The dispatch list: ");

            if(this->curRunningTime < front->proceedingTime)
            {
                printf("* %s is running...\n", front->name);
            }

            else
            {
                printf("* %s is finished.\n", front->name);
                pop();
                if(empty())
                {
                    puts("No more jobs.");
                }
                else
                {
                    printf("* %s is running...\n", front->name);
                }
                curRunningTime = 0;
                finished++;

            }

            Job *p = front;
            while(p != NULL)
            {
                p->output();
                p = p->next;
            }
        }

        void baseOutput()
        {
            printf("\n\n\n");
            printf("[CurTime]: %d\n", curTime);
        }

        // While Running
        void Running()
        {
            int finished = 0;
            int ip = 0;      // not in dispatch list work;
            curTime = 0;
            curRunningTime = 0;

            while(finished != MAX_WORK)
            {
                baseOutput();

                if(ip < MAX_WORK)
                {
                    while(list[ip].achieveTime <= curTime)
                    {
                        push(new Job(list[ip]));
                        ip++;

                        if(ip >= MAX_WORK)
                        {
                            break;
                        }
                    }
                }

                curTime ++;
                if(empty())
                {
                    puts("Nothing in the dispatch list.");
                }
                else
                {
                    checkList(finished);
                    curRunningTime++;
                }

            }
        }

        void push(Job *next)
        {
            if(empty())
            {
                rear = front = next;
                front->next = NULL;
            }
            else
            {
                rear->next = next;
                rear = next;
            }
        }

        bool empty()
        {
            return rear == NULL;
        }

        bool pop()
        {
            if(front == NULL)
                return false;

            Job *temp = front;
            if(front == rear)
                front = rear = NULL;
            else
                front = front->next;

            delete(temp);
            return true;
        }

        Job top()
        {
            return *front;
        }

        int traversal()
        {
            if(front == NULL)
            {
                puts("Empty.");
                return 0;
            }
            Job *p = front;
            while(p != NULL)
            {
                p->output();
                p = p->next;
            }
            return 1;
        }

        //output rest works in random list
        void output()
        {
            puts("Random List:");
            for(int i = 0; i < MAX_WORK; i ++)
            {
                int &atime = list[i].achieveTime;
                int &ptime = list[i].proceedingTime;
                if(atime <= curTime && atime + ptime >= curTime)
                {
                    list[i].output();
                }
            }
        }

        // generate pool queue
        void genList()
        {
            genRandList();
            Job *next;
            for(int i = 0; i < MAX_WORK; i++)
            {
                next = new Job(list[i]);
                push(next);
            }

            puts("");

            puts("Pool queue: ");
            traversal();
        }
};


const int CHANNELS_NUM = 3;
class JCB_queue : public DispatchList
{
    public:
        DispatchList dlist;
        int curChannel;

        void push(Job *next)
        {
            if(rear == NULL)
            {
                front = rear = next;
            }
            else
            {
                rear->next = next;
                rear = next;
            }
            curChannel ++;
        }

        void baseOutput()
        {
            printf("\n\n\n");
            puts("---------------------------------------");
            printf("[CurTime]: %d\n", curTime);
            printf("[Channels_num]: %d\n", CHANNELS_NUM);
            puts("---------------------------------------");
        }

        void Running()
        {
            int finished = 0;
            curTime = 0;
            curRunningTime = 0;
            curChannel = 0;

            while(finished != MAX_WORK)
            {
                baseOutput();
                // pool list is not empty
                if(!dlist.empty() && curChannel != CHANNELS_NUM)
                {
                    //Job *head = NULL;

                    while(dlist.top().achieveTime <= curTime)
                    {
                        Job temp = dlist.top();
                        temp.next = NULL;
                        dlist.pop();

                        push(new Job(temp));

                        /**
                          if(head == NULL)
                          {
                          head = new Job(temp);
                          }
                          else
                          {
                          Job *p = head;
                          if(temp.achieveTime == p->achieveTime && temp.proceedingTime < p->proceedingTime)
                          {
                          temp.next = head;
                          head = new Job(temp);
                          continue;
                          }

                          while(p->next != NULL && temp.achieveTime >= p->next->achieveTime &&
                          temp.proceedingTime > p->next->proceedingTime)
                          {
                          p = p->next;
                          }
                          p->addBack(new Job(temp));
                          }
                         **/

                        if(dlist.empty() || curChannel >= CHANNELS_NUM)
                        {
                            break;
                        }
                    }


                    /*if(rear == NULL)
                      {
                      this->front = this->rear = head;
                      }
                      else
                      {
                      rear->next = head;
                      }
                      while(rear->next != NULL)
                      {
                      rear = rear->next;
                      }*/
                }

                curTime ++;

                if(empty())
                {
                    puts("Nothing in the dispatch list.");
                }
                else
                {
                    checkList(finished);
                    curRunningTime++;
                }

            }
        }

        void genList()
        {
            dlist.genList();
        }

        // check rest work in dispatch list
        void checkList(int &finished)
        {
            puts("The Ready list: ");

            if(this->curRunningTime < front->proceedingTime)
            {
                printf("* %s is running...\n", front->name);
            }

            else
            {
                printf("* %s is finished.\n", front->name);
                pop();
                if(empty())
                {
                    puts("No more jobs.");
                }
                else
                {
                    printf("* %s is running...\n", front->name);
                }
                curRunningTime = 0;
                curChannel--;
                finished++;

            }

            Job *p = front;
            while(p != NULL)
            {
                p->output();
                p = p->next;
            }

            puts("");
            puts("Pool List: ");
            dlist.traversal();
        }

};

class PCB
{
    public:
        char *name;
        PCB *next;
        int ptime; // proceeding time
        char status; // 'R', 'C'

        PCB()
        {
            name = new char[30];
            ptime = 0;
            status = 'R';
            next = NULL;
        }

        /** generate New Name **/
        void genNewName(char *name, int i)
        {
            char prefix[] = "Process_";
            strcpy(name, prefix);
            int len = strlen(name);
            while(i >= 10)
            {
                name[len++] = i / 10 + '0';
                i %= 10;
            }
            name[len++] = i + '0';
            name[len] = '\0';
        }

        void genRand(int i)
        {
            this -> ptime           = (int) rand() % MAX_PROCEEDING_TIME + 1;
            this -> next            = NULL;
            genNewName(this->name, i);
        }

        void output()
        {
            printf("Name: %s | proceedingTime: %d | status: %c\n",
                    name, ptime, status);
        }
};

const int MAX_PROCESS = 10;
class PCB_queue
{
    public:
        PCB *front, *rear;
        PCB list[MAX_PROCESS];
        int pcbNum;
        int curTime;

        PCB_queue()
        {
            front = rear = NULL;
        }

        void genRandList()
        {
            puts("Generate Random Process List:");
            for(int i = 0; i < MAX_PROCESS; i++) {
                list[i].genRand(i);
                list[i].output();
            }
        }

        void genList()
        {
            genRandList();
            puts("---------------------------------");
            puts("Process List:");
            for(int i = 0; i < MAX_PROCESS; i++)
            {
                push(new PCB(list[i]));
            }
        }

        void push(PCB *next)
        {
            if(rear == NULL)
            {
                front = rear = next;
            }
            else
            {
                rear->next = next;
                rear = next;
            }
        }

        void pop()
        {
            PCB *temp;
            temp = front;
            if(front == rear)
            {
                front = rear = NULL;
            }
            else
            {
                front = front->next;
            }
            delete(temp);
        }

        PCB *top()
        {
            return front;
        }

        void output()
        {
            if(rear == NULL)
            {
                puts("Empty.");
                return;
            }

            puts("PCB List: ");
            PCB *p = front;
            while(p != NULL)
            {
                p->output();
                p = p->next;
            }
        }

        void baseOutput()
        {
            printf("\n\n\n");
            printf("[CurTime]: %d\n", curTime);
        }

        void back()
        {
            PCB *p;
            p = front;
            if(front == rear)
            {
                front = rear = NULL;
            }
            else
            {
                front = front->next;
            }
            p->next = NULL;
            push(p);
        }

        void Running()
        {
            curTime = 0;
            PCB *p;
            while(rear != NULL)
            {
                baseOutput();
                p = top();
                p->ptime--;

                printf("* %s is Running...\n", p->name);
                if(p->ptime == 0)
                {
                    p->status = 'C';
                    printf("* %s is Finished.\n", p->name);
                }
                output();
                if(p->ptime == 0)
                {
                    pop();
                }
                else
                {
                    back();
                }
                curTime++;
            }

            puts("Finished.");
        }
};


int main()
{
    freopen("output", "w", stdout);

    DispatchList dlist;
    dlist.genRandList();
    dlist.Running();

    puts("-------------------------------------------------------------");

    JCB_queue jq;
    jq.genList();
    jq.Running();

    puts("-------------------------------------------------------------");

    PCB_queue plist;
    plist.genList();
    plist.output();
    plist.Running();

    return 0;
}


/* vim: fdm=indent
*/
