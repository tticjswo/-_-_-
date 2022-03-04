#include <stdio.h>
#define MAXSIZE 10000
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

void game(int , int , char ** , int ,int, char*);
void *thread_func(void *);

typedef struct THREAD{
	int m;
	int n;
	char **matrix;
	int start;
	int end;
	char *filename;
}THREAD;

void *thread_func(void *p){
	THREAD *pp = (THREAD *)p;
	printf("%ld\t",pthread_self());
	game(pp->m,pp->n,pp->matrix,pp->start,pp->end,pp->filename);
}

void game(int m,int n,char **matrix,int start,int end,char *filename){
	/*for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			printf("%c",matrix[i][j]);
		}printf("\n");
	}
	printf("\n");*/
	char **charbuffer;
	charbuffer = (char **)malloc(sizeof(char *) * (end-start));
	for(int i =0;i<(end-start);i++){
		charbuffer[i] = (char *)malloc(sizeof(char ) * n);
		memset(charbuffer[i],0,sizeof(charbuffer[i]));
	}
	int count = 0;
	for(int i=start,index =0;i<end;i++,index++){
		for(int k=1;k<n;k+=2){
			charbuffer[index][k] = ' ';
		}
		for(int j=0;j<n;j+=2){
			if(i-1>0 && j-2 > 0  && i+1 <m && j+2 <n){
				if(matrix[i-1][j-2] == '1'){
					count ++;
				}
				if(matrix[i-1][j] =='1'){
					count++;
				}
				if(matrix[i-1][j+2]=='1'){
					count++;
				}
				if(matrix[i][j-2]=='1'){
					count++;
				}
				if(matrix[i][j+2]=='1'){
					count++;
				}
				if(matrix[i+1][j-2]=='1'){
					count++;
				}
				if(matrix[i+1][j]=='1'){
					count++;
				}
				if(matrix[i+1][j+2]=='1'){
					count++;
				}
			}
			else {
				if(i==0){
					if(j==0){
						if(matrix[i][j+2] == '1')
							count++;
						if(matrix[i+1][j+2] =='1')
							count++;
						if(matrix[i+1][j] =='1')
							count++;
					}
					else if( j+2 == n){
						if(matrix[i][j-2] == '1')
							count++;
						if(matrix[i+1][j-2] == '1')
							count++;
						if(matrix[i+1][j] == '1')
							count++;
					}
					else{ 
						if(matrix[i][j-2] == '1')
							count++;
						if(matrix[i][j+2] =='1')
							count++;
						if(matrix[i+1][j-2] == '1')
							count++;
						if(matrix[i+1][j] =='1')
							count++;
						if(matrix[i+1][j+2]=='1')
							count++;
					}
				}
				else if(i+1 == m ){
					if(j==0){
						if(matrix[i][j+2] == '1')
							count++;
						if(matrix[i-1][j+2] =='1')
							count++;
						if(matrix[i-1][j] =='1')
							count++;
					}
					else if( j+2 == n){
						if(matrix[i][j-2] == '1')
							count++;
						if(matrix[i-1][j-2] == '1')
							count++;
						if(matrix[i-1][j] == '1')
							count++;
					}
					else{ 
						if(matrix[i][j-2] == '1')
							count++;
						if(matrix[i][j+2] =='1')
							count++;
						if(matrix[i-1][j-2] == '1')
							count++;
						if(matrix[i-1][j] =='1')
							count++;
						if(matrix[i-1][j+2]=='1')
							count++;
					}
				}
				else{
					if(j==0){
						if(matrix[i-1][j+2] =='1')
							count++;
						if(matrix[i-1][j] =='1')
							count++;
						if(matrix[i][j+2] == '1')
							count++;
						if(matrix[i+1][j] =='1')
							count++;
						if(matrix[i+1][j+2] =='1')
							count++;
					}
					else if( j+2 == n){
						if(matrix[i-1][j-2] =='1')
							count++;
						if(matrix[i-1][j]=='1')
							count++;
						if(matrix[i][j-2] == '1')
							count++;
						if(matrix[i+1][j-2] == '1')
							count++;
						if(matrix[i+1][j] == '1')
							count++;
					}
					else{ 
						if(matrix[i-1][j-2] =='1')
							count++;
						if(matrix[i-1][j] =='1')
							count++;
						if(matrix[i-1][j+2] =='1')
							count++;
						if(matrix[i][j-2]=='1')
							count++;
						if(matrix[i][j+2] =='1')
							count++;
						if(matrix[i+1][j-2] =='1')
							count++;
						if(matrix[i+1][j] =='1')
							count++;
						if(matrix[i+1][j+2] == '1')
							count++;
				
					}
				}
			}	
			if(matrix[i][j] == '1'){
				if(count>=3 && count <7){
					charbuffer[index][j] = '1';
				}
				else{
					charbuffer[index][j] ='0';
				}
			}
			else{
				if(count ==4 ){
					charbuffer[index][j] ='1';
				}
				else{
					charbuffer[index][j] ='0';
				}
			}
			//printf("[%d][%d] count = %d\n",i,j,count);
			count =0;
		}
	}
	FILE *fp;
	fp = fopen(filename,"r+");
	fseek(fp,start * (n+1),SEEK_SET);
	for(int i =0;i<(end-start);i++){
		fwrite(charbuffer[i],n,1,fp);
		//printf("%s\n", charbuffer[i]);
		if(end != m || (i+1)!=(end-start))
			fprintf(fp,"\n");
	}
	//printf("\n");

	fclose(fp);
}
int main(int argc,char *argv[]){
	FILE *fp;
	int m,n;
	int count =0;
	char buffer[MAXSIZE]={0,};
	if((fp=fopen(argv[1],"r"))==NULL){
		fprintf(stderr,"error\n");
	}
	while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		n= strlen(buffer);
		count++;
	}
	m=count;
	printf("m = %d n = %d\n",m,n);
	fclose(fp);
	char **temp_matrix;
	temp_matrix = (char**)malloc(sizeof(char*)*m);
	for(int i =0;i<m;i++){
		temp_matrix[i] = (char *)malloc(sizeof(char)*n);
	}
	/*while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		buffer[strlen(buffer)-1] ='\0';
		strcpy(matrix[index],buffer);
		index++;
	}
	reset_matrix(m,n,temp_matrix,matrix);*/
	while(1){
		int thread_num;
		int input_num;
		int gen_num;
		int process_num;
		printf("input 1.exit 2. normal  3. Process  4. Thread : ");
		scanf("%d",&input_num);
		if(input_num == 1){
			printf("thank you .\n");
			break;
		}
		else if(input_num == 2){
			clock_t start, end;
			int index=0;
			double result;
			printf("please input generation number : ");scanf("%d",&gen_num);
			start = clock();
			for(int i=0;i<gen_num;i++){
				char filename[100];
				char ex_filename[100];
            sprintf(filename,"gen_%d.matrix",i+1);
				if(i==0)
					strcpy(ex_filename,argv[1]);
				else
					sprintf(ex_filename,"gen_%d.matrix",i);
				if(i+1==gen_num)
					strcpy(filename,"output.txt");
            FILE *temp_fp;
           	if((temp_fp = fopen(ex_filename,"r"))==NULL){
					fprintf(stderr,"error open file\n");
					exit(1);
				}
				while(fgets(buffer,sizeof(buffer),temp_fp)!=NULL){
					if(buffer[strlen(buffer)-1] == '\n')	
						buffer[strlen(buffer)-1] = '\0';
					strcpy(temp_matrix[index],buffer);
					//printf("%s\n",temp_matrix[index]);
					index++;
				}
				index =0;	
            memset(buffer,0,sizeof(buffer));
				fclose(temp_fp);

				fp = fopen(filename,"w+");
				fclose(fp);

				game(m,n,temp_matrix,0,m,filename);
   		}
			end = clock();
			result = (double)(end - start);
			printf("func time : %fms\n",result);
		}
		else if(input_num == 3 ){
			clock_t start_t,end_t;
			double result;
			int start=0;
			int jump;
			int index =0;
			char buffer_matrix[m][n];
			printf("input the number of child process : ");
			scanf("%d",&process_num);
			int count_process = process_num;
			int count_m = m;
			printf("please input generation number : ");scanf("%d",&gen_num);
			pid_t child_pid[gen_num][process_num], wpid;
			int status=0;
			start_t = clock();
			for(int i= 0;i<gen_num;i++){
				char filename[100];
            char ex_filename[100];
            sprintf(filename,"gen_%d.matrix",i+1);
            if(i==0)
               strcpy(ex_filename,argv[1]);
            else
               sprintf(ex_filename,"gen_%d.matrix",i);
            if(i+1==gen_num)
               strcpy(filename,"output.txt");
            FILE *temp_fp;
            if((temp_fp = fopen(ex_filename,"r"))==NULL){
               fprintf(stderr,"error open file\n");
               exit(1);
            }
            while(fgets(buffer,sizeof(buffer),temp_fp)!=NULL){
               if(buffer[strlen(buffer)-1] == '\n')
                  buffer[strlen(buffer)-1] = '\0';
               strcpy(temp_matrix[index],buffer);
               //printf("%s\n",temp_matrix[index]);
               index++;
            }
            index =0;
            memset(buffer,0,sizeof(buffer));
            fclose(temp_fp);

            fp = fopen(filename,"w+");
            fclose(fp);
				for (int id = 0;id<process_num;id++){
					if((child_pid[i][id] = fork())==0){ 
						jump = count_m / count_process;
						count_m -= jump;
						count_process -=1;
						game(m,n,temp_matrix,start,start+jump,filename);
						start+=jump;
						exit(0);
					}
					else if(child_pid[i][id] >0){
						jump = count_m /count_process;
						count_m -=jump;
						count_process-=1;
						start+=jump;
					}
				}
				start =0;
				jump =0;
				count_m =m;
				count_process = process_num;
				while((wpid = wait(&status))>0);
			}
			end_t = clock();
			result = (double)(end_t - start_t);
			for(int i =0;i<gen_num;i++){
				printf("%dst gen\n",i+1);
				for(int j =0;j<process_num;j++){
					printf("pid %d\n",child_pid[i][j]);
				}
				printf("\n");
			}
			printf("func time : %fms\n",result);
		}	
	
		else if (input_num ==4){
			clock_t start_t, end_t;
			int start=0;
			int jump=0;
			/*THREAD st;
			st.matrix = (char **)malloc(sizeof(char*) * m);
			for(int i =0;i<m;i++){
				st.matrix[i] = (char *)malloc(sizeof(char) *n);
			}
			st.filename = (char *)malloc(sizeof(char*)*100);*/
         int index=0;
         double result;
			printf("please input number of thread : ");scanf("%d",&thread_num);
         printf("please input generation number : ");scanf("%d",&gen_num);
			int count_m = m;
			int count_thread = thread_num;
			THREAD* st;
			st = (THREAD *)malloc(sizeof(THREAD) * thread_num);
			
         for(int k=0;k<thread_num;k++){
				st[k].matrix = (char **)malloc(sizeof(char*) * m);
      	   for(int i =0;i<m;i++){
      	      st[k].matrix[i] = (char *)malloc(sizeof(char) *n);
      	   }
      	   st[k].filename = (char *)malloc(sizeof(char*)*100);
			}
			pthread_t tid[thread_num];
         start_t = clock();
         for(int i=0;i<gen_num;i++){
            char filename[100];
            char ex_filename[100];
            sprintf(filename,"gen_%d.matrix",i+1);
            if(i==0)
               strcpy(ex_filename,argv[1]);
            else
               sprintf(ex_filename,"gen_%d.matrix",i);
            if(i+1==gen_num)
               strcpy(filename,"output.txt");
            FILE *temp_fp;
            if((temp_fp = fopen(ex_filename,"r"))==NULL){
               fprintf(stderr,"error open file\n");
               exit(1);
            }
            while(fgets(buffer,sizeof(buffer),temp_fp)!=NULL){
               if(buffer[strlen(buffer)-1] == '\n')
                  buffer[strlen(buffer)-1] = '\0';
               strcpy(temp_matrix[index],buffer);
					//strcpy(st.matrix[index],buffer);
               index++;
            }
            index =0;
            memset(buffer,0,sizeof(buffer));
            fclose(temp_fp);
            fp = fopen(filename,"w+");
            fclose(fp);

 				int err;
				printf("\n%dst gen\n",i+1);
				for(int j=0;j<thread_num;j++){
					st[j].m = m;
					st[j].n = n;
					//st[j].matrix = (char **)malloc(sizeof(
					st[j].matrix = temp_matrix;
					st[j].start = start;
					jump = count_m / count_thread;
					count_m -=jump;
					count_thread -= 1;	
					st[j].end = start + jump;
					strcpy(st[j].filename,filename);
					start+=jump;
					//printf("* m : %d n : %d start : %d end : %d filename : %s\n",st[j].m,st[j].n,st[j].start,st[j].end,st[j].filename);
					if(err = pthread_create(&tid[j],NULL,thread_func,(void *)&st[j])){
						fprintf(stderr,"error in thread\n");
						exit(1);
					}
				//	pthread_detach(tid[j]);
				}
				start =0;
				jump =0;
				count_m = m;
				count_thread = thread_num;
				for(int j=0;j<thread_num;j++)
					pthread_join(tid[j],NULL);
         }
			printf("\n");
         end_t = clock();
         result = (double)(end_t - start_t);
         printf("func time : %fms\n",result);
      
		}
		else {
	
			
			
			printf("please input 1~4 number\n");
		}
	}
	return 0;
}

