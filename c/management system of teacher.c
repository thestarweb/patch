#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct teacher
{
    long int num;
    char name[20];
    int sco[3];
    int sum;
    int mc;
    char grade[8];
    struct teacher *next;
};

struct teacher *head;//链表头节点

//增加一个教师信息
struct teacher* _add(int num,char name[],int sco1,int sco2,int sco3)
{
    struct teacher *p=(struct teacher*)malloc(sizeof(struct teacher)),*q;
    int mc;
    p->next=NULL;
    p->num=num;
    strcpy(p->name,name);
    p->sco[0]=sco1;
    p->sco[1]=sco2;
    p->sco[2]=sco3;
    p->sum=sco1+sco2+sco3;//顺便计算总分
    //进行评分
    if(p->sum/3 >= 85) strcpy(p->grade,"优秀");
    else if(p->sum/3 >= 60) strcpy(p->grade,"合格");
    else strcpy(p->grade,"不合格");
    q=head;
    mc=0;
    //按评分排序找到位置并记录下名次
    while(q->next!=NULL&&q->next->sum > p->sum)
    {
        mc++;
        q=q->next;
    }

    //将新节点加入进去
    p->next=q->next;
    q->next=p;
    //计算当前名次并修改后面人的名次
    while(q->next!=NULL)
    {
        mc++;
        if(q->next->sum==q->sum) q->next->mc=q->mc;//计算当前名次并修改后面人的名次
        else q->next->mc=mc;//如果不一样，则为使用记录下的排名
        q=q->next;
    }
    return p;
}


//删除一个教师的信息
void _remove(struct teacher *p)
{
    struct teacher *q;
    int mc=0;
    q=head;
    //找到前一个节点
    while(q->next!=NULL)
    {
        if(q->next==p) break;
        q=q->next;
        mc++;
    }
    //删除节点
    q->next=q->next->next;
    //修改后面的名次
    while(q->next!=NULL)
    {
        mc++;
        if(q->next->sum==q->sum) q->next->mc=q->mc;
        else q->next->mc=mc;
        q=q->next;
    }
    free(p);//释放资源
}

//删除所有节点
void remove_all()
{
    while(head->next!=NULL)
    {
        _remove(head->next);
    }
}

//从文件读入
void Input()
{
    int num,sco1,sco2,sco3;
    char name[20];
    FILE* fp;
    remove_all();//首先移除之前所有项
    fp=fopen("D:\\1.txt","r");
    while(fscanf(fp,"%ld %s %d %d %d\n",&num,&name,&sco1,&sco2,&sco3)!=EOF)
    {
        _add(num,name,sco1,sco2,sco3);
    }
    fclose(fp);
    printf("\n\t\t文件读取成功\n");
}

//显示所有数据
void Disp()
{
    struct teacher *p;
    p=head;
    printf("┏━━┯━━━━━━━━━━┯━━┯━━┯━━┯━━┯━━┯━━━┓\n");
    printf("┃ id │        姓名        │sco1│sco2│sco2│总分│名次│ 等级 ┃\n");
    while((p=p->next)!=NULL)
    {
        printf("┠──┼──────────┼──┼──┼──┼──┼──┼───┨\n");
        printf("┃%4d│%-20s│%4d│%4d│%4d│%4d│%4d│%6s┃\n",p->num,p->name,p->sco[0],p->sco[1],p->sco[2],p->sum,p->mc,p->grade);
    }
    printf("┗━━┷━━━━━━━━━━┷━━┷━━┷━━┷━━┷━━┷━━━┛\n");
}


//显示一个教师的信息
void Disp_one(struct teacher *p)
{
    printf("┏━━┯━━━━━━━━━━┯━━┯━━┯━━┯━━┯━━┯━━━┓\n");
    printf("┃ id │        姓名        │sco1│sco2│sco2│总分│名次│ 等级 ┃\n");
    printf("┠──┼──────────┼──┼──┼──┼──┼──┼───┨\n");
    printf("┃%4d│%-20s│%4d│%4d│%4d│%4d│%4d│%6s┃\n",p->num,p->name,p->sco[0],p->sco[1],p->sco[2],p->sum,p->mc,p->grade);
    printf("┗━━┷━━━━━━━━━━┷━━┷━━┷━━┷━━┷━━┷━━━┛\n");
}

//新增
void Append()
{
    int num,sco1,sco2,sco3;
    char name[20];
    printf("\n\n\n\t新增:\n\n");
    printf("\t请输入id:");
    scanf("%d",&num);
    printf("\t请输入姓名:");
    scanf("%s",&name);
    printf("\t请输入sco1:");
    scanf("%d",&sco1);
    printf("\t请输入sco2:");
    scanf("%d",&sco2);
    printf("\t请输入sco3:");
    scanf("%d",&sco3);
    printf("\n\n插入成功 结果如下:\n");
    Disp_one(_add(num,name,sco1,sco2,sco3));
}


//查找教师的基础方法
struct teacher* Found()
{
    int input,id;
    char name[20];
    struct teacher *p;
    while(1)
    {
        printf("\t请选择条件\n\t\t1、id\t2、姓名\t0、取消\n\t\t");
        scanf("%d",&input);
        p=head;
        switch(input)
        {
            case 1:
                printf("\t\t请输入id:");
                scanf("%d",&id);
                while((p=p->next)!=NULL)
                {
                    if(p->num==id) return p;
                }
                printf("\t\t没有找到id为%d的老师，请从新选择条件\n",id);
                break;
            case 2:
                printf("\t\t请输入教师姓名：");
                scanf("%s",&name);
                while((p=p->next)!=NULL)
                {
                    if(strcmp(p->name,name)==0) return p;
                }
                printf("\t\t没有找到姓名为%s的老师，请从新选择条件\n",name);
            break;
            case 0:
                return NULL;
            default:
                printf("\t\t请输入正确的选项！\n");
        }
    }
}

//删除
void Remove()
{
    int ok;
    struct teacher *p;
    printf("\n\n\n\t删除:\n\n");
    while(1)
    {
        p=Found();
        if(p==NULL)
        {
            printf("\t\t删除已取消\n");
            return;
        }
        else
        {
            printf("\t\t老师信息如下，请确认：\n");
            Disp_one(p);
            printf("\t\t输入1确认删除，输入0取消：");
            scanf("%d",&ok);
            if(ok==1){
                _remove(p);
                printf("\n\n");
                printf("\t\t删除成功！你可以继续删除或取消\n\n");
            }
        }
    }
}



//修改

void Modify()
{
    int type,mc,ss;
    struct teacher *p,*q,*t;
    printf("\n\n\n\t修改:\n\n");
    while(1)
    {
        p=Found();
        if(p==NULL)
        {
            printf("\t\t删除已取消\n");
            return;
        }
        else
        {
            printf("\n\t\t老师信息如下，请确认：\n");
            Disp_one(p);
            printf("\t\t1、修改sco1\n\t\t2、修改sco2\n\t\t3、修改sco3\n\t\t0、取消\n\t\t请输入选项:");
            scanf("%d",&type);
            switch(type)
            {
                case 1:
                case 2:
                case 3:
                    printf("\t\t请输入修改后的值：");
                    while(1){
                        scanf("%d",p->sco+type-1);
                        if(*(p->sco+type-1)>=100) printf("\t\t输入的值不应超过100，");
                        else if(*(p->sco+type-1)<=0) printf("\t\t输入的值小于0，");
                        else break;
                        printf("请重新输入：");
                    }
                    //从新计算
                    p->sum=p->sco[0]+p->sco[1]+p->sco[2];
                    if(p->sum/3 >= 85) strcpy(p->grade,"优秀");
                    else if(p->sum/3 >= 60) strcpy(p->grade,"合格");
                    else strcpy(p->grade,"不合格");
                    ss=0;
                    q=head;
                    mc=0;
                    while(q->next)
                    {
                        if((ss%10)==0&&q->next==p)//从原来的位置移除
                        {
                            if(ss/10==0)q->next=q->next->next;
                            else q->next=t;//如果节点已经被放到新的位置，那原来的next已经发生改变，需要通过t找到原来的后一位节点
                            ss+=1;
                            continue;
                        }
                        mc++;
                        if(ss/10==0&&q->next->sum < p->sum)//插入到新的位置
                        {
                            //printf("%d %d\n",q->next->mc , p->mc);
                            t=p->next;
                            p->next=q->next;
                            q->next=p;
                            ss+=10;
                        }
                        if(ss!=0)//链表发生变化了 需要修改排名信息
                        {
                            if(q->sum==q->next->sum) q->next->mc=q->mc;
                            else q->next->mc=mc;
                        }
                        q=q->next;
                    }
                    if(ss/10==0&&q->next==NULL)
                    {
                        q->next=p;
                        p->next=NULL;
                        p->mc=(q->sum==p->sum)?q->mc:(mc+1);
                    }
                    printf("\n\n\t\t修改成功！结果如下：\n");
                    Disp_one(p);
                    printf("\n\t\t现在你可以继续修改或取消\n");
                    break;
                case 0:
                    return;
                default:
                    printf("\t\t请输入正确的选项！\n");
            }
        }
    }
}



//查询

void Query()
{
    struct teacher *p;
    printf("\n\n\n\t查询:\n\n");
    while(1)
    {
        p=Found();
        if(p==NULL)
        {
            printf("\t\t查询已取消\n");
            return;
        }
        else
        {
            printf("\n\n\t\t信息已找到\n");
            Disp_one(p);
            printf("\n\t\t你可以继续查询或取消\n");
        }
    }
}



void main()
{
    int choice;
    head=(struct teacher*)malloc(sizeof(struct teacher));
    head->next=NULL;
    head->mc=0;
    Input();
    while(1)
    {
        printf("\n\t\t主菜单:\n\n");
        printf("\t\t\t1.重新读入数据\n");
        printf("\t\t\t2.显示所有教师信息\n");
        printf("\t\t\t3.删除教师的信息\n");
        printf("\t\t\t4.新增教师的信息\n");
        printf("\t\t\t5.查询教师的信息\n");
        printf("\t\t\t6.修改教师的信息\n");
        printf("\t\t\t0.退出系统\n\n");
        printf("\t\t\t请选择功能(0~6)");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:Input();break;
            case 2:Disp();break;
            case 3:Remove();break;
            case 4:Append();break;
            case 5:Query();break;
            case 6:Modify();break;
            case 0:
                remove_all();//内存回收
                free(head);
                exit(0);
        }
    }
}
