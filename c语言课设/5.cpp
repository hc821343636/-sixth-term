#include<bits/stdc++.h> 
#define MAX 256
//Node和 Book用于记录 图书馆中的书，Book储存名称，Node储存ID和读者信息 
typedef struct Node {
    int data;			//编号 
    char readerName[MAX];//当前借阅读者的名称 
    struct Node* next;
}Node;
typedef struct Book {
    int inNum;      //馆存数量
    int outNum;		//外借数量 
    char name[MAX]; //名称
    Node* id;  		//编号
    struct Book* next;
}Book;
//Readers: name储存读者名字，
//allBooks链表：对于每个Book节点，inNum,outNum为无用信息，
//只需要书名和改书包含的一系列ID 
typedef struct Readers{
	Book* allBooks;
	char name[MAX];
	struct Readers* next;
}Readers;

char BUF[1024];
Book* libary = NULL;
Readers* reader = NULL;
char s[25];//记录当前模式 

int inintReaders();	//将读者件读取到程序中 
int inintLibrary();	//将图书馆文件读取到程序中
void showBook(Book* bk);	//输出bk中的所有信息 
void showAdmin();		//展示管理员操作台 
void showReader();	//展示读者操作台 
Readers* findReader(char* name);	//在reader链表中寻找 name 的节点 
int searchBookByName(char* name); 	//通过name寻找书籍并打印信息 
Book* searchBookByNameHelper(Book* allBooks, char* name); //在allBook链表中通过name寻找书书籍节点 
int searchBookByID(int id); //通过 id寻找书籍并打印信息 
int deleteBookByName(char *name); // 在libary中删除name的书籍 
int deleteBookByID(int id);	//在libary中删除id的书籍 
void addBook(Book* allBooks,char* name , int id);	// 在allBooks链表中添加新书 

int changeBook(char* name,int id,char* newname, int newid);	// 在libary中改变书籍 
/*若改变书名，则添加一本新书籍，否则改变ID*/

int borrowBook(char* bkname , char* rdname);	// 读者借书操作 
int returnBook(char* bkname , char* rdname);	//读者还书操作 
void ForAdmin(); 					//管理员模式 
void ForReader(char* rdname); 			//读者模式 

/*结束程序，并将当前状态写入到文件中保存起来*/ 
void endReader();
void endLib();
void end();

//----------------可爱的分割线 --------------------------------- 
int main(int argc, char *argv[]) {
    if(inintLibrary() == 0)
    	return 0;
	if(inintReaders() == 0)
    	return 0;
    if(strcmp(argv[1],"-u")==0){
    	ForReader(argv[2]);
	}else{
		ForAdmin(); 
	}
	end();
    return 0;
}
//----------------无情的分割线 --------------------------------- 
void endReader(){
	Readers* rd = reader->next;
	FILE* fp = fopen("myreaders.readers","w");
	while(rd != NULL){
		fprintf(fp,"%s#",rd->name);
		Book* bk = rd->allBooks->next;
		while(bk != NULL){
			fprintf(fp,"%s",bk->name);
			Node* p = bk->id->next;
			while(p != NULL){
				fprintf(fp,",%d",p->data);
				p = p->next;
			}
			fprintf(fp,"#");
			bk = bk->next;
		}
		fprintf(fp,"\n");
		rd = rd->next;
	}
}
void endLib(){
	FILE* fp = fopen("mylibary.libary","w");
	Book* bk = libary->next;
	while(bk != NULL){
		printf("sa\n");
		fprintf(fp,"%s#",bk->name);
		fprintf(fp,"%d#",bk->inNum);
		fprintf(fp,"%d#",bk->outNum);
		Node* p = bk->id->next;
		while(p != NULL){
			fprintf(fp,"%d#%s#",p->data,strlen(p->readerName)>0?p->readerName:".");
			p = p->next;
		}
		fprintf(fp,"\n");
		bk = bk->next;
	}
}
void end(){
	endReader();
	endLib();
	printf("欢迎使用！再见！\n");
}
void ForReader(char* rdname){
	//不存在当前读者的话
	Readers* r = findReader(rdname);
	if(r == NULL){
		Readers* rp = reader;
		r = (Readers*)malloc(sizeof(Readers));
		strcpy(r->name,rdname);
		r->allBooks = (Book*)malloc(sizeof(Book));
		r->allBooks->id = (Node*)malloc(sizeof(Node));
		r->allBooks->id->next = NULL;
		r->allBooks->next = NULL;
		
		r->next = rp->next;
		rp->next = r;
	} 
	while(1){
		showReader();
		int choice = 0;
		scanf("%d",&choice);
		if(choice == 0){
			return ;
		}else if(choice == 1){
			char bkname[MAX]="";
			printf("请输入借阅书籍:\n");
			scanf("%s",bkname);getchar();
			borrowBook(bkname , rdname);
		}else if(choice == 2){
			char bkname[MAX]="";
			printf("请输入归还书籍:\n");
			scanf("%s",bkname);getchar();
			returnBook(bkname , rdname);
		}else{
			printf("Error!\n");
		}
	}
}
int inintReaders(){
	FILE* fp = fopen("myreaders.readers", "r");
    if (fp == NULL) {
        printf("ERROR!");
        return 0;
    }
    reader = (Readers*)malloc(sizeof(Readers));
	reader->next = NULL;
	reader->allBooks = NULL;
	Readers* cur = reader;
    while (!feof(fp)) {
    	//重置BUF区域后读取一行到BUF中并建立一本书 
    	memset(BUF,'\0',sizeof(BUF));
        fgets(BUF, 1024, fp);
        if(BUF[0]=='\0'||BUF[0]=='\n'||BUF[0]=='\r')
        	break;
        int n = strlen(BUF);
        //建立用户 
        Readers* p = (Readers*)malloc(sizeof(Readers));
        p->next = NULL;
        p->allBooks = (Book*)malloc(sizeof(Book));
        p->allBooks->next = NULL;
        //书籍的哑节点 
        Book* curbk = p->allBooks;
        int index = 0;
        char name[MAX] = "";
        int t = 0;
        //读者名字 
        while(index<n && BUF[index]!='#'){
        	name[t] = BUF[index];
        	t++;
        	index ++;
		}
		strcpy(p->name,name);
		//剩下的信息全部是书籍的 
		while(index<n&& BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r'){
			if(BUF[index] == '#'){
				if(index == n-1 || BUF[index]=='\n'||BUF[index]=='\0'||BUF[index]=='\r')
					break;
				index ++;
				if(BUF[index]=='\n'||BUF[index]=='\0'||BUF[index]=='\r')
					break;
				memset(name,'\0',sizeof(name));
				t = 0;
				//建立一本新书 
				Book* bk = (Book*)malloc(sizeof(Book));
				bk->id = (Node*)malloc(sizeof(Node));
				bk->id->next = NULL;
				bk->id->data = -1;
				bk->next = NULL;
				Node* sd = bk->id;
				//书名 
				while(index<n && BUF[index]!=','){
					name[t] = BUF[index];
					t++;
					index++;
				}
				strcpy(bk->name,name);
				index ++;
				//记录下所有的ID 
				while(index<n && BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r' && BUF[index]!='#'){
					if(BUF[index]==','){
						index ++;
					}else{
						int id = 0;
						Node* s = (Node*)malloc(sizeof(Node));
						s->next = NULL;
						while(index<n && BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r' && BUF[index]!='#' && BUF[index]!=','){
							id = id*10 + BUF[index]-'0';
							index ++;
						} 
						s->data = id;
						sd->next = s;
						sd = sd->next;
					}
				} 
				//下一本 
				curbk->next = bk;
				curbk = curbk->next;	
			}else{
				index ++;
			}
		}
       	//下一个读者 
		cur->next = p;
		cur = cur->next;
    }
   	return 1;
}
int inintLibrary() {
    FILE* fp = fopen("mylibary.libary", "r+");
    if (fp == NULL) {
        printf("ERROR!");
        return 0;
    }

    libary = (Book*)malloc(sizeof(Book));//头节点 
    libary->next = NULL;
    Book* cur = libary;
	
    while (!feof(fp)) {
    	//重置BUF区域后读取一行到BUF中并建立一本书 
    	memset(BUF,0,sizeof(BUF));
        fgets(BUF, 1024, fp);
        if(BUF[0]=='\0'||BUF[0]=='\n'||BUF[0]=='\r')
        	break; 
        Book* p = (Book*)malloc(sizeof(Book));
        p->next = NULL;
        int index = 0;
        int n = strlen(BUF);
        
        //读取书名 
        while (index < n && BUF[index] != '#') {
            p->name[index] = BUF[index];
            index++;
        }
        p->name[index] = '\0';
        index++;
        
        //读取数量 
        p->inNum = 0;
        p->outNum = 0;
        int num = 0;
        //当前图书馆存在的数量 
        while (index < n && BUF[index] != '#') {
            num = num * 10 + BUF[index] - '0';
            index++;
        }
        p->inNum = num;
        index++;
        //当前图书馆外借的数量
		num = 0;
		while (index < n && BUF[index] != '#') {
            num = num * 10 + BUF[index] - '0';
            index++;
        }
        p->outNum = num;
        index++;
        //读取编号并制成Node链表 
        p->id = (Node*)malloc(sizeof(Node));
        p->id->data = -1;
        strcpy(p->id->readerName ,"");
        Node* tmp = p->id;
        while (index<n && BUF[index]!='\0' && BUF[index]!='\n') {
            if (BUF[index] == '#') {
                index++;
            }
            else {
                int d = 0;
                //获取ID 
                while (index < n && isdigit(BUF[index])) {
                    d = d * 10 + (BUF[index] - '0');
                    index++;
                }
                if(BUF[index] == '#'){
                	index ++;
				}
				char nameBuf[MAX]="";
				int t = 0;
				while(index<n && BUF[index]!='\n'&&BUF[index]!='#'&&BUF[index]!='\0'&&BUF[index]!='\r'){
					nameBuf[t] = BUF[index];
					index++;
					t++;
				}
                Node* s = (Node*)malloc(sizeof(Node));
                s->data = d;
                if(strcmp(nameBuf,".") == 0)
                	strcpy(s->readerName,"\0");
                else
                	strcpy(s->readerName,nameBuf);
                s->next = NULL;
                tmp->next = s;
                tmp = tmp->next;
            }
        }
  		//下一本 
        cur->next = p;
        cur = cur->next;
    }
    return 1;
}
void addBook(Book* allBooks,char* name , int id) {
	Book* cur = searchBookByNameHelper(allBooks,name);
	if(cur == NULL){
		Book* bk = (Book*)malloc(sizeof(Book));
		strcpy(bk->name,name);
		bk->outNum = 0;
		bk->inNum = 1;
		bk->id = (Node*)malloc(sizeof(Node));
		Node* tmp = bk->id;
		tmp->data = -1;
		Node* s = (Node*)malloc(sizeof(Node));
		s->data = id;
		s->next = NULL;
		tmp->next = s;
		
		Book* cur = allBooks;
		bk->next = cur->next;
		cur->next = bk;
		printf("OK!\n");
	}else{
		cur->inNum++;
		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data = id;
		strcpy(tmp->readerName,"");
		Node* p = cur->id;
		tmp->next = p->next;
		p->next = tmp;
	}
	printf("添加成功！\n");
}
Book* searchBookByNameHelper(Book* allBooks, char* name){
	Book* cur = allBooks->next;
	while(cur != NULL){
		if(strcmp(cur->name , name)==0){
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}
int searchBookByName(char* name) {
	Book* bk = searchBookByNameHelper(libary,name);
	if(bk == NULL){
		printf("抱歉！不存在这本书！\n");
		return 0;
	}
	printf("这是寻找到的书的信息:\n");
	showBook(bk);
	return 1;
}
int searchBookByID(int id) {
	Book* bk = libary->next;
	while(bk != NULL){
		Node* p = bk->id->next;
		while(p != NULL){
			if(p->data == id){
				printf("这是寻找到的书的信息:\n");
				showBook(bk);
				return 1;
			}
		}
	}
	printf("抱歉！不存在这本书！\n");
	return 0;
}
void showAdmin() {
    printf("\n--------图书馆管理系(administrators)-----------\n");
    printf("| 你可以选择以下操作(按下相应数字键):         |\n");
    printf("| 0.退出程序                                  |\n");
    printf("| 1.录入图书信息                              |\n");
    printf("| 2.删除图书信息                              |\n");
    printf("| 3.修改图书信息                              |\n");
    printf("| 4.查询图书信息                              |\n");
    printf("| 5.录入读者信息                              |\n");
    printf("| 6.删除读者信息                              |\n");
    printf("| 7.修改读者信息                              |\n");
    printf("| 8.查询读者信息                              |\n");
    printf("----------------------------------------------\n");
}
void showReader() {
    printf("\n--------图书馆管理系(reader)------------------\n");
    printf("| 你可以选择以下操作(按下相应数字键):         |\n");
    printf("| 0.退出程序                                  |\n");
    printf("| 1.借书                                      |\n");
    printf("| 2.还书                                      |\n");
    printf("----------------------------------------------\n");
}
int deleteBookByName(char *name) {
	Book* bk = libary;
	while(bk->next!=NULL){
		if(strcmp(bk->next->name,name) == 0){
			Book* p = bk->next;
			bk->next = p->next;
			free(p);
			printf("删除成功！\n");
			return 1;
		}
		bk = bk->next;
	}
	printf("删除失败！不存在！\n"); 
	return 0;	
}
int deleteBookByID(int id){
	Book* bk = libary->next;
	while(bk != NULL){
		Node* p = bk->id;
		while(p->next != NULL){
			if(p->next->data == id){
				Node* s = p->next;
				p->next = s->next;
				free(s);
				bk->inNum--;
				printf("删除成功！\n");
				return 1;
			}
			p = p->next;
		}
	}
	printf("删除失败！不存在！\n"); 
	return 0;
}
void showBook(Book* bk){
	if(bk==NULL)
		return ;
	printf("书名: %s\n",bk->name);
	printf("馆存数量: %d\n",bk->inNum);
	printf("外借数量: %d\n",bk->outNum);
	Node* cur = bk->id->next;
	printf("编号列表: \n"); 
	while(cur!=NULL){
		printf("ID:%d 外借情况:%s\n",cur->data,strlen(cur->readerName)>0?cur->readerName:"未借出");
		cur = cur->next;
	}
	printf("\n");
}
int changeBook(char* name,int id,char* newname, int newid){
	Book* bk = searchBookByNameHelper(libary,name);
	if(bk == NULL){
		printf("修改失败！不存在！\n");
		return 0;
	}
	Node* tmp = bk->id;
	while(tmp->next != NULL){
		if(tmp->next->data == id){
			if(name == newname){	//名称未变 
				tmp->next->data = newid;
				printf("修改成功！\n");
				return 1;
			}else{
				Node* s = tmp->next;
				tmp->next = s->next;
				bk->inNum--;
				free(s);
				addBook(libary,newname , newid);//名称已变，新建书本 
				return 1;	
			}
		}
		tmp = tmp->next;
	}
	printf("修改失败！不存在！\n");
	return 0;	
}
Readers* findReader(char* name){
	Readers* cur = reader->next;
	while(cur != NULL){
		if(strcmp(cur->name,name)==0){
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}
int  borrowBook(char* bkname , char* rdname) {
	Book* bk = searchBookByNameHelper(libary,bkname);
	if(bk==NULL){
		printf("借阅失败！不存在！\n"); 
		return 0;
	}else if(bk->inNum == 0){
		printf("借阅失败！不存在！\n");
		return 0;
	}
	Node* p = bk->id->next;
	while(p != NULL && strlen(p->readerName)>0){
		p = p->next;
	}
	if(p == NULL){
		printf("借阅失败！不存在！\n");
		return 0;
	}
	int id = p->data;
	Readers* rd = findReader(rdname);
	if(rd == NULL){
		printf("未查询到读者！\n");
		return 0;
	}else{
		
		strcpy(p->readerName , rdname);
		//在读者已有的书籍中寻找 
		Book* rk = searchBookByNameHelper(rd->allBooks,bkname);
		//不存在 
		if(rk == NULL){
			Book* book = rd->allBooks;
			Book* tmp = (Book*)malloc(sizeof(Book));
			strcpy(tmp->name,bkname);
			tmp->next = NULL;
			//ID的记录 
			tmp->id = (Node*)malloc(sizeof(Node));
			tmp->id->data = -1;
			//新ID加入 
			Node* t = (Node*)malloc(sizeof(Node));
			t->data = id;
			t->next = NULL;
			tmp->id->next = t;
			//将书籍加入
			tmp->next = book->next;
			book->next = tmp; 
		}else{
			Node* tp = rk->id;
			Node* sp = (Node*)malloc(sizeof(Node));	
			sp->data = id;
			sp->next = tp->next;
			tp->next = sp;
		}
	}
	bk->inNum--;
	bk->outNum++;
	printf("借阅成功！\n");
	return 1;
}
int returnBook(char* bkname , char* rdname) {
	Readers* rd = findReader(rdname);
	if(rd == NULL){
		printf("归还失败,未查询到读者！\n");
		return 0;
	}
	Book* bk = searchBookByNameHelper(rd->allBooks,bkname);
	if(bk == NULL){
		printf("归还失败,读者未借阅此书！\n");
		return 0;
	}
	Node* t = bk->id;
	if(t->next == NULL){
		printf("归还失败,读者未借阅此书！\n");
		return 0;
	}
	Node* st = t->next;
	t->next = st->next;
	bk = searchBookByNameHelper(libary,bkname);
	if(bk == NULL)	//不存在 
		addBook(libary,bkname,st->data);
	else{
		Node* p = bk->id->next;
		while(p != NULL){
			if(p->data == st->data){
				strcpy(p->readerName,"");
				break;
			}
			p = p->next;
		}
		//不存在此ID 
		if(p == NULL){
			p = bk->id;
			Node* tp = (Node*)malloc(sizeof(Node));
			tp->data = st->data;
			strcpy(tp->readerName , "");
			tp->next = p->next;
			p->next = tp; 
		}
	}
	free(st);
	bk->inNum++;
	bk->outNum--;
	printf("归还成功！\n");
	return 1;
}
void ForAdmin(){
	while(1){
		showAdmin();
		int choice = 0;
		scanf("%d",&choice);
		if(choice == 0){
			return;
		}
		else if(choice == 1){
			char name[MAX] = "";
			printf("输入新书的名称: \n");
			scanf("%s",name);getchar();
			int id = 0;
			printf("输入新书的ID: \n");
			scanf("%d",&id);
			addBook(libary, name , id);
		}
		else if(choice == 2){
			int ch = 0;
			printf("按书名删除请按0，按ID删除请按1: \n");
			scanf("%d",&ch);
			if(ch == 0){
				printf("请输入书名:\n");
				char name[MAX] = "";
				scanf("%s",name);getchar();
				deleteBookByName(name);
			}else if(ch == 1){
				int id = 0;
				scanf("%d",&id);
				deleteBookByID(id);
			}else{
				printf("Error!\n");
			}
		}
		else if(choice == 3){
			char name[MAX] = "";
			char newname[MAX] = "";
			int id = 0;
			int newid = 0;
			printf("请输入被修改书籍的名称: \n");
			scanf("%s",name);getchar();
			printf("请输入被修改书籍的ID: \n");
			scanf("%d",&id);
			printf("请输入新书籍的名称: \n");
			scanf("%s",newname);getchar();
			printf("请输入新书籍的ID: \n");
			scanf("%d",&newid);
			changeBook(name,id,newname,newid);
		}
		else if(choice == 4){
			int ch = 0;
			printf("按书名查询请按0，按ID查询请按1: \n");
			scanf("%d",&ch);
			if(ch == 0){
				printf("请输入书名:\n");
				char name[MAX] = "";
				scanf("%s",name);getchar();
				searchBookByName(name);
			}else if(ch == 1){
				int id = 0;
				scanf("%d",&id);
				searchBookByID(id);
			}else{
				printf("Error!\n");
			}
		}
		else if(choice == 5){
			printf("请输入读者的名字\n");
			char name[MAX] = "";
			scanf("%s",name);getchar();
			Readers* rd = reader;
			Readers* p = (Readers*)malloc(sizeof(Readers));	
			strcpy(p->name,name);
			p->allBooks = (Book*)malloc(sizeof(Book));
			p->allBooks->id = (Node*)malloc(sizeof(Node));
			p->allBooks->id->next = NULL;
			p->allBooks->next = NULL;

			p->next = rd->next;
			rd->next = p;
			printf("添加成功！\n");
		}
		else if(choice == 6){
			printf("输入删除的读者的名字: \n");
			char name[MAX] = "";
			scanf("%s",name);getchar();
			Readers* rd = reader;
			int flag = 1;
			while(rd->next != NULL){
				if(strcmp(rd->next->name,name) == 0){
					Readers* tp = rd->next;
					rd->next = tp->next;
					free(tp);
					flag = 0;
					printf("删除成功！\n");
					break;
				}
				rd = rd->next;
			}
			if(flag == 1){
				printf("不存在该读者!\n");
			}
		}
		else if(choice == 7){
			char name[MAX] = "";
			char newname[MAX] = "";
			printf("输入被修改的读者的名字: \n");
			printf("输入被修改的读者的新名字: \n");
			Readers* rd = findReader(name);
			if(rd == NULL){
				printf("不存在该读者！\n");
			}else{
				strcpy(rd->name,newname);
				printf("修改成功！\n");
			}
		}
		else if(choice == 8){
			printf("请输入读者名字: \n");
			char name[MAX]= "";
			scanf("%s",name);getchar();
			Readers* rd = findReader(name);
			if(rd == NULL){
				printf("未查询到！");
			}else{
				printf("读者名字: %s",name);
				Book* bk = rd->allBooks->next;
				printf("持有书籍: \n");
				while(bk != NULL){
					printf("%s: ",bk->name);
					Node* tp = bk->id->next;
					while(tp != NULL){
						printf("%d",tp->data);
						if(tp->next!=NULL)
							printf(",");
						tp = tp->next;
					}
					bk = bk->next;
				}
			}
		}
		else {
			printf("Error!\n");
		}
	}
}

