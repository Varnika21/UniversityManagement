//om
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*clg_id for OU is 20
clg_id for VCE is 21
clg_id for CBIT is 22
clg_id for MGIT is 23*/
//Branch codes: cse-1,ece-2,it-3,eee-4,mech-5,civil-6
struct student
{
    int roll_no;
    char name[50];
    float cgpa;
    float attendance_percentage;
    int fee_payment;
    struct student *left,*right;
}*OU,*VCE,*CBIT,*MGIT,*clgs;
struct student_temp
{
    int roll_no;
    char name[50];
    float cgpa;
    float attendance_percentage;
    int fee_payment;
}temp[25];
struct student* NewNode(int roll_no,char name[50],float cgpa,float ap,int fp)
{
    struct student *temp = (struct student*)malloc(sizeof(struct student));
    temp->roll_no=roll_no;
    strcpy(temp->name,name);
    temp->cgpa=cgpa;
    temp->attendance_percentage=ap;
    temp->fee_payment=fp;
    temp->right = temp->left =NULL;
    return temp;
}
int branch_rollno[6]={0},count=0,temp12=0;
float cgpa_array[50];
int roll_array[50];
struct student* insert_into_tree(struct student *Node,int roll_no,char name[50],float cgpa,float ap,int fp)
{
    if(Node==NULL)
    {
        return NewNode(roll_no,name,cgpa,ap,fp);
    }
    if(roll_no<Node->roll_no)
        Node->left = insert_into_tree(Node->left,roll_no,name,cgpa,ap,fp);
    else if(roll_no>Node->roll_no)
        Node->right = insert_into_tree(Node->right,roll_no,name,cgpa,ap,fp);
}
struct student* construct_tree(struct student* Node,char filename[10])
{
    int roll_no,fp,temp_branch;
    char name[50],c;
    float ap,cgpa;
    FILE *fptr;
    fptr=fopen(filename,"r");
    if(fptr==NULL)
        printf("File is not created\n");
    else{
        for (c =getc(fptr); c!= EOF; c =getc(fptr))
        {
            if (c =='\n')
            count = count + 1;
        }
        fclose(fptr);
        fptr=fopen(filename,"r");
        for(int i=temp12;i<count;i++)
        {
        fscanf(fptr,"%d %s %f %f %d",&roll_no,name,&cgpa,&ap,&fp);
        temp_branch=(roll_no/100)%10;
        branch_rollno[temp_branch-1]+=1;
        cgpa_array[i]=cgpa;
        roll_array[i]=roll_no;
        if(Node==NULL)
        {
            Node = insert_into_tree(Node,roll_no,name,cgpa,ap,fp);
        }
        else
        {
            insert_into_tree(Node,roll_no,name,cgpa,ap,fp);
        }
        }
        fclose(fptr);
        return Node;
    }
}
struct student* create_new(struct student* Node,int roll_no,char name[20])
{
    if(Node==NULL)
    {
        return NewNode(roll_no,name,0,0,0);
    }
    if(roll_no<Node->roll_no)
    {
        Node->left = create_new(Node->left,roll_no,name);
    }
    else if(roll_no>Node->roll_no)
    {
        Node->right = create_new(Node->right,roll_no,name);
    }
}
void entry(struct student *Node,int clg_code,char filename[20])
{
    char name[50];
    int branch;
    printf("Enter your name\n");
    scanf("\n%s",name);
    printf("Enter the branch code(1-CSE,2-ECE,3-IT,4-EEE,5-Mech,6-Civil)\n");
    scanf("%d",&branch);
    branch_rollno[branch-1]+=1;
    int roll_no = (clg_code*1000)+(branch*100)+branch_rollno[branch-1];
    printf("Your Roll No is %d\n",roll_no);
    create_new(Node,roll_no,name);
    FILE *fptr;
    fptr=fopen(filename,"a");
    fprintf(fptr,"\n%d %s 0 0 0",roll_no,name);
    fclose(fptr);
}
void swap(float *a, float *b)
{
    float t=*a;
    *a=*b;
    *b=t;
}
int partition (float part[], int low, int high)
{
    float pivot= part[high];
    int i = (low - 1);
    int j;
    for (j = low; j <= high- 1; j++)
    {
        if (part[j] < pivot)
        {
            i++;
            swap(&part[i], &part[j]);
        }
    }
    swap(&part[i + 1], &part[high]);
    return (i + 1);
}
void quickSort(float arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void sort_cgpa()
{
    quickSort(cgpa_array,0,count);
}
int flag=0;
int details(struct student *temp,int roll_no)
{
    if(temp != NULL)
    {
    flag+=1;
    details(temp->left,roll_no);
    if(roll_no==temp->roll_no)
        {
        printf("Name : %s\nCGPA : %0.1f\nAttendance_percentage : %0.2f\n",temp->name,temp->cgpa,temp->attendance_percentage);
        char choice1;
        for(int k=0;k<count;k++)
        {
            if(temp->cgpa==cgpa_array[k])
            {
                printf("Rank : %d\n",count-k+1);
                break;
            }
        }
        if(temp->fee_payment==1)
            printf("Fee Payment Status : Paid\n");
        else
            printf("Fee Payment Status : Not Paid\n");
        if(65<=temp->attendance_percentage && temp->attendance_percentage<75)
        {
            printf("Do you have a medical certificate(y/n)\n");
            scanf("\n%c",&choice1);
            if(choice1=='y')
            printf("Submit your medical certificate\n");
            else
            {
                printf("You are detained\n");
            }
        }
        else if(temp->attendance_percentage!=0 && temp->attendance_percentage<65.00)
        printf("You are detained\n");
        return 1;
        }
    details(temp->right,roll_no);
    }
    if(flag==count)
    return 0;
}
int flag_e=0;
int register_exam(struct student *temp,int roll_no,char filename[20])
{
    if(temp != NULL)
    {
    flag_e+=1;
    register_exam(temp->left,roll_no,filename);
    if(roll_no==temp->roll_no)
        {
        if(temp->fee_payment==1)
            printf("Already Paid\n");
        else
        {
            int c,rollno,fp;
            char choice[3];
            printf("Fee is not Paid\n");
            printf("Do you want to pay(yes/no)\n");
            scanf("%s",choice);
            if(!strcmp(choice,"no"))
                printf("OK...Next Time\n");
            else
            {
                printf("Make Payment through UPay\n");
                temp->fee_payment=1;
                FILE *fptr;
                fptr=fopen(filename,"r+");
                while(fptr!=EOF)
                {
                    fscanf(fptr,"%d %*s %*f %*f %d",&rollno,&fp);
                    if(rollno==roll_no)
                    {
                        fseek(fptr,-1,SEEK_CUR);
                        c=fprintf(fptr,"%d",temp->fee_payment);
                        break;
                    }
                }
                if(c==1)
                printf("Payment successful\n");
                else
                printf("Payment unsuccessful\n");
                printf("THANK YOU\n");
            }
        }
        return 1;
        }
    register_exam(temp->right,roll_no,filename);
    }
    if(flag_e==count)
    return 0;
}
struct student* minValueNode(struct student* node)
{
	struct student* current=node;
	while(current&& current->left!=NULL)
		current=current->left;
	return current;
}
struct student* deleteNode(struct student* root, int key)
{
	if (root == NULL)
		return root;
	if (key < root->roll_no)
		root->left = deleteNode(root->left, key);
	else if (key > root->roll_no)
		root->right = deleteNode(root->right, key);
	else
	{
		if (root->left == NULL)
		{
			struct student *temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			struct student *temp = root->left;
			free(root);
			return temp;
		}
		struct student* temp = minValueNode(root->right);
		root->roll_no = temp->roll_no;
		strcpy(root->name,temp->name);
		root->cgpa=temp->cgpa;
		root->attendance_percentage=temp->attendance_percentage;
		root->fee_payment=temp->fee_payment;
		root->right = deleteNode(root->right, temp->roll_no);
	}
	return root;
}
int flag_q=0;
int quit(struct student *node,int roll_no,char filename[20])
{
    if(node != NULL)
    {
        flag_q+=1;
    quit(node->left,roll_no,filename);
    if(roll_no==node->roll_no)
        {
            deleteNode(node,roll_no);
            int i=-1,rollno,fee_payment;
            float attendance_percentage,cgpa;
            char name[50];
            FILE *fptr;
            fptr=fopen(filename,"r+");
            for(int j=0;j<count+1;j++)
                {
                    fscanf(fptr,"%d %s %f %f %d",&rollno,name,&cgpa,&attendance_percentage,&fee_payment);
                    if(rollno!=roll_no)
                    {
                        i++;
                        temp[i].roll_no = rollno;
                        strcpy(temp[i].name,name);
                        temp[i].cgpa=cgpa;
                        temp[i].attendance_percentage=attendance_percentage;
                        temp[i].fee_payment=fee_payment;
                    }
                }
                fclose(fptr);
                int k=0;
                fptr = freopen(filename,"w",stderr);
                while(k<=i)
                {
                    if(k<i)
                    {
                        fprintf(fptr,"%d %s %0.1f %.2f %d\n",temp[k].roll_no,temp[k].name,temp[k].cgpa,temp[k].attendance_percentage,temp[k].fee_payment);
                    }
                    else
                    {
                        fprintf(fptr,"%d %s %0.1f %.2f %d",temp[k].roll_no,temp[k].name,temp[k].cgpa,temp[k].attendance_percentage,temp[k].fee_payment);
                    }
                    k++;
                }
                fclose(fptr);
                printf("Record Deletion is successful\n");
                printf("THANK YOU\n");
                return 1;
        }
    quit(node->right,roll_no,filename);
    }
    if(flag_q==count)
        return 0;
}
void display(struct student *Node,int clg_code,char filename[20])
{
    int choice;
    while(1)
    {
    printf("Click 1 to register and join\n      2 to get your academic details\n      3 to register for exam\n      4 to quit the college\n      5 to exit the portal\n");
    scanf("%d",&choice);
    switch(choice)
    {
    case(1):
        {
            entry(Node,clg_code,filename);
            break;
        }
    case(2):
        {
            sort_cgpa();
            int roll_no,flag1;
            printf("Enter your roll number\n");
            scanf("%d",&roll_no);
            flag=0;
            flag1=details(Node,roll_no);
            if(flag1==0)
            printf("Record not found\n");
            break;
        }
    case(3):
        {
            int roll_no,flag_exam;
            printf("Enter your roll number\n");
            scanf("%d",&roll_no);
            flag_e=0;
            flag_exam=register_exam(Node,roll_no,filename);
            if(flag_exam==0)
            printf("Record not found\n");
            break;
        }
    case(4):
        {
            int roll_no,flag_quit;
            printf("Enter your roll number\n");
            scanf("%d",&roll_no);
            flag_q=0;
            flag_quit=quit(Node,roll_no,filename);
            if(flag_quit==0)
                printf("Record not found\n");
            break;
        }
    case(5):
        {
            printf("HAPPY LEARNING\n");
            exit(1);
        }
    default:
        {
            printf("NOT APPLICABLE\n");
        }
    }
    }
}
void inorder(struct student* node)
{
    if(node == NULL)
        return;
    inorder(node->left);
    printf("%d ",node->roll_no);
    inorder(node->right);
}
void main()
{
    printf("-----WELCOME TO OSMANIA UNIVERSITY-----\n");
    printf("THE COLLEGES UNDER OSMANIA UNIVERSITY ARE:\n    1.VCE\n    2.CBIT\n    3.MGIT\n");
    int i,j,temp123;
    float temp1234;
    temp12=count;
    clgs = construct_tree(clgs,"OU.txt");
    temp12=count;
    construct_tree(clgs,"VCE.txt");
    temp12=count;
    construct_tree(clgs,"CBIT.txt");
    temp12=count;
    construct_tree(clgs,"MGIT.txt");
    for(i=0;i<6;i++)
    branch_rollno[i]=0;
    for(i=0;i<count;i++)
    {
        for(j=i;j<count;j++)
        {
            if(cgpa_array[i]<cgpa_array[j])
            {
                temp1234=cgpa_array[i];
                cgpa_array[i]=cgpa_array[j];
                cgpa_array[j]=temp1234;
                temp123=roll_array[i];
                roll_array[i]=roll_array[j];
                roll_array[j]=temp123;
            }
        }
    }
    printf("Top 10 Ranks of Osmania University\n");
    for(i=0;i<10;i++)
    {
        printf("%d.%d : %.2f\n",i+1,roll_array[i],cgpa_array[i]);
    }
    count=0;
    temp12=0;
    //inorder(clgs);
    int choice;
    printf("Click 1 to navigate to OU\n      2 to navigate to VCE\n      3 to navigate to CBIT\n      4 to navigate to MGIT\n      5 to exit\n");
    scanf("%d",&choice);
    switch(choice)
    {
    case(1):
        {
            OU=construct_tree(OU,"OU.txt");
            display(OU,20,"OU.txt");
            break;
        }
    case(2):
        {
            VCE=construct_tree(VCE,"VCE.txt");
            display(VCE,21,"VCE.txt");
            break;
        }
    case(3):
        {
            CBIT=construct_tree(CBIT,"CBIT.txt");
            display(CBIT,22,"CBIT.txt");
            break;
        }
    case(4):
        {
            MGIT=construct_tree(MGIT,"MGIT.txt");
            display(MGIT,23,"MGIT.txt");
            break;
        }
    default:
        {
            printf("NOT APPLICABLE\n");
            exit(1);
        }
    }
}

